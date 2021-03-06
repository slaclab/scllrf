/**
 *-----------------------------------------------------------------------------
 * Title      : GUNB low level RF device layer EPICS interface
 * ----------------------------------------------------------------------------
 * File       : GUNBLlrfDev.h
 * Author     : Garth Brown, gwbrown@slac.stanford.edu
 * Created    : June 17, 2016
 * Last update: June 26, 2019
 * ----------------------------------------------------------------------------
 * Description:
 * Device layer for code that uses GUNB LLRF chassis registers to do a little
 * more processing before passing data up to application level PVs.
 * Some functionality, such as reading, processing, and requesting waveforms, can
 * be done more cleanly and coherently with access to the contents of UDP packets
 * containing register read and write data. An array of register reads in the same
 * packet are guaranteed to be in the order they were processed by the FPGA. Also,
 * a packet may be assembled with writes in a certain order, guaranteeing they will
 * be processed by the FPGA in that order. Requests or commands that are sent
 * repeatedly, like the 4 write sequence acknowledging a waveform read and requesting
 * the next waveform, can be hard coded, saved and resent rather than composed each
 * time a new waveform is read.
 *
 * ----------------------------------------------------------------------------
 * This file is part of LCLS II. It is subject to
 * the license terms in the LICENSE.txt file found in the top-level directory
 * of this distribution and at:
	* https://confluence.slac.stanford.edu/display/ppareg/LICENSE.html.
 * No part of LCLS II, including this file, may be
 * copied, modified, propagated, or distributed except according to the terms
 * contained in the LICENSE.txt file.
 * ----------------------------------------------------------------------------
**/

#include "GUNBLlrfDev.h"
#include <asynOctetSyncIO.h>
#include <asynCommonSyncIO.h>
#include <limits>
#include <netinet/in.h>
#include <iostream>
#include <math.h>
#include <cmath>
#include <ctgmath>
#include <stdio.h>
#include <stdexcept>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/bindings/lapack/workspace.hpp>
#include <boost/numeric/bindings/traits/traits.hpp>
#include <boost/numeric/bindings/lapack/lapack.hpp>

using namespace std;

using namespace boost::lambda;
using namespace boost::numeric::ublas;
using namespace boost::numeric::bindings::traits;
using namespace boost::numeric::bindings::lapack;

/** Definition of a Vector type with fixed size */
typedef boost::numeric::ublas::vector<double,
        boost::numeric::ublas::bounded_array<double,1024> > bnuVector;

/** Definition of a Matrix type with fixed size */
typedef boost::numeric::ublas::matrix<double,
        boost::numeric::ublas::column_major,
        boost::numeric::ublas::bounded_array<double,1024> > bnuMatrix;

const unsigned GUNBLlrfDev::maxChannel = 32; // for small waveforms, divided into one "channel"/PV per element, this is the size limit


const double GUNBLlrfDev::LOfrequency = 1.320e9;
const double GUNBLlrfDev::ADCfrequency = LOfrequency/14;

const char *GUNBLlrfDev::IfFreqString = "IF_FREQ";
/* Registers */

// Trace Data buffer I/Q data
const char *GUNBLlrfDev::TraceDataChanEnableString = "TRACE_DATA_CHAN_ENABLE";
const char *GUNBLlrfDev::TraceDataNActiveString = "TRACE_DATA_N_ACTIVE";
const char *GUNBLlrfDev::TraceDataRawIString = "TRACE_DATA_RAW_I";
const char *GUNBLlrfDev::TraceDataRawQString = "TRACE_DATA_RAW_Q";
const char *GUNBLlrfDev::TraceDataRawAString = "TRACE_DATA_RAW_A";
const char *GUNBLlrfDev::TraceDataRawPString = "TRACE_DATA_RAW_P";
const char *GUNBLlrfDev::TraceDataIString = "TRACE_DATA_I";
const char *GUNBLlrfDev::TraceDataQString = "TRACE_DATA_Q";
const char *GUNBLlrfDev::TraceDataAString = "TRACE_DATA_A";
const char *GUNBLlrfDev::TraceDataPString = "TRACE_DATA_P";
const char *GUNBLlrfDev::TraceDataScaleString = "TRACE_DATA_SCALE";
const char *GUNBLlrfDev::TraceDataGoldString = "TRACE_DATA_GOLD";

const char* GUNBLlrfDev::TraceDataTimeStepString = "TRACE_DATA_TIME_STEP";
const char* GUNBLlrfDev::TraceDataPollEnableString = "TRACE_DATA_POLL_ENABLE";
const char* GUNBLlrfDev::TraceDataFastUpdateString = "TRACE_DATA_FAST_UPDATE";

// Decay buffer I/Q data
const char *GUNBLlrfDev::DecayChanEnableString = "DECAY_CHAN_ENABLE";
const char *GUNBLlrfDev::DecayNActiveString = "DECAY_N_ACTIVE";
const char *GUNBLlrfDev::DecayRawIString = "DECAY_RAW_I";
const char *GUNBLlrfDev::DecayRawQString = "DECAY_RAW_Q";
const char *GUNBLlrfDev::DecayRawAString = "DECAY_RAW_A";
const char *GUNBLlrfDev::DecayRawPString = "DECAY_RAW_P";
const char *GUNBLlrfDev::DecayIString = "DECAY_I";
const char *GUNBLlrfDev::DecayQString = "DECAY_Q";
const char *GUNBLlrfDev::DecayAString = "DECAY_A";
const char *GUNBLlrfDev::DecayPString = "DECAY_P";
const char *GUNBLlrfDev::DecayScaleString = "DECAY_SCALE";
const char *GUNBLlrfDev::DecayGoldString = "DECAY_GOLD";
const char *GUNBLlrfDev::DecayConstantBString = "DECAY_CONSTANT_B";
const char *GUNBLlrfDev::DecayBWString = "DECAY_BW";
const char *GUNBLlrfDev::DecayStrengthString = "DECAY_STRENGTH";
const char *GUNBLlrfDev::DecayFitSDevString = "DECAY_FIT_STDDEV";
const char* GUNBLlrfDev::DecayTimeStepString = "DECAY_TIME_STEP";
const char* GUNBLlrfDev::DecayPollEnableString = "DECAY_POLL_ENABLE";
const char* GUNBLlrfDev::DecayFastUpdateString = "DECAY_FAST_UPDATE";

#define TEST_MODE
#define LEN 18

// Constants used in llrf_close_loop.py
//	cic_base_period = 33  # default parameter in llrf_dsp.v
//	Tstep = 14./1320e6
//	in_level_0 = 5000
// For the gun, CIC_PERIOD is a factor of 4 higher than other systems.
const unsigned TraceData::CIC_PERIOD = 33*4;
const unsigned TraceData::SHIFT_BASE = 4;
const float TraceData::CLK_FREQ = 1320e6/14; // LCLS-II value from app.py

const unsigned TraceData::SLOW_OFFSET = 17; // register address offset from start of slow buffer to the data we care about

TraceData::TraceData(GUNBLlrfDev *pDriver, DataBuffer *pBuffer, int *rawParamIndex,
		int *iRawParamIndex, int *qRawParamIndex, int *aRawParamIndex, int *pRawParamIndex,
		int *iParamIndex, int *qParamIndex, int *aParamIndex, int *pParamIndex,
		int *decayBWParamIndex, int *decayConstantBParamIndex, int *decayStrengthParamIndex, int *decayFitStddevParamIndex):
		nChan(0), chanKeep(0), newTraceDataAvailable(0), newTraceDataRead(0), pollEnable(true), fastUpdate(false),pollTimeout (0.25),
		doDecayComp(false),pDriver_(pDriver),regBuffer_(pBuffer), regStartAddr_((uint32_t) pBuffer->iStartAddr),
		regEndAddr_((uint32_t) pBuffer->iStartAddr + pBuffer->RegCount -1), iRawParamIndex_(iRawParamIndex),
		qRawParamIndex_(qRawParamIndex), aRawParamIndex_(aRawParamIndex), pRawParamIndex_(pRawParamIndex),
		iParamIndex_(iParamIndex), qParamIndex_(qParamIndex), aParamIndex_(aParamIndex), pParamIndex_(pParamIndex),
		rawParamIndex_(rawParamIndex), decayBWParamIndex_(decayBWParamIndex),
		decayConstantBParamIndex_(decayConstantBParamIndex), decayStrengthParamIndex_(decayStrengthParamIndex),
		decayFitStddevParamIndex_(decayFitStddevParamIndex), gain_(1)
{

	doOnce = true;
	unsigned int i;
	std::stringstream strStmThreadName;
	strStmThreadName << pDriver->portName << hex << regStartAddr_;
	const std::string strThreadName = strStmThreadName.str();

	for (i = 0; i < maxWavesCount; i++)
	{
		std::fill(pIQBuf_[i], pIQBuf_[i] + pBuffer->RegCount, 0);
		std::fill(pRawIQBuf_[i], pRawIQBuf_[i] + pBuffer->RegCount, 0);
		scaleFactor_[i]=1;
	}
	for (i = 0; i < maxWavesCount / 2; i++)
	{
		std::fill(pRawABuf_[i], pRawABuf_[i] + pBuffer->RegCount, 0);
		std::fill(pRawPBuf_[i], pRawPBuf_[i] + pBuffer->RegCount, 0);
		std::fill(pABuf_[i], pABuf_[i] + pBuffer->RegCount, 0);
		std::fill(pPBuf_[i], pPBuf_[i] + pBuffer->RegCount, 0);
		phaseOffset_[i] = 0;
	}

	StartTraceDataRequester(strThreadName.c_str());
}
TraceData::~TraceData()
{
	reqTraceDataEvent.signal();
	rawWaveRead_.signal();
}

/** Constructor for the GUNB class.
 * Calls constructor for the asynPortDriver base class.
 * \param[in] portName The name of the asyn port driver to be created.
 * \param[in] path The path to the peripherial as built by the builder api
 * \param[in] nelms The number of elements of this device (max addr)
 * \paarm[in] nEntries The number of asyn params to be created for each device
 *
 * */
GUNBLlrfDev::GUNBLlrfDev(const char *drvPortName, const char *netPortName)
: GUNBLlrfDrv(drvPortName, netPortName, maxChannel, NUM_GUNB_LLRF_DEV_PARAMS),
	traceData_(this, &TraceDataBuf,  &p_TraceDataWav,
			&p_TraceDataRawI, &p_TraceDataRawQ, &p_TraceDataRawA, &p_TraceDataRawP,
			&p_TraceDataI, &p_TraceDataQ, &p_TraceDataA, &p_TraceDataP,
			&p_TDDecayBW, &p_TDDecayConstantB, &p_TDDecayStrength, &p_TDDecayFitStdDev),
	decayData_(this, &DecayDataBuf,  &p_DecayDataWav,
			&p_DecayRawI, &p_DecayRawQ, &p_DecayRawA, &p_DecayRawP,
			&p_DecayI, &p_DecayQ, &p_DecayA, &p_DecayP,
			&p_DecayBW, &p_DecayConstantB, &p_DecayStrength, &p_DecayFitStdDev),
			phaseStepH(0), phaseStepL(0), phaseModulo(0),
			iFrequency((phaseStepH +(phaseStepL/(4096-phaseModulo)))/2e20)

{
//	bool waveIsReady;

	// Set up message for TraceData acknowledge and trigger data read
	traceData_.traceDataAck[0] = {TraceResetWAdr,1};
	traceData_.traceDataAck[1] = {SlowResetWAdr,0};
	traceData_.traceDataAck[2] = {SlowResetWAdr,1};
	traceData_.traceDataAck[3] = {SlowResetWAdr,0};
	traceData_.traceDataAck[4] = {flagReadMask,blankData};
	htonFpgaRegArray(traceData_.traceDataAck, sizeof(traceData_.traceDataAck)/sizeof(FpgaReg));
	traceData_.statusRequest = {TraceStatus1RAdr | flagReadMask,blankData};
	htonFpgaRegArray(&traceData_.statusRequest, 1);

	// Set up message for Decay acknowledge and trigger data read
	decayData_.traceDataAck[0] = {DecayResetWAdr,1};
	decayData_.traceDataAck[1] = {flagReadMask,blankData};
	decayData_.traceDataAck[2] = {flagReadMask,blankData};
	decayData_.traceDataAck[3] = {flagReadMask,blankData};
	decayData_.traceDataAck[4] = {flagReadMask,blankData};
	htonFpgaRegArray(decayData_.traceDataAck, sizeof(decayData_.traceDataAck)/sizeof(FpgaReg));
	decayData_.statusRequest = {DecayRoEnableRAdr | flagReadMask,blankData};
	htonFpgaRegArray(&decayData_.statusRequest, 1);
	decayData_.doDecayComp = true;

	// Uncomment the next 2 lines for verbose init logging
//	pasynTrace->setTraceMask((pasynUserSelf), 0xb);
//	pasynTrace->setTraceMask(pOctetAsynUser_, 0xb);

	createParam(IfFreqString, asynParamFloat64, &p_IF);


	// Circle Buffer waveforms

	createParam(TraceDataChanEnableString, asynParamUInt32Digital, &p_TraceDataChanEnable);
	createParam(TraceDataNActiveString, asynParamInt32, &p_TraceDataNActive);
	createParam(TraceDataRawIString, asynParamInt32Array, &p_TraceDataRawI);
	createParam(TraceDataRawQString, asynParamInt32Array, &p_TraceDataRawQ);
	createParam(TraceDataRawAString, asynParamFloat32Array, &p_TraceDataRawA);
	createParam(TraceDataRawPString, asynParamFloat32Array, &p_TraceDataRawP);
	createParam(TraceDataIString, asynParamFloat32Array, &p_TraceDataI);
	createParam(TraceDataQString, asynParamFloat32Array, &p_TraceDataQ);
	createParam(TraceDataAString, asynParamFloat32Array, &p_TraceDataA);
	createParam(TraceDataPString, asynParamFloat32Array, &p_TraceDataP);
	createParam(TraceDataScaleString,  asynParamFloat64, &p_TraceDataScale);
	createParam(TraceDataGoldString,  asynParamFloat64, &p_TraceDataGold);

	createParam(TraceDataTimeStepString, asynParamFloat64, &p_TraceDataTimeStep);
	createParam(TraceDataPollEnableString, asynParamUInt32Digital, &p_TraceDataPollEnable);
	createParam(TraceDataFastUpdateString, asynParamUInt32Digital, &p_TraceDataFastUpdate);

	createParam(DecayChanEnableString, asynParamUInt32Digital, &p_DecayChanEnable);
	createParam(DecayNActiveString, asynParamInt32, &p_DecayNActive);
	createParam(DecayRawIString, asynParamInt32Array, &p_DecayRawI);
	createParam(DecayRawQString, asynParamInt32Array, &p_DecayRawQ);
	createParam(DecayRawAString, asynParamFloat32Array, &p_DecayRawA);
	createParam(DecayRawPString, asynParamFloat32Array, &p_DecayRawP);
	createParam(DecayIString, asynParamFloat32Array, &p_DecayI);
	createParam(DecayQString, asynParamFloat32Array, &p_DecayQ);
	createParam(DecayAString, asynParamFloat32Array, &p_DecayA);
	createParam(DecayPString, asynParamFloat32Array, &p_DecayP);
	createParam(DecayScaleString,  asynParamFloat64, &p_DecayScale);
	createParam(DecayGoldString,  asynParamFloat64, &p_DecayGold);
	createParam(DecayBWString, asynParamFloat64, &p_DecayBW);
	createParam(DecayConstantBString, asynParamFloat64, &p_DecayConstantB);
	createParam(DecayStrengthString, asynParamFloat64, &p_DecayStrength);
	createParam(DecayFitSDevString, asynParamFloat64, &p_DecayFitStdDev);
	createParam(DecayTimeStepString, asynParamFloat64, &p_DecayTimeStep);
	createParam(DecayPollEnableString, asynParamUInt32Digital, &p_DecayPollEnable);
	createParam(DecayFastUpdateString, asynParamUInt32Digital, &p_DecayFastUpdate);

	setDoubleParam(p_DecayTimeStep, TraceData::CIC_PERIOD / TraceData::CLK_FREQ);
	// For testing, call this function after adding the FPGA response data copied from Wireshark
//#define TEST_CANNED_RESPONSE
#ifdef TEST_CANNED_RESPONSE
	testCannedResponse();
#endif

	epicsThreadSleep(defaultPollPeriod);
	std::cout << __PRETTY_FUNCTION__ << " initialized " << drvPortName << " with " << NUM_GUNB_LLRF_DEV_PARAMS << " parameters." << std::endl;

	epicsThreadSleep(defaultPollPeriod);
	wakeupPoller();
	wakeupReader();

	// Initialize write register values without writing the registers
	sendBigBuffer(RegmirrorBuf.reqData.data(), RegmirrorBuf.ReqMsgSize);

}

GUNBLlrfDev::~GUNBLlrfDev()
{
	decayData_.reqTraceDataEvent.signal();
	traceData_.reqTraceDataEvent.signal();
}

// To test how the code handles responses from the FPGA, compose a simulated response here.
void GUNBLlrfDev::testCannedResponse()
{
	// set up fake chassis response in this file. It can be quite long.
	// Note that data can be copied from Wireshark. Next time, note the
	// regex that translates to the right format.

#ifdef TEST_CANNED_RESPONSE
	pasynTrace->setTraceMask((pasynUserSelf), 0xb);
	pasynTrace->setTraceMask(pOctetAsynUser_, 0xb);

#include "GUNBDataBuffResponseTest.cpp"
#endif
		std::cout << "<- " <<__PRETTY_FUNCTION__ << endl;
}

asynStatus GUNBLlrfDev::writeUInt32Digital(asynUser *pasynUser, epicsUInt32 value, epicsUInt32 mask)
{
	asynPrint(pasynUser, ASYN_TRACEIO_DEVICE, "--> %s %s: value: %d, mask: %x\n", portName, __PRETTY_FUNCTION__, value, mask);
	unsigned int function = pasynUser->reason;
	int chan;
	asynStatus status = asynSuccess;
	const char *paramName;
	FpgaReg regSendBuf[5]; // LBL reports problems when smaller requests are sent
	std::fill( regSendBuf, regSendBuf + sizeof( regSendBuf )/sizeof( *regSendBuf), (FpgaReg)  {flagReadMask,blankData} );

	/* Fetch the parameter string name for possible use in debugging */
	getParamName(function, &paramName);
	// Some registers have more than 1 "channel"
	getAddress(pasynUser, &chan);
	asynPrint(pasynUser, ASYN_TRACEIO_DEVICE, "%s %s: function=%d, ch. %u, %s, setting to 0x%x\n",
			portName, __PRETTY_FUNCTION__, function, chan, paramName, value);
	// For settings that change the waveform scale,
	// increment tag so inconsistent data is dropped.
	// Numbering convention, to keep consistent with beg0.py:
	//#   0  reserved for never used
	//#   1  reserved for parameter update in progress
	//#   2  reserved for process stopped
	//#   3  unused
	//#  4-11 rotated through [well, 4-123 here, slight change from beg0.py]
	//# I use 4 bits here, even though the hardware is capable of 8
		// TODO: if this works as expected, add other settings that change waveform scale

	if (function == (unsigned int) p_TraceDataChanEnable) // Enable I/Q pairs
	{
		/* Set the parameter in the parameter library. */
		status = (asynStatus) setUIntDigitalParam(0, function, value, mask);

		// Send a message to the FPGA to: set tag to 1 "updating", enable/disable two
		// channels (I/Q pair), set tag to next value in series
//[UNCOMMENT WHEN THERE'S A TAG REGISTER]		regSendBuf[0].addr = (uint32_t) TraceDataDspTagWAdr;
//[UNCOMMENT WHEN THERE'S A TAG REGISTER]		regSendBuf[0].data = (int32_t) 1;
		// Don't bother reading the tag back at this point, it gets changed
		// and read back later in this message
		regSendBuf[0].addr = (uint32_t) KeepWAdr;
		// Enable register has I and Q as their own bits, so set pairs of bits
		regSendBuf[0].data = InterleaveEnableBits(value, value);
		regSendBuf[1].addr = (uint32_t) KeepRAdr | flagReadMask;
		// regSendBuf[1] is a read request, so leave "blankData" there.
//[UNCOMMENT WHEN THERE'S A TAG REGISTER]		regSendBuf[3].addr = (uint32_t) TraceDataDspTagWAdr;
//[UNCOMMENT WHEN THERE'S A TAG REGISTER]		regSendBuf[3].data = (int32_t) traceData_.nextTag();
//[UNCOMMENT WHEN THERE'S A TAG REGISTER]regSendBuf[4].addr = (uint32_t) TraceDataDspTagWAdr | flagReadMask;

		traceData_.chanKeep = (uint16_t) regSendBuf[0].data;
		traceData_.updateRelToAbsIdx();

		htonFpgaRegArray(regSendBuf, sizeof( regSendBuf )/sizeof( *regSendBuf));
		_singleMsgQ.send(regSendBuf, 2 * sizeof( FpgaReg ));
		callParamCallbacks();
	}
	else if (function == (unsigned int) p_TraceDataPollEnable)
	{
		status = (asynStatus) setUIntDigitalParam(0, p_TraceDataPollEnable, value, mask);
		traceData_.pollEnable = value;
		if(value != 0)
		{
			_singleMsgQ.send(traceData_.traceDataAck, sizeof(traceData_.traceDataAck));
		}
	}
	else if (function == (unsigned int) p_DecayPollEnable)
	{
		status = (asynStatus) setUIntDigitalParam(0, p_DecayPollEnable, value, mask);
		decayData_.pollEnable = value;
		if(value != 0)
		{
			_singleMsgQ.send(decayData_.traceDataAck, sizeof(decayData_.traceDataAck));
		}
	}
	else if (function == (unsigned int) p_TraceDataFastUpdate)
	{
		status = (asynStatus) setUIntDigitalParam(0, p_TraceDataFastUpdate, value, mask);
		traceData_.fastUpdate = value;
		if(value != 0)
		{
			_singleMsgQ.send(traceData_.traceDataAck, sizeof(traceData_.traceDataAck));
		}
	}
	else if (function == (unsigned int) p_DecayFastUpdate)
	{
		status = (asynStatus) setUIntDigitalParam(0, p_DecayFastUpdate, value, mask);
		decayData_.fastUpdate = value;
		if(value != 0)
		{
			_singleMsgQ.send(decayData_.traceDataAck, sizeof(decayData_.traceDataAck));
		}
	}
	else
	{
		GUNBLlrfDrv::writeUInt32Digital(pasynUser, value, mask);
	}

	asynPrint(pasynUser, ASYN_TRACEIO_DEVICE, "<-- %s %s: value: %d, mask: %x\n", portName, __PRETTY_FUNCTION__, value, mask);
	return status;
}


/** Called when asyn clients call pasynInt32->write().
 * \param[in] pasynUser pasynUser structure that encodes the reason and address.
 * \param[in] value Pointer to the value to read. */
asynStatus GUNBLlrfDev::writeInt32(asynUser *pasynUser, epicsInt32 value)
{
	unsigned int function = pasynUser->reason;
	int chan;
	asynStatus status = asynSuccess;
	const char *paramName;
	FpgaReg regSendBuf[5]; // LBL reports problems when smaller requests are sent
	std::fill( regSendBuf, regSendBuf + sizeof( regSendBuf )/sizeof( *regSendBuf), (FpgaReg)  {flagReadMask,blankData} );

	/* Fetch the parameter string name for possible use in debugging */
	getParamName(function, &paramName);
	// Some registers have more than 1 "channel"
	getAddress(pasynUser, &chan);
	asynPrint(pasynUser, ASYN_TRACEIO_DEVICE, "--> %s %s: function=%d, ch. %u, %s, set to %d\n",
			portName, __PRETTY_FUNCTION__, function, chan, paramName, value);
	// For settings that change the waveform scale,
	// increment tag so inconsistent data is dropped.
	// Numbering convention, to keep consistent with beg0.py:
	//#   0  reserved for never used
	//#   1  reserved for parameter update in progress
	//#   2  reserved for process stopped
	//#   3  unused
	//#  4-11 rotated through [well, 4-123 here, slight change from beg0.py]
	//# I use 4 bits here, even though the hardware is capable of 8
	// TODO: if this works as expected, add other settings that change waveform scale
	if (function == (unsigned int) p_WaveSampPerW)
	{
			epicsInt32 last_wave_samp_per;
			unsigned int wave_shift, positiveSampPer;
			epicsFloat64 wave_time_step;
			// Protect against read errors or whatever else could cause implausible readbacks
			positiveSampPer = (value & WaveSampPerMask) >= 1? (value & WaveSampPerMask): 1;

			getIntegerParam(p_WaveSampPerW, &last_wave_samp_per);
			if (last_wave_samp_per != (epicsInt32) positiveSampPer)
			{
				//		regSendBuf[0].addr = TraceDataDspTagWAddr;
				//		regSendBuf[0].data = 1; // See beg0.py for tag convention

				// Update waveform scale and Time Step
				printf("new wave_samp_per: %d\n", value);
				wave_shift = traceData_.CalcWaveScale(positiveSampPer);
				printf("new wave_shift: %d\n", wave_shift);

				regSendBuf[0].addr = WaveShiftWAdr;
				regSendBuf[0].data = wave_shift & WaveShiftMask;

				wave_time_step = positiveSampPer * TraceData::CIC_PERIOD / TraceData::CLK_FREQ;
				setDoubleParam(p_TraceDataTimeStep, wave_time_step);
				printf("new wave_time_step: %e s\n", wave_time_step);

				regSendBuf[1].addr = WaveSampPerWAdr;
				regSendBuf[1].data = value;

				//		regSendBuf[3].addr = TraceDataDspTagWAddr;
				//		 = traceData_.nextTag();
				//		regSendBuf[3].data = ;

				status = (asynStatus) setIntegerParam(0, p_WaveSampPerW, value);
				status = (asynStatus) setIntegerParam(0, p_WaveShiftW, wave_shift);

				htonFpgaRegArray(regSendBuf, sizeof( regSendBuf )/sizeof( *regSendBuf));
				_singleMsgQ.send(regSendBuf, 2 * sizeof( FpgaReg ));
				callParamCallbacks();
			}
	}
//	else
//	if (function == p_TagNowW)
//	{
//		status = GUNBLlrfDrv::writeInt32(pasynUser, value);
//
//		status = GUNBLlrfDrv::writeInt32(pasynUser, value);
//	}
	else
	{
		status = GUNBLlrfDrv::writeInt32(pasynUser, value);
	}

	if (status)
		epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize,
				"%s %s: status=%d, function=%d, name=%s, value=%d",
				portName, __PRETTY_FUNCTION__, status, function, paramName, value);
	else
		asynPrint(pasynUser, ASYN_TRACEIO_DEVICE,
			"<-- %s %s: function=%d, name=%s, value=%d\n",
			portName, __PRETTY_FUNCTION__, function, paramName, value);

	return status;
}

asynStatus GUNBLlrfDev::writeFloat64(asynUser *pasynUser, epicsFloat64 value)
{
	int function = pasynUser->reason;
	int chan;
	const char *paramName;
	asynStatus status = asynSuccess;

	// Some registers have more than 1 "channel"
	getAddress(pasynUser, &chan);
	getParamName(function, &paramName);
	asynPrint(pasynUser, ASYN_TRACEIO_DEVICE, "--> %s %s: function=%d, %s channel %d, set to %f\n",
			portName, __PRETTY_FUNCTION__, function, paramName, chan, value);

	if(function == p_TraceDataScale && (size_t) chan < sizeof(traceData_.scaleFactor_)/sizeof(*traceData_.scaleFactor_))
	{
		// Chan in this case corresponds to the published waveforms,
		// where an I/Q pair comprise one channel. Current implementation
		// has separate scale factor array members for I/Q, but keep
		// them equal. There was some talk of being able to set them separately.
		traceData_.scaleFactor_[chan*2] = value;
		traceData_.scaleFactor_[chan*2+1] = value;
		decayData_.scaleFactor_[chan*2] = value;
		decayData_.scaleFactor_[chan*2+1] = value;
//		cout << portName << " Setting scale factor for channel" << chan << " to " << value << endl;
	}
	else if(function == p_TraceDataGold && (size_t) chan < sizeof(traceData_.phaseOffset_)/sizeof(*traceData_.phaseOffset_))
	{
		traceData_.phaseOffset_[chan] = fmod(value,360.0);
//		cout << portName << " Setting \"Gold\" phase offset for channel " << chan << " to " << value << endl;
	}

	status = GUNBLlrfDrv::writeFloat64(pasynUser, value);

	if (status)
		epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize,
				"%s %s: status=%d, function=%d, name=%s, value=%f",
				portName, __PRETTY_FUNCTION__, status, function, paramName, value);
	else
		asynPrint(pasynUser, ASYN_TRACEIO_DEVICE,
			"<-- %s %s: function=%d, name=%s, value=%f\n",
			portName, __PRETTY_FUNCTION__, function, paramName, value);

	callParamCallbacks(chan, chan);
	return status;
}

static void TraceDataRequesterC(void *drvPvt)
{
	//printf("%s: starting\n", __PRETTY_FUNCTION__);
	TraceData *pTraceData = (TraceData*)drvPvt;
	try
	{
		pTraceData->TraceDataRequester();
	}
	catch(std::exception e)
	{
		cout << "Unhandeld exception in TraceDataRequester, thread exiting" << endl;
		cout<< e.what() <<endl;
	}
	printf("%s: exiting\n", __PRETTY_FUNCTION__);
}

/** Starts the poller thread.
 ** Derived classes will typically call this at near the end of their constructor.
 ** Derived classes can typically use the base class implementation of the poller thread,
 ** but are free to re-implement it if necessary.
 ** \param[in] pollPeriod The time between polls. */
asynStatus TraceData::StartTraceDataRequester(const char *netPortName)
{
	std::string  strThreadName(netPortName);
	strThreadName += "waveRequ";

	epicsThreadCreate(strThreadName.c_str(),
			epicsThreadPriorityMedium,
			epicsThreadGetStackSize(epicsThreadStackMedium),
			(EPICSTHREADFUNC)TraceDataRequesterC, (void *)this);
	return asynSuccess;
}

bnuMatrix calculateNonSquarePseudoInverse(const bnuMatrix & m) {
  unsigned M = m.size1();
  unsigned N = m.size2();


  /** Workspace used by the LAPACK gels() routine */
  boost::numeric::bindings::traits::detail::array<double> _workspace(1024);
  bnuMatrix _pinvGMatrix;
  boost::numeric::ublas::vector<double> _s;
  bnuMatrix _u;
  bnuMatrix _v;
  bnuMatrix _a;
  bnuMatrix _sMatrix;
  bnuMatrix _sPrime;
  _s.resize(M);
  _u.resize(M,M);
  _v.resize(N,N, false);
  _a.resize(M,N);

  _u.clear();
  _v.clear();
  _a.clear();
  _s.clear();

  for (unsigned i = 0; i < M; ++i) {
    for (unsigned j = 0; j < N; ++j) {
      _a(i,j) = m(i,j);
    }
  }

  for (unsigned i = 0; i < _s.size(); ++i) {
    _s[i] = 1;
  }

  //** DEBUG
  std::cout << "  _a(GMATRIX): " << std::endl;
  for (int i = 0; i < (int) _a.size1(); ++i) {
    std::cout << "  ";
    for (int j = 0; j < (int) _a.size2(); ++j) {
      std::cout << _a(i, j) << " ";
    }
    std::cout << std::endl;
  }

  std::cout << "  _s: " << std::endl;
  for (int i = 0; i < (int) _s.size(); ++i) {
    std::cout << _s(i) << " ";
  }
  std::cout << std::endl;
   //DEBUG **/

  int status = 0;
  try {
    status = gesvd(_a, _s, _u, _v);
  } catch (...) {
    printf("Failed to find SVD of G Matrix");
throw;
  }
  if (status != 0) {
    printf("Failed to find SVD of G Matrix");
throw;
  }


  _sMatrix.resize(M,M);
  _sMatrix.clear();
  unsigned z = 0;
  for (unsigned i = 0; i < _sMatrix.size1(); ++i) {
    for (unsigned j = 0; j < _sMatrix.size2(); ++j) {
      _sMatrix(i,j) = 0;
      if (i == j) {
	if (z < _s.size()) {
	  _sMatrix(i,j) = _s(z);
	}
	++z;
      }
    }
  }

  // Find the pseudo inverse of S
  _sPrime.resize(N,M);
  _sPrime.clear();
  for (unsigned i = 0; i < (M<N?M:N); ++i) {
    for (unsigned j = 0; j < (M<N?M:N); ++j) {
      if (i == j) {
	_sPrime(i,j) = 1;
      }
    }
  }

  //  boost::numeric::bindings::traits::detail::array<double> _workspace(200);
  status = gelss(_sMatrix, _sPrime, workspace(_workspace));
  if (status != 0) {
    printf("Failed to find pseudo inverse of G Matrix");
throw;
  }

  /** DEBUG
  std::cout << "  _s(eigenvalues): " << std::endl;
  for (int i = 0; i < (int) _s.size(); ++i) {
    std::cout << _s(i) << " ";
  }
  std::cout << std::endl;
  std::cout << "  _u: " << std::endl;
  for (int i = 0; i < (int) _u.size1(); ++i) {
    std::cout << "  ";
    for (int j = 0; j < (int) _u.size2(); ++j) {
      std::cout << _u(i, j) << " ";
    }
    std::cout << std::endl;
  }
  std::cout << "  _v: " << std::endl;
  for (int i = 0; i < (int) _v.size1(); ++i) {
    std::cout << "  ";
    for (int j = 0; j < (int) _v.size2(); ++j) {
      std::cout << _v(i, j) << " ";
    }
    std::cout << std::endl;
  }
 //DEBUG **/

  // Now compute the pseudo inverse of X!
  // Which is X+ = V * _sprime * Utransposed
  _u = trans(_u);
  _v = trans(_v);

  _a = prod(_v, _sPrime);
  _pinvGMatrix = prod(_a, _u);

  //** DEBUG
  std::cout << "  _a(pseudo-inverse): " << std::endl;
  for (int i = 0; i < (int) _a.size1(); ++i) {
    std::cout << "  ";
    for (int j = 0; j < (int) _a.size2(); ++j) {
      std::cout << _a(i, j) << " ";
    }
    std::cout << std::endl;
  }
  std::cout << "  pinv(GMatrix): " << std::endl;
  for (int i = 0; i < (int) _pinvGMatrix.size1(); ++i) {
    std::cout << "  ";
    for (int j = 0; j < (int) _pinvGMatrix.size2(); ++j) {
      std::cout << _pinvGMatrix(i, j) << " ";
    }
    std::cout << std::endl;
  }
  ////DEBUG **/

  if (std::isnan(_a(0,0))) {
    std::cout << "NAN NAN NAN --- nan pseudo inverse found!" << std::endl;
    printf("Calculated NAN pseudo inverse of G Matrix");
throw;
  }
  return _pinvGMatrix;
}

float* TraceData::CavityDecayConstantCompute(
		int *decay_real, int *decay_imag, unsigned int start, unsigned int ch
#define D_REAL(i) (decay_real[i])
#define D_IMAG(i) (decay_imag[i])
)
{
	unsigned int length=LEN - start;
	double invdt = TraceData::CLK_FREQ / TraceData::CIC_PERIOD;
	double energy_hat = 0.0;
	double ar, ai, arr, aii;
	ar = ai = arr = aii = 0;

	bnuMatrix X(2,2*(length-1));
	bnuMatrix Y(1,2*(length-1));

	for (unsigned int i=0; i<length-1;i++) {
		int j = start+i;
		double xr1 = D_REAL(j);
		double xi1 = D_IMAG(j);
		double xr2 = D_REAL(j+1);
		double xi2 = D_IMAG(j+1);
		X(0,i) = 0.5*(xi1 + xi2);
		X(0,length+i-1) = 0.5*(xr1 + xr2);
		X(1,i) = 0.5*(xr1 + xr2);
		X(1,length+i-1) = -0.5*(xi1+xi2);
		Y(0,i) = -xi1 + xi2;
		Y(0,length+i-1) = -xr1 + xr2;

		energy_hat = energy_hat + xr1*xr1+xi1*xi1;
	}

	try
	{
		bnuMatrix A = prod(Y, calculateNonSquarePseudoInverse(X));

		ar = -A(0,0);
		ai = A(0,1);
		// ai.dt = w.dt = 2*pi*f.dt
		// 1/dt = 1.3675e+06
		//  double invdt = 1.7857e+06;
		ai = (ai * invdt) / (2 * M_PI);
		ar = (ar * invdt) / (2 * M_PI);
		arr = A(0,0);
		aii = A(0,1);

	}
	catch(std::exception e)
	{
		cout << "exception type in PseudoInverse call to SVD" << endl;
		cout<< e.what() <<endl;
	}

	double residue = 0.0;
	for (unsigned int i=0; i<2*(length-1);i++) {
		double x1 = X(0,i);
		double x2 = X(1,i);
		double y = Y(0,i);
		double r = (y-(x1*arr)-(x2*aii));
		residue = residue + r*r;
	}

	double stdev = sqrt(residue/(2*length));

	// PVs: decay_constant_b, decay_strength, decay_fit_stddev
	pDriver_->setDoubleParam(ch, *decayBWParamIndex_, ar);
	pDriver_->setDoubleParam(ch, *decayConstantBParamIndex_, ai);
	pDriver_->setDoubleParam(ch, *decayStrengthParamIndex_, energy_hat);
	pDriver_->setDoubleParam(ch, *decayFitStddevParamIndex_, stdev);
	asynPrint(pDriver_->pOctetAsynUser_, ASYN_TRACEIO_DEVICE,
			"Ch. %u: Bandwidth %8.1f Hz  Detune %8.1f Hz  Signal strength %7f  Stdev %5.3f\n", ch, ar, ai, energy_hat, stdev);

	return NULL;
}

// parse register data, write to array PV
//asynStatus TraceData::ProcessTraceDataReadback(const FpgaReg *pFromFpga)
//{
//}


void TraceData::ReqTraceData()
{
	pDriver_->sendBigBuffer(regBuffer_->reqData.data(), regBuffer_->ReqMsgSize);
}
//TraceData WavePoller:
//resetReg = TraceResetWAdr;
//resetMsg = ;
//statusReg = TraceStatus1RAdr | flagReadMask;
//statusMask =
void TraceData::TraceDataRequester()
{

	// Main polling loop
	while (1)
	{
		newTraceDataRead = newTraceDataAvailable; // Indicate that we got the previous signal

		if (!reqTraceDataEvent.wait(pollTimeout))
		{
			if (pDriver_->isShuttingDown_)
			{
				return;
			}

			if(fastUpdate)
			{
				pDriver_->_singleMsgQ.send(&statusRequest, sizeof( FpgaReg ));
			}
			asynPrint(pDriver_->pOctetAsynUser_, ASYN_TRACEIO_DEVICE,
					"%s %s: timed out waiting for data ready flag\n", pDriver_->portName, __PRETTY_FUNCTION__);
			if (pDriver_->isShuttingDown_)
			{
				return;
			}
			continue;
		}

		asynPrint(pDriver_->pOctetAsynUser_, ASYN_TRACEIO_DEVICE,
				"%s %s: signaled by reqWaveEventId_\n", pDriver_->portName, __PRETTY_FUNCTION__);
		// Don't request data if no active channels
		if ( nChan <=0 || !pollEnable )
		{
			asynPrint(pDriver_->pOctetAsynUser_, ASYN_TRACEIO_DEVICE,
					"%s %s: traceData_.nChan=%d\n", pDriver_->portName, __PRETTY_FUNCTION__, nChan);
			epicsThreadSleep(pDriver_->pollPeriod_);
		}
		else
		{
			/* We got an event, rather than a timeout.
			 **/
			if (pDriver_->isShuttingDown_)
			{
				return;
			}

			ReqTraceData();
			if(fastUpdate)
			{
				traceDataAck[4].addr = statusRequest.addr;
			}
			else
			{
				traceDataAck[4].addr = htonl(flagReadMask);
			}
			pDriver_->_singleMsgQ.send(traceDataAck, sizeof(traceDataAck));
			asynPrint(pDriver_->pOctetAsynUser_, ASYN_TRACEIO_DEVICE,
					"%s %s: done sending waveform request\n", pDriver_->portName, __PRETTY_FUNCTION__);

			// Wait for date to be read back, then process I/Q to A/P and publish
			if ( rawWaveRead_.wait(pollTimeout)) // wait returns false if timeout, e.g. dropped reply
			{
				if (pDriver_->isShuttingDown_)
				{
					return;
				}
				unsigned int rel_chan_ix = 0;
				unsigned int nPoints = nChan>0? regBuffer_->RegCount/nChan: 0;
				unsigned int i;
				float Irot, Qrot; // temporary place to hold rotated I & Q values.
				for (rel_chan_ix=0; rel_chan_ix<nChan; ++rel_chan_ix)
				{
					unsigned int chIndex = relToAbsIdx[rel_chan_ix]/2;
					if(relToAbsIdx[rel_chan_ix]%2 == 1) // if this is a Q channel
					{
						unsigned int Iindex = rel_chan_ix-1;
						unsigned int Qindex = rel_chan_ix;
						try
						{
							asynPrint(pDriver_->pOctetAsynUser_, ASYN_TRACEIO_DEVICE,
									"%s %s publishing  waveform %d, from relative channel %d\n",
									pDriver_->portName, __PRETTY_FUNCTION__ , relToAbsIdx[Qindex]/2, Qindex);
							pDriver_->doCallbacksInt32Array(pRawIQBuf_[Qindex], nPoints, *qRawParamIndex_, relToAbsIdx[Qindex]/2);
						}
						catch(std::exception e)
						{
							cout << "Unhandeld exception in TraceDataRequester: " << e.what() <<endl;
						}

						if((rel_chan_ix > 0) && (relToAbsIdx[Qindex]/2 == relToAbsIdx[Iindex]/2)) // if the corresponding I is also active
						{
							for (i=0; i<nPoints; i++)
							{
								try
								{
									pRawABuf_[chIndex][i] = (epicsFloat32) hypot(pRawIQBuf_[Qindex][i], pRawIQBuf_[Iindex][i]);
									pRawPBuf_[chIndex][i] = (epicsFloat32) (atan2(pRawIQBuf_[Qindex][i], pRawIQBuf_[Iindex][i])) * 180.0/M_PI;
									if (doOnce)
									{
										//cout << "A = " << pRawABuf_[chIndex][i] << ", P = " << pRawPBuf_[chIndex][i] << ", I = " << pRawIQBuf_[Iindex][i] << ", Q = " << pRawIQBuf_[Qindex][i];
									}
									Irot = pIQBuf_[Iindex][i] * cos((float)(phaseOffset_[chIndex]*M_PI)/180.0)
											- pIQBuf_[Qindex][i] * sin((float)(phaseOffset_[chIndex]*M_PI)/180.0);
									Qrot = pIQBuf_[Iindex][i] * sin((float)(phaseOffset_[chIndex]*M_PI)/180.0)
											+ pIQBuf_[Qindex][i] * cos((float)(phaseOffset_[chIndex]*M_PI)/180.0);
									pIQBuf_[Iindex][i] = Irot;
									pIQBuf_[Qindex][i] = Qrot;
									pABuf_[chIndex][i] = (epicsFloat32) hypot(Qrot, Irot);
									pPBuf_[chIndex][i] = (epicsFloat32) (atan2(Qrot, Irot)) * 180.0/M_PI;
									// make it wrap arround so that going past 360 wraps around to -360
									pPBuf_[chIndex][i] = fmod(pPBuf_[chIndex][i]+360.0,720.0);
									if (pPBuf_[chIndex][i] < 0)
									{
										pPBuf_[chIndex][i] += 720.0;
									}
									pPBuf_[chIndex][i] -= 360.0;
									if (doOnce)
									{
										//cout << ", P offset " << phaseOffset_[chIndex] << ", Irot = " << Irot << ", Qrot = " << Qrot<< ", Ascl = " << pABuf_[chIndex][i] << ", Pscl = " << pPBuf_[chIndex][i] << endl;
									}
								}
								catch (std::exception& e)
								{
									printf("pIQBufI_[%u][%u] = %f, ", rel_chan_ix, i, pIQBuf_[Iindex][i]);
									printf("pIQBufQ_[%u][%u] = %f, ", Qindex, i, pIQBuf_[Qindex][i]);
									std::cerr << "exception in " << __PRETTY_FUNCTION__ << " caught: " << e.what() << endl;
								}
							}
							try
							{
							asynPrint(pDriver_->pOctetAsynUser_, ASYN_TRACEIO_DEVICE,
									"%s %s publishing amplitude and phase waveforms %d, from relative channels %d/%d\n",
									pDriver_->portName, __PRETTY_FUNCTION__ , chIndex, Qindex, Iindex);

							pDriver_->doCallbacksFloat32Array(pIQBuf_[Iindex], nPoints, *iParamIndex_, chIndex);
							pDriver_->doCallbacksFloat32Array(pIQBuf_[Qindex], nPoints, *qParamIndex_, chIndex);
							pDriver_->doCallbacksFloat32Array(pRawABuf_[chIndex], nPoints, *aRawParamIndex_, chIndex);
							pDriver_->doCallbacksFloat32Array(pRawPBuf_[chIndex], nPoints, *pRawParamIndex_, chIndex);
							pDriver_->doCallbacksFloat32Array(pABuf_[chIndex], nPoints, *aParamIndex_, chIndex);
							pDriver_->doCallbacksFloat32Array(pPBuf_[chIndex], nPoints, *pParamIndex_, chIndex);
							}
							catch(std::exception e)
							{
								cout << "Unhandeld exception in TraceDataRequester publishing waveforms" << endl;
								cout<< e.what() <<endl;
							}

							// TODO: add "if" to select the right channel for this analysis. Cavity? Also, "if" pulsed mode.
							if(doDecayComp)
							{
								try
								{
									CavityDecayConstantCompute(pRawIQBuf_[Iindex], pRawIQBuf_[Qindex], 3, chIndex);
								}
								catch(std::exception e)
								{
									cout << "Unhandeld exception in TraceDataRequester" << endl;
									cout<< e.what() <<endl;
								}
							}

							std::fill( pABuf_[chIndex],
									pABuf_[chIndex] + sizeof( pABuf_[chIndex] )/sizeof( *pABuf_[chIndex]), 0 );
							std::fill( pPBuf_[chIndex],
									pPBuf_[chIndex] + sizeof( pPBuf_[chIndex] )/sizeof( *pPBuf_[chIndex]), 0 );

						}
						else // This is a Q waveform, but the corresponding I isn't active, so clear A and P
						{
							try
							{
								std::fill( pABuf_[chIndex],
										pABuf_[chIndex] + sizeof( pABuf_[chIndex] )/sizeof( *pABuf_[chIndex]), 0 );
								std::fill( pPBuf_[chIndex],
										pPBuf_[chIndex] + sizeof( pPBuf_[chIndex] )/sizeof( *pPBuf_[chIndex]), 0 );
								pDriver_->doCallbacksFloat32Array(pABuf_[chIndex], 1, *aParamIndex_, chIndex);
								pDriver_->doCallbacksFloat32Array(pPBuf_[chIndex], 1, *pParamIndex_, chIndex);
								pDriver_->doCallbacksFloat32Array(pIQBuf_[Qindex], nPoints, *qParamIndex_, chIndex);
							}
							catch(std::exception e)
							{
								cout << "Unhandeld exception in TraceDataRequester publishing Q-only channel " << chIndex<< endl;
								cout<< e.what() <<endl;
							}
						}

					}
					else
					{
						asynPrint(pDriver_->pOctetAsynUser_, ASYN_TRACEIO_DEVICE, "%s %s publishing I waveform %d, from relative channel %d\n",
								pDriver_->portName, __PRETTY_FUNCTION__ , chIndex, rel_chan_ix);
						//				cout << "publishing I waveform " << chIndex << ", from relative channel" << rel_chan_ix << endl;
						pDriver_->doCallbacksInt32Array(pRawIQBuf_[rel_chan_ix], nPoints, *iRawParamIndex_, chIndex);

						// Only publish Q waveform here if the corresponding I isn't enabled.
						// If the corresponding I is enabled, wait until phase offset correction is calculated
						if(relToAbsIdx[rel_chan_ix]/2 != relToAbsIdx[rel_chan_ix+1]/2)
						{
							pDriver_->doCallbacksFloat32Array(pIQBuf_[rel_chan_ix], nPoints, *iParamIndex_, chIndex);
						}
					}
					// TODO: fill with 0 after publishing, change size of unused channels to 0
				}
				doOnce = false;
			}
		}
	}
}

int TraceData::CalcWaveScale(int32_t wave_samp_per)
{
	unsigned int wave_shift;

	wave_shift=ceil(log2(wave_samp_per));
	gain_ = pow((wave_samp_per/pow(2.0,wave_shift)),2.0);

	return wave_shift;
}

void TraceData::updateRelToAbsIdx()
{
	unsigned int rel_chan_ix = 0;
	int abs_chan_ix;
	nChan = chanKeep.count();

	asynPrint(pDriver_->pOctetAsynUser_, ASYN_TRACEIO_DEVICE, "--> %s %s with %lu\n",
			pDriver_->portName, __PRETTY_FUNCTION__, chanKeep.to_ulong() );
	for (rel_chan_ix=0, abs_chan_ix=-1; rel_chan_ix<nChan; ++rel_chan_ix)
	{
		while(! chanKeep.test(++abs_chan_ix))
		{
		}
		asynPrint(pDriver_->pOctetAsynUser_, ASYN_TRACEIO_DEVICE,
		"Relative channel %u maps to absolute channel %u\n",
				rel_chan_ix , abs_chan_ix );
		relToAbsIdx[rel_chan_ix] = abs_chan_ix;
	}
	for(;rel_chan_ix< sizeof(relToAbsIdx)/sizeof(*relToAbsIdx); rel_chan_ix++)
	{
		asynPrint(pDriver_->pOctetAsynUser_, ASYN_TRACEIO_DEVICE,
		"Relative channel %u is not used\n", rel_chan_ix );
		relToAbsIdx[rel_chan_ix] = -1;
	}
	asynPrint(pDriver_->pOctetAsynUser_, ASYN_TRACEIO_DEVICE, "--> %s %s with %u active channels\n",
			pDriver_->portName, __PRETTY_FUNCTION__, nChan );
}

// parse register data, write to array PV
asynStatus TraceData::ProcessTraceDataReadback(const FpgaReg *pFromFpga)
{
	unsigned int regOffset = (pFromFpga->addr & addrMask) - (regStartAddr_ & addrMask);
	unsigned int bufNumber = nChan>0? regOffset % nChan : 0;
	unsigned int bufIndex = nChan>0? regOffset / nChan : 0;

	regBuffer_->setDataAt(regOffset, pFromFpga->data);
//	waveReadback_[regOffset] = pFromFpga->data;
	if (regOffset == (regEndAddr_ - regStartAddr_)) // if this is the last point of the buffer
	{
		asynPrint(pDriver_->pOctetAsynUser_, ASYN_TRACEIO_DEVICE,
				"%s %s: got last waveform datapoint. Publishing.\n", pDriver_->portName, __PRETTY_FUNCTION__);
//		if (doOnce)
//		{
//			cout << "First raw trace data buffer: ";
//			for( int i = 0; i< regOffset; i++)
//			{
//				cout << setw(8) <<regBuffer_->getDataAt(i);
//			}
//			cout << endl;
//		}
		regBuffer_->publish(pDriver_, rawParamIndex_);
	}

	// avoid divide by 0 errors when waveforms are inactive
	if (nChan <=0)
	{
		if(regOffset == 0)
		{
			asynPrint(pDriver_->pOctetAsynUser_, ASYN_TRACEIO_DEVICE,
					"%s %s can't process waveform data with 0 active channels, chan=%d\n",
					pDriver_->portName, __PRETTY_FUNCTION__, nChan);
		}

		return asynError;
	}
	/*[UNCOMMENT IF WE GET A TAG REGISTER]	if( tagMismatch_ ) // Settings changed mid-waveform, so drop it.
	{
		return asynSuccess;
		asynPrint(pDriver_->pOctetAsynUser_, ASYN_TRACEIO_DEVICE,
				"%s %s settings changed mid-waveform read. Drop this one.\n",
				portName, __PRETTY_FUNCTION__);
	}*/
	assert(bufNumber < maxWavesCount);
	pRawIQBuf_[bufNumber][bufIndex] = regBuffer_->getDataAt(regOffset);
	pIQBuf_[bufNumber][bufIndex] = regBuffer_->getDataAt(regOffset) * scaleFactor_[relToAbsIdx[bufNumber]]/gain_;

	if (regOffset == (regEndAddr_ - regStartAddr_)) // if this is the last point of the buffer
	{
		rawWaveRead_.signal();
	}

	return asynSuccess;
}


/**  Extract register address and data from the received message and set the appropriate
 * asyn parameter.
 * Some registers have a "new waveform data ready" flag. If they have this and it is set,
 * set weveIsReady to true.
 * Note: This function should not set waveIsReady to false. That is done by a loop in the
 * calling function.
* \param[in] pFromFpga Data returned from the FPGA for a single register
*/
asynStatus GUNBLlrfDev::processRegReadback(const FpgaReg *pFromFpga)
{
	asynStatus status = asynSuccess;
	assert(pFromFpga->addr&flagReadMask); // This function is only for read registers
	epicsInt32 tmpData;

	/* Map address to parameter, set the parameter in the parameter library. */
	switch (pFromFpga->addr)
	{
	case ModuloWAdr|flagReadMask:

	status = (asynStatus) setIntegerParam(p_ModuloW,
			(pFromFpga->data & ModuloMask) );
	asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DEVICE,
			"%s %s: readback for address=%s, value=0x%x\n", portName, __PRETTY_FUNCTION__,
			ModuloWString,
			(unsigned int) (pFromFpga->data & ModuloMask));
	iFrequency = ADCfrequency * ((phaseStepH + (phaseStepL/(4096-phaseModulo))));

	break;

	case PhaseStepHWAdr|flagReadMask:

	status = (asynStatus) setIntegerParam(p_PhaseStepHW,
			(pFromFpga->data & PhaseStepHMask) );
	asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DEVICE,
			"%s %s: readback for address=%s, value=0x%x\n", portName, __PRETTY_FUNCTION__,
			PhaseStepHWString,
			(unsigned int) (pFromFpga->data & PhaseStepHMask));
	iFrequency = ADCfrequency * ((phaseStepH + (phaseStepL/(4096-phaseModulo))));
	break;

	case PhaseStepLWAdr|flagReadMask:

	status = (asynStatus) setIntegerParam(p_PhaseStepLW,
			(pFromFpga->data & PhaseStepLMask) );
	asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DEVICE,
			"%s %s: readback for address=%s, value=0x%x\n", portName, __PRETTY_FUNCTION__,
			PhaseStepLWString,
			(unsigned int) (pFromFpga->data & PhaseStepLMask));
	iFrequency = ADCfrequency * ((phaseStepH + (phaseStepL/(4096-phaseModulo))));
	break;

	case TraceStatus1RAdr|flagReadMask:
	status = (asynStatus) setUIntDigitalParam(p_TraceStatus1R,
			(pFromFpga->data & TraceStatus1Mask), TraceStatus1Mask);

	// if the waveform poller is caught up, and there is at least one active channel
	if((traceData_.newTraceDataAvailable == traceData_.newTraceDataRead) && (traceData_.nChan > 0))
	{
		// if flags are set for any active channels,
		if (pFromFpga->data & TraceDataIsReadyMask)
		{
			// Set the message counter with a "new waveform" notification
			// to the message counter value for the message we just received
			traceData_.newTraceDataAvailable = lastResponseCount_;

			traceData_.reqTraceDataEvent.signal();
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DEVICE,"%s %s: new waveform data available, signaling the waveform requester\n",
					portName, __PRETTY_FUNCTION__);
		}
		else
		{
			// When fast waveform updates are enabled, poll the flag again as soon as we get a response
			if (traceData_.fastUpdate == true)
			{
				_singleMsgQ.send(&traceData_.statusRequest, sizeof( FpgaReg ));
				asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DEVICE,"%s %s: waveform data not yet available, polling again....\n",
						portName, __PRETTY_FUNCTION__);
			}
		}
	}

	asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DEVICE,
			"%s %s: readback for address=%s, value=0x%x\n", portName, __PRETTY_FUNCTION__,
			TraceStatus1RString,
			(unsigned int) (pFromFpga->data & TraceStatus1Mask));
	break;

	case DecayRoEnableRAdr|flagReadMask:
	status = (asynStatus) setUIntDigitalParam(p_DecayRoEnableR,
			(pFromFpga->data & DecayRoEnableMask), DecayRoEnableMask);

	// if the waveform poller is caught up, and there is at least one active channel
	if((decayData_.newTraceDataAvailable == decayData_.newTraceDataRead) && (decayData_.nChan > 0))
	{
		// if flags are set for any active channels,
		if (pFromFpga->data & DecayDataIsReadyMask)
		{
			// Set the message counter with a "new waveform" notification
			// to the message counter value for the message we just received
			decayData_.newTraceDataAvailable = lastResponseCount_;

			decayData_.reqTraceDataEvent.signal();
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DEVICE,"%s %s: new waveform data available, signaling the waveform requester\n",
					portName, __PRETTY_FUNCTION__);
		}
		else
		{
			// When fast waveform updates are enabled, poll the flag again as soon as we get a response
			if (decayData_.fastUpdate == true)
			{
				_singleMsgQ.send(&decayData_.statusRequest, sizeof( FpgaReg ));
				asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DEVICE,"%s %s: waveform data not yet available, polling again....\n",
						portName, __PRETTY_FUNCTION__);
			}
		}
	}

	asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DEVICE,
			"%s %s: readback for address=%s, value=0x%x\n", portName, __PRETTY_FUNCTION__,
			DecayRoEnableRString,
			(unsigned int) (pFromFpga->data & DecayRoEnableMask));
	break;

	case WaveSampPerWAdr|flagReadMask:
	epicsInt32 last_wave_samp_per;
	unsigned int wave_shift;
	epicsFloat64 wave_time_step;
	// Protect against read errors or whatever else could cause implausible readbacks
	tmpData = (pFromFpga->data & WaveSampPerMask) >= 1? (pFromFpga->data & WaveSampPerMask): 1;

	getIntegerParam(p_WaveSampPerW, &last_wave_samp_per);
	status = (asynStatus) setIntegerParam(p_WaveSampPerW,
			(pFromFpga->data & WaveSampPerMask) );
	if (last_wave_samp_per != pFromFpga->data)
	{
		// Update waveform scale and Time Step
		printf("new_wave_samp_per: %d\n", pFromFpga->data);
		wave_shift = traceData_.CalcWaveScale(tmpData);

		pasynUserSelf->reason = p_WaveShiftW;
		writeInt32(pasynUserSelf, wave_shift & WaveShiftMask);

		wave_time_step = tmpData * TraceData::CIC_PERIOD / TraceData::CLK_FREQ;
		setDoubleParam(p_TraceDataTimeStep, wave_time_step);
		printf("wave_time_step: %e\n", wave_time_step);
	}
	asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DEVICE,
			"%s %s: readback for address=%s, value=0x%x\n", portName, __PRETTY_FUNCTION__,
			WaveSampPerWString,
			(unsigned int) (pFromFpga->data & WaveSampPerMask));
	break;

	case KeepRAdr|flagReadMask:
	tmpData = pFromFpga->data & KeepMask;
	status = (asynStatus) setIntegerParam(p_KeepR,
			(pFromFpga->data & KeepMask));

	if(traceData_.chanKeep != (uint32_t) tmpData)
	{
		traceData_.chanKeep = (uint32_t) (pFromFpga->data & KeepMask);
		status = (asynStatus) setUIntDigitalParam(p_TraceDataChanEnable,
				DeInterleaveBits(traceData_.chanKeep.to_ulong()), KeepMask);

		traceData_.updateRelToAbsIdx();
		// Note: updateRelToAbsIdx also updates nChan
		setIntegerParam(p_TraceDataNActive, traceData_.nChan);
	}
	asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DEVICE,
			"%s %s: readback for address=%s, value=0x%x, now %d active channels\n", portName, __PRETTY_FUNCTION__,
			KeepRString, (unsigned int) (pFromFpga->data & KeepMask), traceData_.nChan);
	break;

	default:
		if( TraceDataRAdr <= (pFromFpga->addr & addrMask) && (pFromFpga->addr & addrMask) <= traceData_.GetEndAddr() )
		{
			traceData_.ProcessTraceDataReadback(pFromFpga);
		}
		else if( DecayDataRAdr <= (pFromFpga->addr & addrMask) && (pFromFpga->addr & addrMask) <= decayData_.GetEndAddr() )
		{
			decayData_.ProcessTraceDataReadback(pFromFpga);
		}
		else if ((pFromFpga->addr >= (ConfigRomOutRAdr|flagReadMask)) &&
				(pFromFpga->addr < ((ConfigRomOutRAdr|flagReadMask) + ConfigRomOutBuf.RegCount)))
		{
			// This one is also processed by grandparent class leepDrv
			GUNBLlrfDrv::processRegReadback(pFromFpga);
			leepDrv::processRegReadback(pFromFpga);
		}
		else if ((pFromFpga->addr >= (RegmirrorRAdr|flagReadMask)) &&
				(pFromFpga->addr < ((RegmirrorRAdr|flagReadMask) + RegmirrorBuf.RegCount)))
		{	// Used for initialization of write-only registers
			FpgaReg tmpReg = *pFromFpga;
			tmpReg.addr -= (RegmirrorRAdr|flagReadMask);
			try
			{
				regInterface tmpRegIf = addrToReg.at(tmpReg.addr);
				if(tmpRegIf.isWritable)
				{
					switch(tmpRegIf.paramType)
					{
					case asynParamInt32:
						status = (asynStatus) setIntegerParam(tmpRegIf.paramNum, pFromFpga->data );
						asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DEVICE,
								"%s %s: Initializing writable int32 parameter %d, address=0x%x, to value=%d\n", portName, __PRETTY_FUNCTION__,
								tmpRegIf.paramNum, (unsigned int) (pFromFpga->addr), pFromFpga->data);
						break;

					case asynParamUInt32Digital:
						status = (asynStatus) setUIntDigitalParam(tmpRegIf.paramNum, pFromFpga->data, 0xFFFFFFFF);
						asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DEVICE,
								"%s %s: Initializing writable digital parameter %d, address=0x%x, to value=0x%x\n", portName, __PRETTY_FUNCTION__,
								tmpRegIf.paramNum, (unsigned int) (pFromFpga->addr), (unsigned int) pFromFpga->data);
						break;

					case asynParamFloat64:
					case asynParamOctet:
					case asynParamInt8Array:
					case asynParamInt16Array:
					case asynParamInt32Array:
					case asynParamFloat32Array:
					case asynParamFloat64Array:
					case asynParamGenericPointer:
					case asynParamNotDefined:     /**< Undefined */
					default:
						asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
								"%s %s: readback for address=0x%x, value=0x%x, maps to invalid param type %u\n", portName, __PRETTY_FUNCTION__,
								(unsigned int) (pFromFpga->addr), (unsigned int) (pFromFpga->data), tmpRegIf.paramType);
					}
				}
			}
			catch (std::out_of_range& e)
			{
				// Not all addresses have registers defined, so this is expected and normal.
				// std::cerr << "No write parameter defined at address " << tmpReg.addr << ", exception caught: " << e.what() << endl;
			}
		}
		else
		{
			status = GUNBLlrfDrv::processRegReadback(pFromFpga);
		}
		break;
	}

	return status;
}

/**  Extract register address and data from the received message and set the appropriate
 * asyn parameter.
* \param[in] pFromFpga Data returned from the FPGA for a single register
*/
asynStatus GUNBLlrfDev::processRegWriteResponse(const FpgaReg *pFromFpga)
{
	asynStatus status = asynSuccess;
	epicsInt32 valueSet[maxMsgSize/sizeof(FpgaReg)]; // Put the value sent to the FPGA here for comparison
	epicsInt32 errorCount;

	assert(!(pFromFpga->addr&flagReadMask)); // This function is only for read registers

	/* Map address to parameter, set the parameter in the parameter library. */
	switch (pFromFpga->addr)
	{
	case ModuloWAdr:
		status = (asynStatus) getIntegerParam(p_ModuloW, valueSet );
		if( (int32_t)(valueSet[0] & ModuloMask) == (pFromFpga->data & ModuloMask))
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DEVICE,
				"%s %s: echo for address=%s, value=0x%x\n", portName, __PRETTY_FUNCTION__,
				ModuloWString, (unsigned int) (pFromFpga->data & ModuloMask));
			iFrequency = ADCfrequency * ((phaseStepH + (phaseStepL/(4096-phaseModulo))));
		}
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s %s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", portName, __PRETTY_FUNCTION__,
				ModuloWString, (unsigned int) (valueSet[0] & ModuloMask), (unsigned int) (pFromFpga->data & ModuloMask));
			status = asynError;
			setParamStatus(p_ModuloW, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, ++errorCount);
		}

		break;
	case PhaseStepHWAdr:
		status = (asynStatus) getIntegerParam(p_PhaseStepHW, valueSet );
		if( (int32_t)(valueSet[0] & PhaseStepHMask) == (pFromFpga->data & PhaseStepHMask))
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DEVICE,
				"%s %s: echo for address=%s, value=0x%x\n", portName, __PRETTY_FUNCTION__,
				PhaseStepHWString, (unsigned int) (pFromFpga->data & PhaseStepHMask));
			iFrequency = ADCfrequency * ((phaseStepH + (phaseStepL/(4096-phaseModulo))));
		}
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s %s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", portName, __PRETTY_FUNCTION__,
				PhaseStepHWString, (unsigned int) (valueSet[0] & PhaseStepHMask), (unsigned int) (pFromFpga->data & PhaseStepHMask));
			status = asynError;
			setParamStatus(p_PhaseStepHW, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, ++errorCount);
		}

		break;
	case PhaseStepLWAdr:
		status = (asynStatus) getIntegerParam(p_PhaseStepLW, valueSet );
		if( (int32_t)(valueSet[0] & PhaseStepLMask) == (pFromFpga->data & PhaseStepLMask))
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DEVICE,
				"%s %s: echo for address=%s, value=0x%x\n", portName, __PRETTY_FUNCTION__,
				PhaseStepLWString, (unsigned int) (pFromFpga->data & PhaseStepLMask));
			iFrequency = ADCfrequency * ((phaseStepH + (phaseStepL/(4096-phaseModulo))));
		}
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s %s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", portName, __PRETTY_FUNCTION__,
				PhaseStepLWString, (unsigned int) (valueSet[0] & PhaseStepLMask), (unsigned int) (pFromFpga->data & PhaseStepLMask));
			status = asynError;
			setParamStatus(p_PhaseStepLW, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, ++errorCount);
		}

		break;
	case TraceResetWAdr:
		// This register is written by waveform request, outside EPICS.
		// Don't worry about asyn parameter values.
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DEVICE,
				"%s %s: echo for address=%s, value=0x%x\n", portName, __PRETTY_FUNCTION__,
				TraceResetWString, (unsigned int) (pFromFpga->data & TraceResetMask));

		break;

	case KeepWAdr:
		traceData_.chanKeep = (pFromFpga->data & KeepMask);
		status = (asynStatus) setUIntDigitalParam(p_TraceDataChanEnable,
				DeInterleaveBits(traceData_.chanKeep.to_ulong()), KeepMask);

		traceData_.updateRelToAbsIdx();
		// Note: updateRelToAbsIdx also updates nChan
		setIntegerParam(p_TraceDataNActive, traceData_.nChan);

		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DEVICE,
				"%s %s KeepWAdr says %d active channels\n",portName, __PRETTY_FUNCTION__,traceData_.nChan);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DEVICE,
				"%s %s: echo for address=%s, value=0x%x\n", portName, __PRETTY_FUNCTION__,
				KeepWString, (unsigned int) (pFromFpga->data & KeepMask));

		break;

	case DecaykeepWAdr:
	decayData_.chanKeep = (pFromFpga->data & KeepMask);

	decayData_.updateRelToAbsIdx();
	// Note: updateRelToAbsIdx also updates nChan
	setIntegerParam(p_DecayNActive, decayData_.nChan);

	printf("%s %s KeepWAdr says %d active channels\n",portName, __PRETTY_FUNCTION__,decayData_.nChan);
	asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DEVICE,
			"%s %s: echo for address=%s, value=0x%x\n", portName, __PRETTY_FUNCTION__,
			DecaykeepWString, (unsigned int) (pFromFpga->data & DecaykeepMask));

	break;

	default:
// TODO: Add checking for arrays. Until then, we'll hit "default" for array values, so not really an error. GWB 8-23-2016
//		getIntegerParam(p_CommErrorCount, &errorCount);
//		setIntegerParam(p_CommErrorCount, errorCount + 1);
		status = GUNBLlrfDrv::processRegWriteResponse(pFromFpga);

		break;
	}

	// TODO: handle arrays

	return status;
}



extern "C" {

/* Configuration routine.  Called directly, or from the iocsh function below */

/** EPICS iocsh callable function to call constructor for the GUNB class.
 * \param[in] portName The name of the asyn port driver to be created.
 * \param[in] netPortName The name of the asynIPport this will use to communicate */
int GUNBLlrfDevConfigure(const char *drvPortName, const char *netPortName)
{
	new GUNBLlrfDev(drvPortName, netPortName);
	return asynSuccess;
}


/* EPICS iocsh shell commands */

static const iocshArg initArg0 = { "drvPortName",iocshArgString};
static const iocshArg initArg1 = { "IP port name",iocshArgString};
static const iocshArg * const initArgs[] = {&initArg0,
		&initArg1};

static const iocshFuncDef initFuncDef = {"GUNBLlrfDevConfigure",2,initArgs};
static void initCallFunc(const iocshArgBuf *args)
{
	GUNBLlrfDevConfigure(args[0].sval, args[1].sval);
}

void GUNBLlrfDevRegister(void)
{
	iocshRegister(&initFuncDef,initCallFunc);
}

epicsExportRegistrar(GUNBLlrfDevRegister);

}



