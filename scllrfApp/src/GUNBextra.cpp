
/**
 *-----------------------------------------------------------------------------
 * Title	  : superconducting low level RF EPICS interface
 * ----------------------------------------------------------------------------
 * File	   : scllrfAsynPortDriver.cpp
 * Author	 : Garth Brown, gwbrown@slac.stanford.edu
 * Created	: June 17, 2016
 * Last update: September 6, 2016
 * ----------------------------------------------------------------------------
 * Description:
 * Prototype for FPGA register access to FPGAs using protocol defined in papers
 * given to SLAC by LBNL (Larry Doolittle). Uses asynPortDriver interface. Some of this
 * should eventually be autogenerated from json files or some such, and templates.
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

#include "GUNBextra.h"

#include <asynOctetSyncIO.h>
#include <asynCommonSyncIO.h>
#include <limits>
#include <netinet/in.h>
#include <iostream>
#include <bitset>
#include <math.h>
#include <stdio.h>
#include "../../newmat10/newmat.h"
#include "../../newmat10/newmatap.h"
#include "../../newmat10/newmatio.h"
#include "../../newmat10/precisio.h"

using namespace std;
using namespace NEWMAT;

const unsigned GUNBextra::maxChannel = 1024; // for small waveforms, divided into one "channel"/PV per element, this is the size limit


const double GUNBextra::LOfrequency = 1.320e9;
const double GUNBextra::ADCfrequency = LOfrequency/14;

const char *GUNBextra::IfFreqString = "IF_FREQ";
/* Registers */

// Circle buffer I/Q data
const char *GUNBextra::TraceDataChanEnableString = "TRACE_DATA_CHAN_ENABLE";
const char *GUNBextra::TraceDataNActiveString = "TRACE_DATA_N_ACTIVE";
const char *GUNBextra::TraceDataIString = "TRACE_DATA_I";
const char *GUNBextra::TraceDataQString = "TRACE_DATA_Q";
const char *GUNBextra::TraceDataAString = "TRACE_DATA_A";
const char *GUNBextra::TraceDataPString = "TRACE_DATA_P";
const char *GUNBextra::DecayConstantBString = "DECAY_CONSTANT_B";
const char *GUNBextra::DecayStrengthString = "DECAY_STRENGTH";
const char *GUNBextra::DecayFitStdDevString = "DECAY_FIT_STDDEV";

const char *GUNBextra::TraceDataMinsRString = "TRACE_DATA_MINS_R";
const char *GUNBextra::TraceDataMaxsRString = "TRACE_DATA_MAXS_R";
const char* GUNBextra::TraceDataTimeStepString = "TRACE_DATA_TIME_STEP";
const char* GUNBextra::TraceDataFastUpdateString = "TRACE_DATA_FAST_UPDATE";

#define TEST_MODE
#define LEN 32

// Constants used in llrf_close_loop.py
//	cic_base_period = 33  # default parameter in llrf_dsp.v
//	Tstep = 14./1320e6
//	in_level_0 = 5000
const unsigned TraceData::CIC_PERIOD = 33;
const unsigned TraceData::SHIFT_BASE = 4;
const float TraceData::CLK_FREQ = 1320e6/14; // LCLS-II value from app.py

const unsigned TraceData::SLOW_OFFSET = 17; // register address offset from start of slow buffer to the data we care about
const unsigned GUNBextra::TraceDataRegCount = sizeof(bufTraceData)/sizeof(*bufTraceData);


//const unsigned TraceData::TraceDataRegCount = GUNBextra::TraceDataRegCount;
//const unsigned TraceData::maxWavesCount = 12; // max channels, max number of waveforms interlaced in waveform buffer
const unsigned TraceData::reqBufSegmentCount = (TraceDataRegCount + maxRegPerMsg -1)/maxRegPerMsg; // # of UDP requests, divide and round up, ~48
const unsigned TraceData::reqMsgSize = TraceDataRegCount + reqBufSegmentCount; // All register addresses plus nonce space


TraceData::TraceData(GUNBextra *pDriver, unsigned int waveAddr, int *rawParamIndex,
		int *iParamIndex, int *qParamIndex, int *aParamIndex, int *pParamIndex,
		int *minsParamIndex, int *maxsParamIndex, epicsInt16 *readBuffer, FpgaReg *requestMsg) :
		nChan_(0), chanKeep_(0), waveReadback_(readBuffer), pDriver_(pDriver), pRequestMsg_(requestMsg),
		readInProgress_(false), regStartAddr_((uint32_t) waveAddr), regEndAddr_((uint32_t) waveAddr + TraceDataRegCount -1),
		iParamIndex_(iParamIndex), qParamIndex_(qParamIndex), aParamIndex_(aParamIndex), pParamIndex_(pParamIndex),
		rawParamIndex_(rawParamIndex), minsParamIndex_(minsParamIndex), maxsParamIndex_(maxsParamIndex), gain_(1)
{
	unsigned int i;
	for (i = 0; i < maxWavesCount; i++)
	{
		std::fill(pIQBuf_[i], pIQBuf_[i] + TraceDataRegCount, 0);
	}
	for (i = 0; i < maxWavesCount / 2; i++)
	{
		std::fill(pABuf_[i], pABuf_[i] + TraceDataRegCount, 0);
		std::fill(pPBuf_[i], pPBuf_[i] + TraceDataRegCount, 0);
	}
}

/** Constructor for the GUNB class.
 * Calls constructor for the asynPortDriver base class.
 * \param[in] portName The name of the asyn port driver to be created.
 * \param[in] path The path to the peripherial as built by the builder api
 * \param[in] nelms The number of elements of this device (max addr)
 * \paarm[in] nEntries The number of asyn params to be created for each device
 *
 * */
GUNBextra::GUNBextra(const char *drvPortName, const char *netPortName)
: GUNBDriver(drvPortName, netPortName, maxChannel, NUM_GUNBEXTRA_PARAMS),
	newTraceDataAvailable_(0), newTraceDataRead_ (0),
	traceData_(this, TraceDataRAdr,  &p_TraceDataWav,
			&p_TraceDataI, &p_TraceDataQ, &p_TraceDataA, &p_TraceDataP,
			&p_TraceDataMinsR, &p_TraceDataMaxsR,  bufTraceData, reqTraceData),
			phaseStepH(0), phaseStepL(0), phaseModulo(0),
			iFrequency((phaseStepH +(phaseStepL/(4096-phaseModulo)))/2e20)

{
	FpgaReg adHocMessage;
	bool waveIsReady;

	pasynTrace->setTraceMask((pasynUserSelf), 0xb);
	pasynTrace->setTraceMask(pOctetAsynUser_, 0xb);

	fillWaveRequestMsg(reqTraceData, sizeof(reqTraceData) / sizeof(*reqTraceData), TraceDataRAdr);

	createParam(IfFreqString, asynParamFloat64, &p_IF);


	// Circle Buffer waveforms

	createParam(TraceDataChanEnableString, asynParamUInt32Digital, &p_TraceDataChanEnable);
	createParam(TraceDataNActiveString, asynParamInt32, &p_TraceDataNActive);
	createParam(TraceDataIString, asynParamInt32Array, &p_TraceDataI);
	createParam(TraceDataQString, asynParamInt32Array, &p_TraceDataQ);
	createParam(TraceDataAString, asynParamFloat32Array, &p_TraceDataA);
	createParam(TraceDataPString, asynParamFloat32Array, &p_TraceDataP);
	createParam(DecayConstantBString, asynParamFloat64, &p_DecayConstantB);
	createParam(DecayStrengthString, asynParamFloat64, &p_DecayStrength);
	createParam(DecayFitStdDevString, asynParamFloat64, &p_DecayFitStdDev);

	createParam(TraceDataMinsRString, asynParamInt32, &p_TraceDataMinsR);
	createParam(TraceDataMaxsRString, asynParamInt32, &p_TraceDataMaxsR);
	createParam(TraceDataTimeStepString, asynParamFloat64, &p_TraceDataTimeStep);
	createParam(TraceDataFastUpdateString, asynParamUInt32Digital, &p_TraceDataFastUpdate);

	// For testing, call this function after adding the FPGA response data copied from Wireshark
	// testCannedResponse();

	// Start with a sensible X axis for circle buffer waveforms
	adHocMessage.addr = WaveSampPerWAdr|flagReadMask;
	adHocMessage.data = 1;
	processRegReadback(&adHocMessage, waveIsReady);


	epicsThreadSleep(defaultPollPeriod);
	std::cout << __PRETTY_FUNCTION__ << " created " << NUM_GUNBEXTRA_PARAMS << " parameters." << std::endl;

	StartTraceDataRequester();

	epicsThreadSleep(defaultPollPeriod);
	wakeupPoller();
	wakeupReader();
}

GUNBextra::~GUNBextra()
{
	isShuttingDown_ = true;
	epicsThreadSleep(0.1); // Allow threads to run and exit
	epicsEventSignal(reqWaveEventId_);
	wakeupPoller();
	wakeupReader();
	epicsThreadSleep(0.1); // Allow threads to run and exit
	wakeupPoller(); // call this twice in case the poller was never set to run
	wakeupReader();
	epicsThreadSleep(0.1); // Allow threads to run and exit
	wakeupReader();
	pasynOctetSyncIO->disconnect(pOctetAsynUser_);
	pasynCommonSyncIO->disconnectDevice(pCommonAsynUser_);
	pasynCommonSyncIO->disconnect(pCommonAsynUser_);
}

// To test how the code handles responses from the FPGA, compose a simulated response here.
void GUNBextra::testCannedResponse()
{
	// set up fake chassis response in this file. It can be quite long.
	// Note that data can be copied from Wireshark. Next time, note the
	// regex that translates to the right format.

//	bool dummy; // For the waveform flag

	pasynTrace->setTraceMask((pasynUserSelf), 0xb);
	pasynTrace->setTraceMask(pOctetAsynUser_, 0xb);

//#include "GunDataBuffResponseTest.cpp"

		std::cout << "<- " <<__PRETTY_FUNCTION__ << endl;
}

asynStatus GUNBextra::writeUInt32Digital(asynUser *pasynUser, epicsUInt32 value, epicsUInt32 mask)
{
asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, "--> %s: value: %d, mask: %x\n", __PRETTY_FUNCTION__, value, mask);
	int function = pasynUser->reason;
	asynStatus status = asynSuccess;
	const char *paramName;
	FpgaReg regSendBuf[5]; // LBL reports problems when smaller requests are sent
	std::fill( regSendBuf, regSendBuf + sizeof( regSendBuf )/sizeof( *regSendBuf), (FpgaReg)  {flagReadMask,blankData} );

	/* Fetch the parameter string name for possible use in debugging */
	getParamName(function, &paramName);
	asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, "%s: function=%d, %s\n",
			__PRETTY_FUNCTION__, function, paramName);

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

	if (function == p_TraceDataChanEnable) // Enable I/Q pairs
	{
		/* Set the parameter in the parameter library. */
		status = (asynStatus) setUIntDigitalParam(0, function, value, mask);

		// Send a message to the FPGA to: set tag to 1 "updating", enable/disable two
		// channels (I/Q pair), set tag to next value in series
//[UNCOMMENT WHEN THERE'S A TAG REGISTER]		regSendBuf[0].addr = (uint32_t) TraceDataDspTagWAdr;
//[UNCOMMENT WHEN THERE'S A TAG REGISTER]		regSendBuf[0].data = (int32_t) 1;
		// Don't bother reading the tag back at this point, it gets changed
		// and read back later in this message
		regSendBuf[1].addr = (uint32_t) KeepWAdr;
		// Enable register has I and Q as their own bits, so set pairs of bits
		regSendBuf[1].data = InterleaveEnableBits(value, value);
// [UNCOMMENT IF KEEP BECOMES READABLE]		regSendBuf[2].addr = (uint32_t) KeepWAdr | flagReadMask;
		// regSendBuf[3] is a read request, so leave "blankData" there.
//[UNCOMMENT WHEN THERE'S A TAG REGISTER]		regSendBuf[3].addr = (uint32_t) TraceDataDspTagWAdr;
//[UNCOMMENT WHEN THERE'S A TAG REGISTER]		regSendBuf[3].data = (int32_t) traceData_.nextTag();
//[UNCOMMENT WHEN THERE'S A TAG REGISTER]regSendBuf[4].addr = (uint32_t) TraceDataDspTagWAdr | flagReadMask;

// [UNCOMMENT IF KEEP BECOMES READABLE]		status = (asynStatus) setUIntDigitalParam(0, p_KeepR, (epicsUInt32) regSendBuf[1].data, mask);

		htonFpgaRegArray(regSendBuf, sizeof( regSendBuf )/sizeof( *regSendBuf));
		_singleMsgQ.send(regSendBuf, 4 * sizeof( FpgaReg ));
	}
	else if (function == p_TraceDataFastUpdate)
	{
		status = (asynStatus) setUIntDigitalParam(0, p_TraceDataFastUpdate, value, mask);
		if(value != 0)
		{
			regSendBuf[0].addr = TraceResetWAdr;
			regSendBuf[0].data = 1;
			regSendBuf[1].addr = flagReadMask;
			regSendBuf[1].data = blankData;
			regSendBuf[2].addr = flagReadMask;
			regSendBuf[2].data = blankData;
			regSendBuf[3].addr = TraceStatus1RAdr | flagReadMask;
			regSendBuf[3].data = blankData;
			htonFpgaRegArray(regSendBuf, 4);
			_singleMsgQ.send(regSendBuf, 4*sizeof( FpgaReg ));
		}
	}
	else
	{
		GUNBDriver::writeUInt32Digital(pasynUser, value, mask);
	}
	return status;
}


/** Called when asyn clients call pasynInt32->write().
 * \param[in] pasynUser pasynUser structure that encodes the reason and address.
 * \param[in] value Pointer to the value to read. */
asynStatus GUNBextra::writeInt32(asynUser *pasynUser, epicsInt32 value)
{
	int function = pasynUser->reason;
	asynStatus status = asynSuccess;
	const char *paramName;
	FpgaReg regSendBuf[5]; // LBL reports problems when smaller requests are sent
	std::fill( regSendBuf, regSendBuf + sizeof( regSendBuf )/sizeof( *regSendBuf), (FpgaReg)  {flagReadMask,blankData} );

	/* Fetch the parameter string name for possible use in debugging */
	getParamName(function, &paramName);
	asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, "--> %s: function=%d, %s, set to %d\n",
			__PRETTY_FUNCTION__, function, paramName, value);

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
	if (function == p_WaveSampPerW)
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

				regSendBuf[1].addr = WaveShiftWAdr;
				regSendBuf[1].data = wave_shift & WaveShiftMask;

				wave_time_step = positiveSampPer * TraceData::CIC_PERIOD / TraceData::CLK_FREQ;
				setDoubleParam(p_TraceDataTimeStep, wave_time_step);
				printf("new wave_time_step: %e s\n", wave_time_step);

				regSendBuf[2].addr = WaveSampPerWAdr;
				regSendBuf[2].data = value;

				//		regSendBuf[3].addr = TraceDataDspTagWAddr;
				//		 = traceData_.nextTag();
				//		regSendBuf[3].data = ;

				status = (asynStatus) setIntegerParam(0, p_WaveSampPerW, value);

				htonFpgaRegArray(regSendBuf, sizeof( regSendBuf )/sizeof( *regSendBuf));
				_singleMsgQ.send(regSendBuf, 4 * sizeof( FpgaReg ));
			}
	}
//	else
//	if (function == p_TagNowW)
//	{
//		status = GUNBDriver::writeInt32(pasynUser, value);
//
//		status = GUNBDriver::writeInt32(pasynUser, value);
//	}
	else
	{
		status = GUNBDriver::writeInt32(pasynUser, value);
	}

	if (status)
		epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize,
				"%s: status=%d, function=%d, name=%s, value=%d",
				__PRETTY_FUNCTION__, status, function, paramName, value);
	else
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER,
			"<-- %s: function=%d, name=%s, value=%d\n",
			__PRETTY_FUNCTION__, function, paramName, value);
	return status;
}

Matrix TraceData::PseudoInverse(const Matrix & m)
{
	Matrix result;
	// int rows = m.Nrows();
	unsigned int cols = m.Ncols();
	// calculate SVD decomposition
	Matrix U,V;
	DiagonalMatrix D;
	try
	{
		SVD(m,D,U,V);
	}
	catch(Exception)
	{
		cout<<Exception::what() <<endl;
	}


	Matrix Dinv(cols,cols);
	Dinv = 0;
	for (unsigned int i=0; i<cols; i++)
		if ( fabs(D(i+1,i+1)) < FloatingPointPrecision::Epsilon() )
			Dinv(i+1,i+1) = 0;
		else
			Dinv(i+1,i+1) = 1/D(i+1,i+1);
	result = V * Dinv * U.t();
	result.Release();
	return result;
}

float* TraceData::CavityDecayConstantCompute(
		int *decay_real, int *decay_imag, unsigned int start
#define D_REAL(i) (decay_real[i])
#define D_IMAG(i) (decay_imag[i])
)
{
	unsigned int length=LEN - start;
	double invdt = 1.7857e+06;  // 100e6/(4*14), wrong for today's setup, but right for test case
	Matrix X(2*length-1,2);
	Matrix Y(2*length-1,1);
	int energy_hat = 0.0;
	for (unsigned int i=0; i<length-1;i++) {
		int j = start+i;
		int xr1 = D_REAL(j);
		int xi1 = D_IMAG(j);
		int xr2 = D_REAL(j+1);
		int xi2 = D_IMAG(j+1);
		X(i+1,1) = 0.5*(xi1 + xi2);
		X(length+i+1,1)=0.5*(xr1 + xr2);
		X(i+1,2) = 0.5*(xr1 + xr2);
		X(length+i+1,2)=-0.5*(xi1+xi2);
		Y(i+1,1) = -xi1 + xi2;
		Y(length+i+1,1) = -xr1 + xr2;
		// Even 64 entries at 32K each can't overflow, with this prescaling
#define PS 8
		energy_hat = energy_hat + (xr1/PS)*(xr1/PS)+(xi1/PS)*(xi1/PS);
	}
	Matrix A = PseudoInverse(X) * Y;
	double ar = -A(1,1);
	double ai = A(2,1);
	// ai.dt = w.dt = 2*pi*f.dt
	// 1/dt = 1.3675e+06
	//  double invdt = 1.7857e+06;
	ai = (ai * invdt) / (2 * 3.14159);
	ar = (ar * invdt) / (2 * 3.14159);
	double arr = A(1,1);
	double aii = A(2,1);
	double residue = 0.0;
	for (unsigned int i=0; i<2*length-1;i++) {
		double x1 = X(i+1,1);
		double x2 = X(i+1,2);
		double y = Y(i+1,1);
		double r = (y-(x1*arr)-(x2*aii));
		residue = residue + r*r;
	}
	double stdev = sqrt(residue/(2*length));
#ifndef TEST_MODE
	arout->set_value(ar*1000);
	aiout->set_value(ai*1000);
	strength->set_value(energy_hat);
	stddev->set_value(stdev*1000);
#else
	// PVs: decay_constant_b, decay_strength, decay_fit_stddev
	asynPrint(pDriver_->pOctetAsynUser_, ASYN_TRACEIO_DEVICE,
			"Bandwidth %8.1f Hz  Detune %8.1f Hz  Signal strength %7d  Stdev %5.3f\n", ar, ai, energy_hat, stdev);
#endif
	return NULL;
}

static void TraceDataRequesterC(void *drvPvt)
{
	//printf("%s: starting\n", __PRETTY_FUNCTION__);
	GUNBextra *pllrfDriver = (GUNBextra*)drvPvt;
	pllrfDriver->TraceDataRequester();
	//printf("%s: exiting\n", __PRETTY_FUNCTION__);
}

/** Starts the poller thread.
 ** Derived classes will typically call this at near the end of their constructor.
 ** Derived classes can typically use the base class implementation of the poller thread,
 ** but are free to re-implement it if necessary.
 ** \param[in] pollPeriod The time between polls. */
asynStatus GUNBextra::StartTraceDataRequester()
{
	epicsThreadCreate("waveformRequester",
			epicsThreadPriorityMedium,
			epicsThreadGetStackSize(epicsThreadStackMedium),
			(EPICSTHREADFUNC)TraceDataRequesterC, (void *)this);
	return asynSuccess;
}

void TraceData::ReqTraceData()
{
	pDriver_->sendBigBuffer(pRequestMsg_, reqMsgSize);
}

void GUNBextra::TraceDataRequester()
{
	epicsInt32 readyBits;

	FpgaReg TraceDataAck[] =
	{
			{0,0},
			{TraceResetWAdr,1},
			{flagReadMask,blankData},
			{flagReadMask,blankData},
			{TraceStatus1RAdr | flagReadMask,blankData},
	};
	printf("\n%s calling htonFpgaRegArray for %u registers of TraceDataAck\n", __PRETTY_FUNCTION__, 5 );
	htonFpgaRegArray(TraceDataAck, sizeof(TraceDataAck)/sizeof(FpgaReg));

	// Main polling loop
	while (1)
	{
		newTraceDataRead_ = newTraceDataAvailable_; // Indicate that we got the previous signal
		reqTraceDataEvent_.wait();
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
					"%s: got data ready flag\n", __PRETTY_FUNCTION__);

		if (isShuttingDown_)
		{
			break;
		}

		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: signaled by reqWaveEventId_\n", __PRETTY_FUNCTION__);
		// Don't request data if no active channels
		if ( traceData_.nChan_ <=0 )
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
						"%s: traceData_.nChan_=%d\n", __PRETTY_FUNCTION__, traceData_.nChan_);
			epicsThreadSleep(pollPeriod_);
		}
		else
		{
			/* We got an event, rather than a timeout.
			 **/
			traceData_.ReqTraceData();

			sendRegRequest(TraceDataAck, sizeof(TraceDataAck)/sizeof(FpgaReg));
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
					"%s: done sending waveform request\n", __PRETTY_FUNCTION__);

		}
	}
}

/*
def calc_xscale(self):
	"""
	Calculate monitor channel gain by combining LO, CIC and data
	truncations. Called after time scale change.
	Returns register for ccfilter.v, total gain and xscale
	"""
	cic_r = self.wave_samp_per * self.cic_period
	self.time_step_mon = cic_r / self.clk_freq
	cic_bit_growth = 2 * np.log2(cic_r)
	cic_snr_bit_growth = .5 * np.log2(cic_r / 2)
	total_bit_growth = (np.log2(self.lo_dds_gain) + cic_bit_growth)

	full_shift = np.floor(total_bit_growth - cic_snr_bit_growth)
	wave_shift = int(max(0, (full_shift - self.shift_base)/2))
	self.cic_gain = 2**(total_bit_growth - (2*wave_shift + self.shift_base) + 1)
*/

int TraceData::CalcWaveScale(int32_t wave_samp_per)
{
	unsigned int cic_r;
	float cic_bit_growth;
	float cic_snr_bit_growth;
	float total_bit_growth;
	unsigned int full_shift;
	unsigned int wave_shift;
	float cic_gain;

	cic_r =  wave_samp_per *CIC_PERIOD;
	cic_bit_growth = 2 * log2(cic_r);
	cic_snr_bit_growth = .5 * log2(cic_r / 2);
	total_bit_growth = cic_bit_growth;
//	printf("total_bit_growth: %f\n", total_bit_growth);
	full_shift = (unsigned int)floor(total_bit_growth - cic_snr_bit_growth);
//	printf("full_shift: %d\n", full_shift);
	wave_shift = (full_shift > SHIFT_BASE) ? (unsigned int)((full_shift - SHIFT_BASE)/2) : 0;
//	printf("wave_shift: %d\n", wave_shift);
	cic_gain = pow(2.0, total_bit_growth - (2*wave_shift + SHIFT_BASE) + 1);
//	printf("cic_gain: %f\n", cic_gain);

	gain_ = cic_gain;

	return wave_shift;
}

// parse register data, write to array PV
asynStatus TraceData::ProcessTraceDataReadback(const FpgaReg *pFromFpga)
{
	unsigned int regOffset = (pFromFpga->addr & addrMask) - (regStartAddr_ & addrMask);
	unsigned int bufNumber = nChan_>0? regOffset % nChan_ : 0;
	unsigned int bufIndex = nChan_>0? regOffset / nChan_ : 0;
	unsigned int rel_chan_ix = 0;
	unsigned int num_of_chans = 0;
	unsigned int abs_chan_ix = 0;
	unsigned int nPoints = nChan_>0? TraceData::TraceDataRegCount/nChan_: 0;
	unsigned int i;

	waveReadback_[regOffset] = pFromFpga->data;
	if (regOffset == (regEndAddr_ - regStartAddr_)) // if this is the last point of the buffer
	{
		asynPrint(pDriver_->pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: got last waveform datapoint. Publishing.\n", __PRETTY_FUNCTION__);

		pDriver_->doCallbacksInt16Array(waveReadback_, TraceData::TraceDataRegCount, *rawParamIndex_, 0);
	}

	// avoid divide by 0 errors when waveforms are inactive
	if (nChan_ <=0)
	{
		if(regOffset == 0)
		{
		asynPrint(pDriver_->pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s can't process waveform data with 0 active channels, chan=%d\n",
				__PRETTY_FUNCTION__, nChan_);
		}

		return asynError;
	}
/*[UNCOMMENT IF WE GET A TAG REGISTER]	if( tagMismatch_ ) // Settings changed mid-waveform, so drop it.
	{
		return asynSuccess;
		asynPrint(pDriver_->pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s settings changed mid-waveform read. Drop this one.\n",
				__PRETTY_FUNCTION__);
	}*/

	pIQBuf_[bufNumber][bufIndex] = (epicsInt16) pFromFpga->data;
//cout << "regOffset is " << regOffset << ", waiting for " << (regEndAddr_ - regStartAddr_) << endl;
	if (regOffset == (regEndAddr_ - regStartAddr_)) // if this is the last point of the buffer
	{
		bitset<maxWavesCount> bitset_chan_keep (chanKeep_);

		for (rel_chan_ix=0; rel_chan_ix<nChan_; ++rel_chan_ix)
		{
			num_of_chans = 0;
			abs_chan_ix = 0;
//			cout << __PRETTY_FUNCTION__ << " with " << bitset_chan_keep.to_string() << "\n";
			for (abs_chan_ix=bitset_chan_keep.size(); num_of_chans<=rel_chan_ix;num_of_chans += bitset_chan_keep.test(abs_chan_ix))
			{
				//num_of_chans = bitset_chan_keep.test(abs_chan_ix) ? ++num_of_chans : num_of_chans;
				--abs_chan_ix;
//				cout << abs_chan_ix;
			}
			//--abs_chan_ix;
//			cout << "relative index:" << rel_chan_ix << " total: " << num_of_chans << " abs index:" << abs_chan_ix << "\n";

			if(abs_chan_ix%2 == 0) // if this is a Q channel
			{
				asynPrint(pDriver_->pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
						"%s publishing Q waveform %d, from relative channel %d\n",
						__PRETTY_FUNCTION__ , abs_chan_ix/2, rel_chan_ix);

				pDriver_->doCallbacksInt32Array(pIQBuf_[rel_chan_ix], nPoints, *qParamIndex_, abs_chan_ix/2);
//				cout << "bitset for " << abs_chan_ix << ": " << bitset_chan_keep.test(abs_chan_ix);
//				cout << ", bitset for " << abs_chan_ix-1 << ": " << bitset_chan_keep.test(abs_chan_ix+1) << endl;
				if(bitset_chan_keep.test(abs_chan_ix+1)) // if the corresponding I is also active
				{
//					cout << "calculating A/P for shell " << shellNum << endl;
					for (i=0; i<nPoints; i++)
					{
						try
						{
							pABuf_[abs_chan_ix/2][i] = (epicsFloat32) hypot(pIQBuf_[rel_chan_ix-1][i], pIQBuf_[rel_chan_ix][i])/gain_;
							pPBuf_[abs_chan_ix/2][i] = (epicsFloat32) (atan2(pIQBuf_[rel_chan_ix-1][i], pIQBuf_[rel_chan_ix][i]));
//							cout << "relative waveform " << rel_chan_ix << ", physical channel " << abs_chan_ix/2 << ", I = " << pIQBuf_[rel_chan_ix-1][i] << ", Q = " << pIQBuf_[rel_chan_ix][i]<< ", A = " << pABuf_[abs_chan_ix/2][i] << ", P = " << pPBuf_[abs_chan_ix/2][i] << endl;
						}
						catch (std::exception& e)
						{
							printf("pIQBufI_[%u][%u] = %d, ", rel_chan_ix, i, pIQBuf_[rel_chan_ix][i]);
							printf("pIQBufQ_[%u][%u] = %d, ", rel_chan_ix-1, i, pIQBuf_[rel_chan_ix-1][i]);
							std::cerr << "exception caught: " << e.what() << endl;
						}
					}

					asynPrint(pDriver_->pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
							"%s publishing amplitude and phase waveforms %d, from relative channels %d/%d\n",
							__PRETTY_FUNCTION__ , abs_chan_ix/2, rel_chan_ix-1, rel_chan_ix);
					pDriver_->doCallbacksFloat32Array(pABuf_[abs_chan_ix/2], nPoints, *aParamIndex_, abs_chan_ix/2);
					pDriver_->doCallbacksFloat32Array(pPBuf_[abs_chan_ix/2], nPoints, *pParamIndex_, abs_chan_ix/2);

					// TODO: add "if" to select the right channel for this analysis. Cavity? Also, "if" pulsed mode.
					CavityDecayConstantCompute(pIQBuf_[rel_chan_ix], pIQBuf_[rel_chan_ix-1], 3);

					std::fill( pABuf_[abs_chan_ix/2],
							pABuf_[abs_chan_ix/2] + sizeof( pABuf_[abs_chan_ix/2] )/sizeof( *pABuf_[abs_chan_ix/2]), 0 );
					std::fill( pPBuf_[abs_chan_ix/2],
							pPBuf_[abs_chan_ix/2] + sizeof( pPBuf_[abs_chan_ix/2] )/sizeof( *pPBuf_[abs_chan_ix/2]), 0 );

				}
				else // This is an I waveform, but the corresponding Q isn't active, so clear A and P
				{
					std::fill( pABuf_[abs_chan_ix/2],
							pABuf_[abs_chan_ix/2] + sizeof( pABuf_[abs_chan_ix/2] )/sizeof( *pABuf_[abs_chan_ix/2]), 0 );
					std::fill( pPBuf_[abs_chan_ix/2],
							pPBuf_[abs_chan_ix/2] + sizeof( pPBuf_[abs_chan_ix/2] )/sizeof( *pPBuf_[abs_chan_ix/2]), 0 );
					pDriver_->doCallbacksFloat32Array(pABuf_[abs_chan_ix/2], 1, *aParamIndex_, abs_chan_ix/2);
					pDriver_->doCallbacksFloat32Array(pPBuf_[abs_chan_ix/2], 1, *pParamIndex_, abs_chan_ix/2);
				}

			}
			else
			{
				asynPrint(pDriver_->pOctetAsynUser_, ASYN_TRACEIO_DRIVER, "%s publishing I waveform %d, from relative channel %d\n",
						__PRETTY_FUNCTION__ , abs_chan_ix/2, rel_chan_ix);
//				cout << "publishing I waveform " << abs_chan_ix/2 << ", from relative channel" << rel_chan_ix << endl;
				pDriver_->doCallbacksInt32Array(pIQBuf_[rel_chan_ix], nPoints, *iParamIndex_, abs_chan_ix/2);
			}

			// TODO: fill with 0 after publishing, change size of unused channels to 0
		}
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
* \param[in] waveIsReady A flag that gets set to true if the appropriate bit was set by the FPGA
*/
asynStatus GUNBextra::processRegReadback(const FpgaReg *pFromFpga, bool &waveIsReady)
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
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				ModuloWString,
				(unsigned int) (pFromFpga->data & ModuloMask));
		iFrequency = ADCfrequency * ((phaseStepH + (phaseStepL/(4096-phaseModulo))));

	break;

	case PhaseStepHWAdr|flagReadMask:

		status = (asynStatus) setIntegerParam(p_PhaseStepHW,
				(pFromFpga->data & PhaseStepHMask) );
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				PhaseStepHWString,
				(unsigned int) (pFromFpga->data & PhaseStepHMask));
		iFrequency = ADCfrequency * ((phaseStepH + (phaseStepL/(4096-phaseModulo))));
	break;

	case PhaseStepLWAdr|flagReadMask:

		status = (asynStatus) setIntegerParam(p_PhaseStepLW,
				(pFromFpga->data & PhaseStepLMask) );
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				PhaseStepLWString,
				(unsigned int) (pFromFpga->data & PhaseStepLMask));
		iFrequency = ADCfrequency * ((phaseStepH + (phaseStepL/(4096-phaseModulo))));
	break;

	case TraceStatus1RAdr|flagReadMask:
	status = (asynStatus) setIntegerParam(p_TraceStatus1R,
			(pFromFpga->data & TraceStatus1Mask));

	// if the waveform poller is caught up, and there is at least one active channel
	if((newTraceDataAvailable_ == newTraceDataRead_) && (traceData_.nChan_ > 0))
	{
	// if flags are set for any active channels,
		if (pFromFpga->data & TraceDataIsReadyMask)
		{
			// Set the message counter with a "new waveform" notification
			// to the message counter value for the message we just received
			newTraceDataAvailable_ = lastResponseCount_;

			reqTraceDataEvent_.signal();
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,"%s: new waveform data available, signaling the waveform requester\n",
					__PRETTY_FUNCTION__);
		}
		else
		{
			unsigned int fastUpdate;
			getUIntDigitalParam(p_TraceDataFastUpdate, &fastUpdate, 1);

			// When fast waveform updates are enabled, poll the flag again as soon as we get a response
			if (fastUpdate == true)
			{
				FpgaReg pollTS1msg = {TraceStatus1RAdr|flagReadMask, blankData};
				htonFpgaRegArray(&pollTS1msg, 1);
				_singleMsgQ.send(&pollTS1msg, sizeof( FpgaReg ));
				asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,"%s: waveform data not yet available, polling again....\n",
						__PRETTY_FUNCTION__);
			}
		}
	}

		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				TraceStatus1RString,
				(unsigned int) (pFromFpga->data & TraceStatus1Mask));
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
	asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
			"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
			WaveSampPerWString,
			(unsigned int) (pFromFpga->data & WaveSampPerMask));
	break;

	case KeepWAdr|flagReadMask:
		tmpData = pFromFpga->data & KeepMask;
		status = (asynStatus) setUIntDigitalParam(p_KeepW,
				(pFromFpga->data & KeepMask) , KeepMask);
		// Keep read and write consistent for this one.
		status = (asynStatus) setUIntDigitalParam(p_KeepW,
				(pFromFpga->data & KeepMask) , KeepMask);

		traceData_.chanKeep_ = tmpData;
		status = (asynStatus) setUIntDigitalParam(p_TraceDataChanEnable,
				DeInterleaveBits(traceData_.chanKeep_), KeepMask);

		// Count the number of bits set
		for (traceData_.nChan_ = 0; tmpData; traceData_.nChan_++)
		{
		tmpData &= tmpData - 1; // clear the least significant bit set
		}
		setIntegerParam(p_TraceDataNActive, traceData_.nChan_);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x, now %d active channels\n", __PRETTY_FUNCTION__,
				KeepWString, (unsigned int) (pFromFpga->data & KeepMask), traceData_.nChan_);
	break;

	default:
		if( TraceDataRAdr <= (pFromFpga->addr & addrMask) && (pFromFpga->addr & addrMask) <= traceData_.GetEndAddr() )
		{
			traceData_.ProcessTraceDataReadback(pFromFpga);
		}
		else
		{
			status = GUNBDriver::processRegReadback(pFromFpga, waveIsReady);
		}
		break;
	}

	return status;
}

/**  Extract register address and data from the received message and set the appropriate
 * asyn parameter.
* \param[in] pFromFpga Data returned from the FPGA for a single register
*/
asynStatus GUNBextra::processRegWriteResponse(const FpgaReg *pFromFpga)
{
	asynStatus status = asynSuccess;
	epicsInt32 valueSet[maxMsgSize/sizeof(FpgaReg)]; // Put the value sent to the FPGA here for comparison
	epicsInt32 tmpData;
	epicsInt32 errorCount;

	assert(!(pFromFpga->addr&flagReadMask)); // This function is only for read registers

	/* Map address to parameter, set the parameter in the parameter library. */
	switch (pFromFpga->addr)
	{
	case ModuloWAdr:
		status = (asynStatus) getIntegerParam(p_ModuloW, valueSet );
		if( (int32_t)(valueSet[0] & ModuloMask) == (pFromFpga->data & ModuloMask))
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: echo for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				ModuloWString, (unsigned int) (pFromFpga->data & ModuloMask));
			iFrequency = ADCfrequency * ((phaseStepH + (phaseStepL/(4096-phaseModulo))));
		}
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
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
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: echo for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				PhaseStepHWString, (unsigned int) (pFromFpga->data & PhaseStepHMask));
			iFrequency = ADCfrequency * ((phaseStepH + (phaseStepL/(4096-phaseModulo))));
		}
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
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
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: echo for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				PhaseStepLWString, (unsigned int) (pFromFpga->data & PhaseStepLMask));
			iFrequency = ADCfrequency * ((phaseStepH + (phaseStepL/(4096-phaseModulo))));
		}
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
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
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: echo for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				TraceResetWString, (unsigned int) (pFromFpga->data & TraceResetMask));

		break;

	case KeepWAdr:
		tmpData = pFromFpga->data & KeepMask;
		//status = (asynStatus) getIntegerParam(p_KeepW, valueSet);
		// [IF KEEP BECOMES READABLE, UNCOMMENT THE LINE ABOVE AND DELETE THE LINES BELOW]
		status = (asynStatus) setIntegerParam(p_KeepW, tmpData);
		valueSet[0] = tmpData;

		if( (valueSet[0] & KeepMask) == (pFromFpga->data & KeepMask))
		{
			traceData_.chanKeep_ = tmpData = (pFromFpga->data & KeepMask);;
			status = (asynStatus) setUIntDigitalParam(p_TraceDataChanEnable,
					DeInterleaveBits(traceData_.chanKeep_), KeepMask);

			// Count the number of bits set
			for (traceData_.nChan_ = 0; tmpData; traceData_.nChan_++)
			{
			tmpData &= tmpData - 1; // clear the least significant bit set
			}
			setIntegerParam(p_TraceDataNActive, traceData_.nChan_);

			printf("%s KeepWAdr says %d active channels\n",__PRETTY_FUNCTION__,traceData_.nChan_);
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: echo for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				KeepWString, (unsigned int) (pFromFpga->data & KeepMask));
		}
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				KeepWString, (unsigned int) (valueSet[0] & KeepMask), (unsigned int) (pFromFpga->data & KeepMask));
			status = asynError;
			setParamStatus(p_KeepW, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, ++errorCount);
		}

		break;

	default:
// TODO: Add checking for arrays. Until then, we'll hit "default" for array values, so not really an error. GWB 8-23-2016
//		getIntegerParam(p_CommErrorCount, &errorCount);
//		setIntegerParam(p_CommErrorCount, errorCount + 1);
		status = GUNBDriver::processRegWriteResponse(pFromFpga);

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
int GUNBextraConfigure(const char *drvPortName, const char *netPortName)
{
	new GUNBextra(drvPortName, netPortName);
	return asynSuccess;
}


/* EPICS iocsh shell commands */

static const iocshArg initArg0 = { "drvPortName",iocshArgString};
static const iocshArg initArg1 = { "IP port name",iocshArgString};
static const iocshArg * const initArgs[] = {&initArg0,
		&initArg1};

static const iocshFuncDef initFuncDef = {"GUNBextraConfigure",2,initArgs};
static void initCallFunc(const iocshArgBuf *args)
{
	GUNBextraConfigure(args[0].sval, args[1].sval);
}

void GUNBextraRegister(void)
{
	iocshRegister(&initFuncDef,initCallFunc);
}

epicsExportRegistrar(GUNBextraRegister);

}



