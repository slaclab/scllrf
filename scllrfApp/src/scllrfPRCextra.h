
#ifndef SCLLRFPRCEXTRA_DRIVER_H
#define SCLLRFPRCEXTRA_DRIVER_H

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
#include "scllrfPRC.h"

static const unsigned maxChannel = 1024; // for small waveforms, divided into one "channel"/PV per element, this is the size limit

// Waveform data is packed as 2D arrays, not one after the other
// in the network data. See cmoc code "submodules/common_hdl/circle.txt" for more details.
// Summary: (array 1, data 1 and 2), (array 2, data 1 and 2), (array 3, data 1 and 2)....
// Addressing: I/Q waveforms base address = 0x140000
	// additional base offsets, calculated from npt = 1 << ((trace_status >> 24) & 0x3F)
	//   0 * npt  16-bit I and Q
	//   1 * npt  16-bit I and Q
	//   2 * npt  22-bit I
	//   3 * npt  22-bit Q
static const uint32_t traceIQWavesStart = 0x140000; // From FPGA design, base address
static const uint32_t traceIQWavesEnd = 0x14ffff; // max possible
static const unsigned traceIQWaveRegCount = traceIQWavesEnd - traceIQWavesStart + 1;
static const unsigned maxTraceIQWavesCount = 8; // max channels, max number of waveforms interlaced in waveform buffer
static const unsigned traceIQWaveBuffSize = traceIQWaveRegCount/4 + traceIQWaveRegCount/(maxMsgSize-sizeof(FpgaReg)); // divide by number of buffers
/**< Waveform buffer is read in waveSegmentCount segments due to network packet size limits.
 *  1+ is an approximation of "round upwards". -sizeof(FpgaReg) makes space for the nonce */
static const unsigned traceIQWaveSegmentCount = 1 + (sizeof(FpgaReg) * traceIQWaveBuffSize)
				/ (maxMsgSize - sizeof(FpgaReg));

/**< Size of each segment, in number of registers */
static const unsigned traceIQWaveSegmentSize = (maxMsgSize/sizeof(FpgaReg));
//		+ (waveBuffSize + waveSegmentCount) / waveSegmentCount;

// ----  Waveform data at 0x170000 to 0x170fff, "circle buffer" llrfCircleData  ----
//
// digDspCircleBufFlip register: write 1 to bit 0 or bit 1 to flip between the buffer
// with ready data and the buffer that is filling, for cavities 0 or 1 respectively.
//
// LlrfCircleReady register: bits 0 or 1 indicate that the active buffer is full
// and ready to be read
//
// digDspMuxShell0DspChanKeep, digDspMuxShell1DspChanKeep: status data that correlates with the
// waveforms, should be read at the same time.
//
// The sequence would be to wait for the bits in the LlrfCircleReady register to be set,
// read the corresponding buffer, read digDspMuxShell*DspChanKeep, then write a 1 to the
// bit in digDspCircleBufFlip to indicate that you're done reading that buffer
// and ready for data from the next
//
// Data is two 16 bit signed values per 32 bit register
// First 16 is for one cavity, second for another cavity
// digDspMuxShell0DspChanKeep digDspMuxShell1DspChanKeep, 12 bits to activate I or Q for 6 channels
// Each of the above registers looks like:
// msb _________________________________________________________________________________________ lsb
// | Ch5 Q | Ch5 I | Ch4 Q | Ch4 I | Ch3 Q | Ch3 I | Ch2 Q | Ch2 I | Ch1 Q | Ch1 I | Ch0 Q | Ch0 I |
// -------------------------------------------------------------------------------------------------
// Data is interleaved, based on the number of active channels. For example, if C0 has Ch4 I/Q enabled,
// and C1 has Ch5 and 4 I/Q enabled, it would be packed like:
// | High 16 bits | Low 16 bits |
// |   C1 Ch4 I   |   C0 Ch4 I  |
// |   C1 Ch4 Q   |   C0 Ch4 Q  |
// |   C1 Ch5 I   |   C0 Ch4 I  |
// |   C1 Ch5 Q   |   C0 Ch4 Q  |
// |   C1 Ch4 I   |   C0 Ch4 I  |
// |   C1 Ch4 Q   |   C0 Ch4 Q  |
// .....
//
static const uint32_t circIQBufStart = 0x170000; // From FPGA design, base address
static const uint32_t circIQBufEnd = 0x171fff; // max possible
static const unsigned circIQBufWaveRegCount = circIQBufEnd - circIQBufStart + 1;
static const unsigned circIQBufWavePoints = circIQBufWaveRegCount/2; // # data points for I or Q. Half the registers are I data, half Q
static const unsigned maxCircIQBufWavesCount = 12; // max channels, max number of waveforms interlaced in waveform buffer
static const unsigned circIQBufSegmentCount = (circIQBufWaveRegCount + maxRegPerMsg -1)/maxRegPerMsg; // # of UDP requests, divide and round up
static const unsigned circIQBufReqMsgSize = circIQBufWaveRegCount + circIQBufSegmentCount; // All register addresses plus nonce space
static const unsigned slowDataBuffRegCount = 128; // 128 registers
static const unsigned slowDataBuffSize = slowDataBuffRegCount + nonceSize; // 128 registers and the nonce

static const double LOfrequency = 1.320e9;
static const double ADCfrequency = LOfrequency/14;

/* Registers */

// I and Q waveforms
static const char *WaveformI16BitString = "WAVEFORM_I_16_BIT";
static const char *WaveformQ16BitString = "WAVEFORM_Q_16_BIT";
static const char *WaveformI22BitString = "WAVEFORM_I_22_BIT";
static const char *WaveformQ22BitString = "WAVEFORM_Q_22_BIT";

// Amplitude and Phase waveforms
static const char *WaveformA16BitString = "WAVEFORM_A_16_BIT";
static const char *WaveformP16BitString = "WAVEFORM_P_16_BIT";
static const char *WaveformA22BitString = "WAVEFORM_A_22_BIT";
static const char *WaveformP22BitString = "WAVEFORM_P_22_BIT";

// Binary 0 for 22 bit data, 1 for 16 bit data
static const char *IQBitWidthString = "I_Q_BIT_WIDTH";
// Number of active waveforms, from 0 to 8
static const char *IQNActiveString = "I_Q_N_ACTIVE";
// A buffer holds all active waveforms while remaining the same size.
// NELM is the size of each active waveform, so NELM * N_ACTIVE = total bufffer size
static const char *IQ16BitNELMString = "I_Q_16BIT_NELM";
static const char *IQ22BitNELMString = "I_Q_22BIT_NELM";

// Circle buffer I/Q data
static const char *Circ0NActiveString = "CIRC_0_N_ACTIVE";
static const char *Circ1NActiveString = "CIRC_1_N_ACTIVE";
static const char *CircIQBuf0IString = "CIRC_IQ_BUF_0_I";
static const char *CircIQBuf0QString = "CIRC_IQ_BUF_0_Q";
static const char *CircIQBuf0AString = "CIRC_IQ_BUF_0_A";
static const char *CircIQBuf0PString = "CIRC_IQ_BUF_0_P";
static const char *CircIQBuf1IString = "CIRC_IQ_BUF_1_I";
static const char *CircIQBuf1QString = "CIRC_IQ_BUF_1_Q";
static const char *CircIQBuf1AString = "CIRC_IQ_BUF_1_A";
static const char *CircIQBuf1PString = "CIRC_IQ_BUF_1_P";

static const char *Shell0CircleCountRString = "SHELL_0_CIRCLE_COUNT_R";
static const char *Shell0CircleStatRString = "SHELL_0_CIRCLE_STAT_R";
static const char *Shell0MmRString = "SHELL_0_MM_R";
static const char *Shell0TagNowRString = "SHELL_0_TAG_NOW_R";
static const char *Shell0TagOldRString = "SHELL_0_TAG_OLD_R";
static const char *Shell0TimeStampHighRString = "SHELL_0_TIME_STAMP_HIGH_R";
static const char *Shell0TimeStampLowRString = "SHELL_0_TIME_STAMP_LOW_R";

static const char *Shell1CircleCountRString = "SHELL_1_CIRCLE_COUNT_R";
static const char *Shell1CircleStatRString = "SHELL_1_CIRCLE_STAT_R";
static const char *Shell1MmRString = "SHELL_1_MM_R";
static const char *Shell1TagNowRString = "SHELL_1_TAG_NOW_R";
static const char *Shell1TagOldRString = "SHELL_1_TAG_OLD_R";
static const char *Shell1TimeStampHighRString = "SHELL_1_TIME_STAMP_HIGH_R";
static const char *Shell1TimeStampLowRString = "SHELL_1_TIME_STAMP_LOW_R";


class scllrfPRCextra: public scllrfPRCDriver
{
public:
	scllrfPRCextra(const char *drvPortName, const char *netPortName);
	virtual ~scllrfPRCextra();
	virtual asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);

	enum traceIQWavBitWidth { read22bit, read16bit };
	void traceIQWaveformRequester(); // When signaled that waveforms are waiting, request them.
	void circIQBufRequester(); // When signaled that waveforms are waiting, request them.

protected:
	virtual asynStatus processRegReadback(const FpgaReg *pFromFpga,
			bool &waveIsReady); // parse register data, write to PVs
	virtual asynStatus processRegWriteResponse(const FpgaReg *pFromFpga);

	//// Bigger waveforms I/Q data
	virtual asynStatus startTraceIQWaveformRequester(); // For system startup
	traceIQWavBitWidth wavBitWidth_;
	FpgaReg pReqIQ16bAMsg_[traceIQWaveSegmentCount][traceIQWaveSegmentSize]; // Canned message to request 16 bit I/Q data, first npt_ points
	FpgaReg pReqIQ16bBMsg_[traceIQWaveSegmentCount][traceIQWaveSegmentSize]; // Canned message to request 16 bit I/Q data, last npt_ points
	FpgaReg pReqI22bMsg_[traceIQWaveSegmentCount][traceIQWaveSegmentSize]; // Canned message to request 22 bit I data
	FpgaReg pReqQ22bMsg_[traceIQWaveSegmentCount][traceIQWaveSegmentSize]; // Canned message to request 22 bit Q data
	size_t npt_; /**< Number of points in each waveform buffer */
	unsigned int nchan_;
	void fillTraceIQWavReqMsg();
	void reqTraceIQWWaveform(FpgaReg (*readTraceIQWaveformsMsg)[traceIQWaveSegmentSize]);
	std::ostringstream strGitSHA1;
	epicsInt16 pWave16bitI_[maxTraceIQWavesCount][traceIQWaveBuffSize *2];
	epicsInt16 pWave16bitQ_[maxTraceIQWavesCount][traceIQWaveBuffSize *2];
	epicsFloat32 pWave16bitA_[maxTraceIQWavesCount][traceIQWaveBuffSize *2];
	epicsFloat32 pWave16bitP_[maxTraceIQWavesCount][traceIQWaveBuffSize *2];
	epicsInt32 pWave22bitI_[maxTraceIQWavesCount][traceIQWaveBuffSize];
	epicsInt32 pWave22bitQ_[maxTraceIQWavesCount][traceIQWaveBuffSize];
	epicsFloat32 pWave22bitA_[maxTraceIQWavesCount][traceIQWaveBuffSize];
	epicsFloat32 pWave22bitP_[maxTraceIQWavesCount][traceIQWaveBuffSize];

	virtual asynStatus processTraceIQWaveReadback(const FpgaReg *pFromFpga); // parse register data, write to array PV
//	virtual asynStatus catGitSHA1(); // Once the individual bytes are all read into registers, concatenate them into a string

	// Smaller waveforms I/Q data
	virtual asynStatus startCircIQBufRequester(); // For system startup
	FpgaReg pReqCircIQBufMsg_[circIQBufReqMsgSize]; // Canned message to request data buffer
	FpgaReg pReqSlowBuf0Msg_[slowDataBuffSize]; // Canned message to request slow data buffer 0
	FpgaReg pReqSlowBuf1Msg_[slowDataBuffSize]; // Canned message to request slow data buffer 1
	unsigned int nCirc0Chan_;
	unsigned int nCirc1Chan_;
	void fillCircIQBufReqMsg();
	void reqCircIQBuf();
	epicsEventId reqCircIQBufEventId_; /**< Event ID to signal the waveform requester */
	unsigned int newCircIQBufAvailable_; /**< netSendCount value of the latest response with the "new waveform" flag set */
	unsigned int newCircIQBufRead_; /**< netSendCount for the most recent waveform */
	unsigned int phaseStepH, phaseStepL, phaseModulo;
	double iFrequency;

	epicsInt32 pCircIQBuf_[circIQBufWaveRegCount];
	epicsInt16 pCircIQBuf0I_[maxCircIQBufWavesCount][circIQBufWavePoints];
	epicsInt16 pCircIQBuf0Q_[maxCircIQBufWavesCount][circIQBufWavePoints];
	epicsInt16 pCircIQBuf1I_[maxCircIQBufWavesCount][circIQBufWavePoints];
	epicsInt16 pCircIQBuf1Q_[maxCircIQBufWavesCount][circIQBufWavePoints];
	epicsFloat32 pCircIQBuf0A_[maxCircIQBufWavesCount][circIQBufWavePoints];
	epicsFloat32 pCircIQBuf0P_[maxCircIQBufWavesCount][circIQBufWavePoints];
	epicsFloat32 pCircIQBuf1A_[maxCircIQBufWavesCount][circIQBufWavePoints];
	epicsFloat32 pCircIQBuf1P_[maxCircIQBufWavesCount][circIQBufWavePoints];

	virtual asynStatus processCircIQBufReadback(const FpgaReg *pFromFpga); // parse register data, write to array PV


	/** Values used for pasynUser->reason, and indexes into the parameter library.
	 * For this prototype, it's read only values that identify the FPGA. */

    // parameters for reading I/Q waveforms
    int p_IQBitWidth;
    #define FIRST_SCLLRFPRCEXTRA_PARAM p_IQBitWidth
    int p_IQNActive;
    int p_IQ16BitNELM;
    int p_IQ22BitNELM;

    // I/Q waveform data
    int p_WaveformI16Bit;
    int p_WaveformQ16Bit;
    int p_WaveformI22Bit;
    int p_WaveformQ22Bit;

    // Amplitude/Phase waveform data
    int p_WaveformA16Bit;
    int p_WaveformP16Bit;
    int p_WaveformA22Bit;
    int p_WaveformP22Bit;

    // Circle buffer I/Q data
    int p_Circ0NActive;
    int p_Circ1NActive;
    int p_CircIQBuf0I;
    int p_CircIQBuf0Q;
    int p_CircIQBuf0A;
    int p_CircIQBuf0P;
    int p_CircIQBuf1I;
    int p_CircIQBuf1Q;
    int p_CircIQBuf1A;
    int p_CircIQBuf1P;

    int p_Shell0CircleCountR;
    int p_Shell0CircleStatR;
    int p_Shell0MmR;
    int p_Shell0TagNowR;
    int p_Shell0TagOldR;
    int p_Shell0TimeStampHighR;
    int p_Shell0TimeStampLowR;

    int p_Shell1CircleCountR;
    int p_Shell1CircleStatR;
    int p_Shell1MmR;
    int p_Shell1TagNowR;
    int p_Shell1TagOldR;
    int p_Shell1TimeStampHighR;
    int p_Shell1TimeStampLowR;

    // Flag to indicate new waveform data is available
    int p_WvformsAvailableR;
    #define LAST_SCLLRFPRCEXTRA_PARAM p_WvformsAvailableR

#define NUM_SCLLRFPRCEXTRA_PARAMS (&LAST_SCLLRFPRCEXTRA_PARAM - &FIRST_SCLLRFPRCEXTRA_PARAM + 1)

private:

    // masks applied to returned register data
    enum ExtraRegMasks
    {
    	TraceKeepRMask = 0x000000FF,
    	TraceKeepWMask = 0x000000FF,
		waveIsReadyMask = 0x40000000, // for trace status reg
		nptMask = 0x3F000000 // for trace status reg
    };
};

#endif

