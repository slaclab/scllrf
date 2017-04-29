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

#ifndef SCLLRFAPP_SRC_SCLLRFASYNPORTDRIVER_H_
#define SCLLRFAPP_SRC_SCLLRFASYNPORTDRIVER_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <vector>
//#include <atomic>
#include <string>
#include <sstream>
#include <iomanip>


#include <epicsTypes.h>
#include <epicsThread.h>
#include <epicsEvent.h>
#include <iocsh.h>
#include <epicsMessageQueue.h>

#include <asynPortDriver.h>

#include <epicsExport.h>

// Move these to a common file somewhere

// First byte of data is for control (read/write flag)
// Next 3 bytes are the address
// last 32 bits are data
typedef struct
{
	uint32_t addr;
	int32_t data;
} FpgaReg;

static const unsigned int nonceSize = 1; // nonce is one FpgaReg big

// Functions to convert arrays of registers in place.
/**
 * Convert an array of FpgaReg elements to network byte order in place
 * @param buffer Array of FpgaReg elements to be sent over the network
 * @param regCount the dimension of the buffer, not the number of bytes
 * @see ntohFpgaRegArray
 * @return The test results
 */
void htonFpgaRegArray(FpgaReg *buffer, unsigned int regCount);
/**
 * Convert an array of FpgaReg elements from network byte order in place
 * @param buffer Array of FpgaReg elements received over the network
 * @param regCount the dimension of the buffer, not the number of bytes
 * @see htonFpgaRegArray
 * @return The test results
 */
void ntohFpgaRegArray(FpgaReg *buffer, unsigned int regCount);

// Set this bit in the control portion of address to request a reg read
static const uint32_t flagReadMask = 0x10000000;

// Mask for the lower 24 bits of the addr field
static const uint32_t addrMask = 0x00FFFFFF;

// Something easy to spot assigned to uninitialized data
static const int32_t blankData = 0xDEADBEEF;

static const unsigned maxMsgSize = 1400; // Estimated MTU minus fudge factor, in bytes
static const unsigned maxRegPerMsg = maxMsgSize/sizeof(FpgaReg)-1; // Number of register requests minus the nonce
static const unsigned minRegPerMsg = 5; // Limitation of UDP and what the FPGA's limited network stack can cope with

// Communication tuning parameters
static const double readTimeout = 1.0; // seconds
static const double writeTimeout = 5.0; // seconds
static const double defaultPollPeriod = 0.1; // seconds
static const double throttleLoopDelay = 0.001; // seconds, delay when sending is outstripping reading
static const unsigned int defaultMaxParallelRequests = 1; // throttle requests when this many are outstanding


/* Registers */


class scllrfAsynPortDriver: public asynPortDriver
{
public:
	scllrfAsynPortDriver(const char *drvPortName, const char *netPortName, int maxAddr=0, int paramTableSize=7);
	virtual ~scllrfAsynPortDriver();
	virtual asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);
	virtual asynStatus readInt32(asynUser *pasynUser, epicsInt32 *value);
	virtual asynStatus writeInt32Array(asynUser *pasynUser, epicsInt32 *value,
			size_t nElements);
//	virtual asynStatus readInt32Array(asynUser *pasynUser, epicsInt32 *value,
//			size_t nElements, size_t *nIn);
	virtual asynStatus writeUInt32Digital(asynUser *pasynUser, epicsUInt32 value, epicsUInt32 mask);

	/* Define a polling mechanism.
	 * Sends requests for all register data at a specified period.
	 * A separate thread processes the response.
	 */
	void regPoller(); // Polling loop, exits on shutdown
	void regSettingsInit(); // canned settings from LBL
	virtual asynStatus wakeupPoller(); // Alert poller to request new data
	virtual asynStatus sendRegRequest(FpgaReg *regBuffer,
			unsigned int regBuffSize); // Send out canned request for all registers

	virtual void responseHandler(); // Readback loop, processes data from FPGAs when it arrives
	virtual asynStatus wakeupReader(); // Alert responseHandler to expect new data
	virtual void init();
	void singleMessageQueuer(); // Accumulates individual requests until they can be sent together.

protected:

	// EPICS database driver strings
	static const char *RunStopString; /* asynInt32,    r/w */
	static const char *ReadOneRegString; /* asynInt32 array[2] w */
	static const char *WriteOneRegString; /* asynInt32 array[2] w */
	static const char *MaxParallelRequestsString; /* asynInt32,    r/w */
	static const char *PollPeriodString; /* asynInt32,    r/w */
	static const char *CommErrorCountString;  /* asynInt32,    r */

	// For readable registers that are polled together at the set polling rate
	FpgaReg *pPolledRegMsg_; // Leave the first array element blank, for use as the nonce
	size_t PolledRegMsgSize_; // number of registers plus one for the nonce
	virtual asynStatus startPoller(double pollPeriod); // For system startup

	// Response handler, reads data sent back from FPGA and assigns it to
	// the appropriate variables and pvs.
	virtual asynStatus startResponseHandler(); // For system startup
	asynStatus processReadbackBuffer(FpgaReg *pFromFpga,
			unsigned int readCount);
	virtual asynStatus processRegWriteResponse(const FpgaReg *pFromFpga);
	virtual asynStatus processRegReadback(const FpgaReg *pFromFpga,
			bool &waveIsReady); // parse register data, write to PVs
	virtual asynStatus functionToRegister(const int function, FpgaReg *pToFpga); /**< Translate asyn function number/reason to a register address */

	virtual asynStatus startSingleMessageQueuer();
	epicsEventId singleMsgQueueEventId_; /**< Event ID to signal the write message queuer */
	epicsMessageQueue _singleMsgQ;

	void fillWaveRequestMsg(FpgaReg pMsgBuff[], const size_t buffSize, const unsigned int iStartAddr); /**< For requesting a waveform, fill canned message request with sequential addresses */

	epicsEventId pollEventId_; /**< Event ID to wake up poller */
	double pollPeriod_; /**< The time between polls */
	epicsEventId readEventId_; /**< Event ID to wake up network message response reader */
	double readTimeout_; /**< The time between polls */
	epicsEventId reqWaveEventId_; /**< Event ID to signal the waveform requester */
	bool isShuttingDown_; /**< Flag to indicate threads should exit */
	asynUser* pCommonAsynUser_; /**< asynUser for asynCommonSyncIO */
	asynUser* pOctetAsynUser_; /**< asynUser for asynOctetSyncIO */
	epicsMutexId comCountersMutexId_;
	unsigned int netSendCount_; /**< Number of messages sent to FPGA, sent in nonce for error checking */
	unsigned int lastResponseCount_; /**< Used by response processing to check for missing or out of order responses */
//  std::atomic<uint> netWaitingRequests; // writers increment, reader decrements
	unsigned int netWaitingRequests_; /**< writers increment, reader decrements */
	unsigned int newWaveAvailable_; /**< netSendCount value of the latest response with the "new waveform" flag set */
	unsigned int newWaveRead_; /**< netSendCount for the most recent waveform */
	enum
	{
		stop, run
	};

	/** Values used for pasynUser->reason, and indexes into the parameter library.
	 * For this prototype, it's read only values that identify the FPGA. */
	int p_RunStop;
#define FIRST_SCLLRF_PARAM p_RunStop
	int p_ReadOneReg;
	int p_WriteOneReg;
	int p_MaxParallelRequests;
	int p_PollPeriod;
	int p_CommErrorCount;

/* Registers */
    #define LAST_SCLLRF_PARAM p_CommErrorCount

	epicsUInt32 uReadOneRegAddr, uWriteOneRegAddr;

	//Note: the number of parameters must be manually set in the iocsh binding,
	// the addresses of the parameters used in the #define are out of scope there.
#define NUM_SCLLRF_PARAMS (&LAST_SCLLRF_PARAM - &FIRST_SCLLRF_PARAM + 1)

	// mapping of register names to addresses

    // mapping of register names to addresses

    // mapping of register names to addresses

    // masks applied to returned register data
};

#endif /* SCLLRFAPP_SRC_SCLLRFASYNPORTDRIVER_H_ */
