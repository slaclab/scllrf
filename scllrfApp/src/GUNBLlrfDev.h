
#ifndef GUNBEXTRA_DRIVER_H
#define GUNBEXTRA_DRIVER_H

/**
 *-----------------------------------------------------------------------------
 * Title      : superconducting low level RF EPICS interface
 * ----------------------------------------------------------------------------
 * File       : scllrfAsynPortDriver.h
 * Author     : Garth Brown, gwbrown@slac.stanford.edu
 * Created    : June 17, 2016
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
#include "GUNB.h"
#include <math.h>
#include <bitset>

#include "newmat.h"
#include "newmatap.h"
#include "newmatio.h"
using namespace NEWMAT;

class GUNBExtra;

class TraceData
{
public:

	static const unsigned maxWavesCount = 32; // max channels, max number of waveforms interlaced in waveform buffer

	static const unsigned CIC_PERIOD;
	static const unsigned SHIFT_BASE;
	static const float CLK_FREQ;
	static const unsigned SLOW_OFFSET;

	TraceData(GUNBExtra *pDriver, DataBuffer *pBuffer,
			int *rawParamIndex, int *iRawParamIndex, int *qRawParamIndex, int *aRawParamIndex, int *pRawParamIndex,
			int *iParamIndex, int *qParamIndex, int *aParamIndex,int *pParamIndex,
			int *decayBWParamIndex, int *decayConstantBParamIndex, int *decayStrengthParamIndex, int *decayFitStddevParamIndex);
	~TraceData();
	FpgaReg traceDataAck[5];
	FpgaReg statusRequest;
	void ReqTraceData();
	int CalcWaveScale(int32_t wave_samp_per);
	unsigned int nChan;
	bitset<maxWavesCount> chanKeep;
	unsigned int  relToAbsIdx[maxWavesCount]; // Index is relative index, value is equivalent absolute index
	void updateRelToAbsIdx();
	uint32_t GetEndAddr(){return regEndAddr_;}
	asynStatus ProcessTraceDataReadback(const FpgaReg *pFromFpga); // parse register data, write to array PV
	static epicsMutexId newmatMutexID_; // The newmat library is not thread safe. Only allow one thread at a time to use it.
//	asynStatus ProcessSlowDataReadback(const FpgaReg *pFromFpga); // parse register data, write to various parameters
//	unsigned int nextTag(){tag_ = ((tag_+1-4) % (unsigned int) pow(2,7))+4; return tag_;};
	epicsFloat32 scaleFactor_[maxWavesCount]; // Separate scale factors for I and Q
	epicsFloat32 phaseOffset_[maxWavesCount/2]; // Phase is meaningless without an I/Q pair, so one "gold" phase offset per pair
	void TraceDataRequester(); // When signaled that waveforms are waiting, request them.
	virtual asynStatus StartTraceDataRequester(const char *netPortName); // For system startup

	epicsEvent reqTraceDataEvent; /**< Event to signal the waveform requester */
	unsigned int newTraceDataAvailable; /**< netSendCount value of the latest response with the "new waveform" flag set */
	unsigned int newTraceDataRead; /**< netSendCount for the most recent waveform */
	bool pollEnable; // If false, stop reading waveforms
	bool fastUpdate; // Read waveforms in a tight loop, rather than at the polling rate
	float pollTimeout;
	bool doDecayComp;

private:
	GUNBExtra *pDriver_;
	DataBuffer *regBuffer_;
	uint32_t regStartAddr_;
	uint32_t regEndAddr_;
	int *iRawParamIndex_;
	int *qRawParamIndex_;
	int *aRawParamIndex_;
	int *pRawParamIndex_;
	int *iParamIndex_;
	int *qParamIndex_;
	int *aParamIndex_;
	int *pParamIndex_;
	int *rawParamIndex_;
	int *decayBWParamIndex_;
	int *decayConstantBParamIndex_;
	int *decayStrengthParamIndex_;
	int *decayFitStddevParamIndex_;
	epicsFloat32 gain_;
	epicsEvent waveDataReady_; /**< Event to signal the waveform requester */
	epicsEvent rawWaveRead_; /**< Event to signal the waveform requester */

	float* CavityDecayConstantCompute(int *decay_real, int *decay_imag, unsigned int start, unsigned int ch);
	Matrix PseudoInverse(const Matrix & m);

	epicsFloat32 pIQBuf_[maxWavesCount][GUNBDriver::TraceDataRegCount]; // Data mapped into channels, I/Q separated
	epicsInt32 pRawIQBuf_[maxWavesCount][GUNBDriver::TraceDataRegCount]; // Data mapped into channels, I/Q separated
	epicsFloat32 pABuf_[maxWavesCount/2][GUNBDriver::TraceDataRegCount]; // Amplitude data channels
	epicsFloat32 pRawABuf_[maxWavesCount/2][GUNBDriver::TraceDataRegCount]; // Amplitude data channels
	epicsFloat32 pPBuf_[maxWavesCount/2][GUNBDriver::TraceDataRegCount]; // Phase data channels
	epicsFloat32 pRawPBuf_[maxWavesCount/2][GUNBDriver::TraceDataRegCount]; // Phase data channels

	bool doOnce;
};

class GUNBExtra: public GUNBDriver
{
public:
	GUNBExtra(const char *drvPortName, const char *netPortName);
	static const unsigned maxChannel; // for small waveforms, divided into one "channel"/PV per element, this is the size limit
	virtual ~GUNBExtra();
	virtual asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);
	virtual asynStatus writeUInt32Digital(asynUser *pasynUser, epicsUInt32 value, epicsUInt32 mask);
	virtual asynStatus writeFloat64(asynUser *pasynUser, epicsFloat64 value);

	static const double LOfrequency;
	static const double ADCfrequency;
	static const char *IfFreqString;

	/* Registers */

	// I/Q data
	static const char *TraceDataNActiveString;
	static const char *TraceDataChanEnableString;
	static const char *TraceDataRawIString;
	static const char *TraceDataRawQString;
	static const char *TraceDataRawAString;
	static const char *TraceDataRawPString;
	static const char *TraceDataIString;
	static const char *TraceDataQString;
	static const char *TraceDataAString;
	static const char *TraceDataPString;
	static const char *TraceDataScaleString;
	static const char *TraceDataGoldString;
	static const char* TraceDataTimeStepString;
	static const char* TraceDataPollEnableString;
	static const char* TraceDataFastUpdateString;

	// Cavity decay data
	static const char *DecayNActiveString;
	static const char *DecayChanEnableString;
	static const char *DecayRawIString;
	static const char *DecayRawQString;
	static const char *DecayRawAString;
	static const char *DecayRawPString;
	static const char *DecayIString;
	static const char *DecayQString;
	static const char *DecayAString;
	static const char *DecayPString;
	static const char *DecayScaleString;
	static const char *DecayGoldString;
	static const char *DecayBWString;
	static const char *DecayConstantBString;
	static const char *DecayStrengthString;
	static const char *DecayFitSDevString;
	static const char* DecayTimeStepString;
	static const char* DecayPollEnableString;
	static const char* DecayFastUpdateString;

//	static const unsigned TraceDataRegCount; // # data points for I or Q. Half the registers are I data, half Q
	friend class ::TraceData;

protected:
	virtual asynStatus processRegReadback(const FpgaReg *pFromFpga); // parse register data, write to PVs
	virtual asynStatus processRegWriteResponse(const FpgaReg *pFromFpga);
	void testCannedResponse();

	TraceData traceData_;
	TraceData decayData_;
	unsigned int phaseStepH, phaseStepL, phaseModulo;
	double iFrequency;

	/** Values used for pasynUser->reason, and indexes into the parameter library.
	 * For this prototype, it's read only values that identify the FPGA. */

	// parameters for reading I/Q waveforms
	// Circle buffer I/Q data
	int p_TraceDataNActive;
#define FIRST_GUNBEXTRA_PARAM p_TraceDataNActive
	int p_TraceDataChanEnable;
	int p_TraceDataRawI;
	int p_TraceDataRawQ;
	int p_TraceDataRawA;
	int p_TraceDataRawP;
	int p_TraceDataI;
	int p_TraceDataQ;
	int p_TraceDataA;
	int p_TraceDataP;
	int p_TraceDataScale;
	int p_TraceDataGold;
	int p_TraceDataTimeStep;
	int p_TraceDataPollEnable;
	int p_TraceDataFastUpdate;
	int p_TDDecayBW;
	int p_TDDecayConstantB;
	int p_TDDecayStrength;
	int p_TDDecayFitStdDev;

	int p_DecayNActive;
	int p_DecayChanEnable;
	int p_DecayRawI;
	int p_DecayRawQ;
	int p_DecayRawA;
	int p_DecayRawP;
	int p_DecayI;
	int p_DecayQ;
	int p_DecayA;
	int p_DecayP;
	int p_DecayScale;
	int p_DecayGold;
	int p_DecayTimeStep;
	int p_DecayPollEnable;
	int p_DecayFastUpdate;
	int p_DecayBW;
	int p_DecayConstantB;
	int p_DecayStrength;
	int p_DecayFitStdDev;
	int p_IF; // intermediate frequency

	#define LAST_GUNBEXTRA_PARAM p_IF

#define NUM_GUNBEXTRA_PARAMS (&LAST_GUNBEXTRA_PARAM - &FIRST_GUNBEXTRA_PARAM + 1)

private:

	// masks applied to returned register data
	enum ExtraRegMasks
	{
		TraceKeepRMask = 0x000000FF,
		TraceKeepWMask = 0x000000FF,
		BanyanDataIsReadyMask = 0x40000000, // flag for trace data
		TraceDataIsReadyMask = 0x40000000, // flag for trace data
		DecayDataIsReadyMask = 0x00000001, // flag for trace data
		nptMask = 0x3F000000 // for trace status reg
	};
};

#endif
