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


// Waveform data is packed as 2D arrays, not one after the other
// in the network data. See cmoc code "submodules/common_hdl/circle.txt" for more details.
// Summary: (array 1, data 1 and 2), (array 2, data 1 and 2), (array 3, data 1 and 2)....
// Addressing: I/Q waveforms base address = 0x140000
	// additional base offsets, calculated from npt = 1 << ((trace_status >> 24) & 0x3F)
	//   0 * npt  16-bit I and Q
	//   1 * npt  16-bit I and Q
	//   2 * npt  22-bit I
	//   3 * npt  22-bit Q
static const uint32_t wavesStart = 0x140000; // From FPGA design, base address
static const uint32_t wavesEnd = 0x14ffff; // max possible
static const unsigned waveBufferRegCount = wavesEnd - wavesStart + 1;
static const unsigned maxWavesCount = 8; // max channels, max number of waveforms interlaced in waveform buffer
static const unsigned waveBuffSize = waveBufferRegCount/4 + waveBufferRegCount/(maxMsgSize-sizeof(FpgaReg)); // divide by number of buffers
/**< Waveform buffer is read in waveSegmentCount segments due to network packet size limits.
 *  1+ is an approximation of "round upwards". -sizeof(FpgaReg) makes space for the nonce */
static const unsigned waveSegmentCount = 1 + (sizeof(FpgaReg) * waveBuffSize)
				/ (maxMsgSize - sizeof(FpgaReg));

/**< Size of each segment, in number of registers */
static const unsigned waveSegmentSize = (maxMsgSize/sizeof(FpgaReg));
//		+ (waveBuffSize + waveSegmentCount) / waveSegmentCount;

/* Registers */

// I and Q waveforms
static const char *WaveformI16BitString = "WAVEFORM_I_16_BIT";
static const char *WaveformQ16BitString = "WAVEFORM_Q_16_BIT";
static const char *WaveformI22BitString = "WAVEFORM_I_22_BIT";
static const char *WaveformQ22BitString = "WAVEFORM_Q_22_BIT";
// Binary 0 for 22 bit data, 1 for 16 bit data
static const char *IQBitWidthString = "I_Q_BIT_WIDTH";
// Number of active waveforms, from 0 to 8
static const char *IQNActiveString = "I_Q_N_ACTIVE";
// A buffer holds all active waveforms while remaining the same size.
// NELM is the size of each active waveform, so NELM * N_ACTIVE = total bufffer size
static const char *IQ16BitNELMString = "I_Q_16BIT_NELM";
static const char *IQ22BitNELMString = "I_Q_22BIT_NELM";


class scllrfPRCextra: public scllrfPRCDriver
{
public:
	scllrfPRCextra(const char *drvPortName, const char *netPortName);
	virtual ~scllrfPRCextra();
	asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);
	asynStatus writeInt32Array(asynUser *pasynUser, epicsInt32 *value,
			size_t nElements);

	enum WavBitWidth { read22bit, read16bit };
	void waveformRequester(); // When signaled that waveforms are waiting, request them.

protected:
	virtual asynStatus processRegReadback(const FpgaReg *pFromFpga,
			bool &waveIsReady); // parse register data, write to PVs
	virtual asynStatus processRegWriteResponse(const FpgaReg *pFromFpga);

	virtual asynStatus startWaveformRequester(); // For system startup
	WavBitWidth wavBitWidth_;
	FpgaReg pReqIQ16bAMsg_[waveSegmentCount][waveSegmentSize]; // Canned message to request 16 bit I/Q data, first npt_ points
	FpgaReg pReqIQ16bBMsg_[waveSegmentCount][waveSegmentSize]; // Canned message to request 16 bit I/Q data, last npt_ points
	FpgaReg pReqI22bMsg_[waveSegmentCount][waveSegmentSize]; // Canned message to request 22 bit I data
	FpgaReg pReqQ22bMsg_[waveSegmentCount][waveSegmentSize]; // Canned message to request 22 bit Q data
	size_t npt_; /**< Number of points in each waveform buffer */
	unsigned int nchan_;
	void fillWavReqMsg();
	void reqOneWaveform(FpgaReg (*readWaveformsMsg)[waveSegmentSize]);
	//	std::ostringstream strGitSHA1;
	epicsInt16 pWave16bitI_[maxWavesCount][waveBuffSize *2];
	epicsInt16 pWave16bitQ_[maxWavesCount][waveBuffSize *2];
	epicsInt32 pWave22bitI_[maxWavesCount][waveBuffSize];
	epicsInt32 pWave22bitQ_[maxWavesCount][waveBuffSize];

	virtual asynStatus processWaveReadback(const FpgaReg *pFromFpga); // parse register data, write to array PV
//	virtual asynStatus catGitSHA1(); // Once the individual bytes are all read into registers, concatenate them into a string


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

