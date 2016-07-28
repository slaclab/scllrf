/*
 * scllrfAsynPortDriver.cpp
 *
 *  Created on: Jun 17, 2016
 *      Author: gwbrown
 */

#include "scllrfAsynPortDriver.h"
#include <asynOctetSyncIO.h>
#include <asynCommonSyncIO.h>
#include <limits>
#include <netinet/in.h>
#include <iostream>
using namespace std;
#include <math.h>

#define NUM_CMOC_PARAMS (&LAST_CMOC_COMMAND - &FIRST_CMOC_COMMAND + 1)
/** Constructor for the scllrfAsynPortDriver class.
 * Calls constructor for the asynPortDriver base class.
 * \param[in] portName The name of the asyn port driver to be created.
 * \param[in] path The path to the peripherial as built by the builder api
 * \param[in] nelms The number of elements of this device (max addr)
 * \paarm[in] nEntries The number of asyn params to be created for each device
 *
 * */
scllrfAsynPortDriver::scllrfAsynPortDriver(const char *drvPortName, const char *netPortName)
: asynPortDriver(drvPortName,
		wavesCount, /* maxAddr */
		NUM_CMOC_PARAMS,
		asynInt32Mask | asynFloat64Mask | asynOctetMask | asynDrvUserMask | asynInt32ArrayMask|asynUInt32DigitalMask, /* Interface mask */
		asynInt32Mask | asynFloat64Mask | asynOctetMask | asynEnumMask | asynInt32ArrayMask,  /* Interrupt mask */
		1, /* asynFlags.  This driver does block and it is not multi-device, so flag is 1 */
		1, /* Autoconnect */
		epicsThreadPriorityMedium,
		0), /* Default stack size*/
		isShuttingDown_(0), netSendCount_(0), netWaitingRequests_(0), newWaveAvailable_(0), newWaveRead_ (0)
{
	asynStatus status = asynSuccess;

	createParam(P_MagicString, asynParamInt32, &P_Magic);
	createParam(P_DspFlavorString, asynParamInt32, &P_DspFlavor);
	createParam(P_BuildYearString, asynParamInt32, &P_BuildYear);
	createParam(P_BuildMonthString, asynParamInt32, &P_BuildMonth);
	createParam(P_BuildDayString, asynParamInt32, &P_BuildDay);
	createParam(P_BuildHourString, asynParamInt32, &P_BuildHour);
	createParam(P_BuildMinuteString, asynParamInt32, &P_BuildMinute);
	createParam(P_CodeIsCleanString, asynParamInt32, &P_CodeIsClean);
	createParam(P_ToolRevString, asynParamInt32, &P_ToolRev);
	createParam(P_UserString, asynParamInt32, &P_User);
	createParam(P_BoardTypeString, asynParamInt32, &P_BoardType);
	createParam(P_VersionString, asynParamInt32, &P_Version);
	createParam(P_GitSHA1aString, asynParamInt32, &P_GitSHA1a);
	createParam(P_GitSHA1bString, asynParamInt32, &P_GitSHA1b);
	createParam(P_GitSHA1cString, asynParamInt32, &P_GitSHA1c);
	createParam(P_GitSHA1dString, asynParamInt32, &P_GitSHA1d);
	createParam(P_GitSHA1eString, asynParamInt32, &P_GitSHA1e);
	createParam(P_GitSHA1fString, asynParamInt32, &P_GitSHA1f);
	createParam(P_GitSHA1gString, asynParamInt32, &P_GitSHA1g);
	createParam(P_GitSHA1hString, asynParamInt32, &P_GitSHA1h);
	createParam(P_GitSHA1iString, asynParamInt32, &P_GitSHA1i);
	createParam(P_GitSHA1jString, asynParamInt32, &P_GitSHA1j);
	createParam(P_GitSHA1kString, asynParamInt32, &P_GitSHA1k);
	createParam(P_GitSHA1lString, asynParamInt32, &P_GitSHA1l);
	createParam(P_GitSHA1mString, asynParamInt32, &P_GitSHA1m);
	createParam(P_GitSHA1nString, asynParamInt32, &P_GitSHA1n);
	createParam(P_GitSHA1oString, asynParamInt32, &P_GitSHA1o);
	createParam(P_GitSHA1pString, asynParamInt32, &P_GitSHA1p);
	createParam(P_GitSHA1qString, asynParamInt32, &P_GitSHA1q);
	createParam(P_GitSHA1rString, asynParamInt32, &P_GitSHA1r);
	createParam(P_GitSHA1sString, asynParamInt32, &P_GitSHA1s);
	createParam(P_GitSHA1tString, asynParamInt32, &P_GitSHA1t);
    createParam(P_Waveform1String,asynParamInt32Array, &P_Waveform);

	status=pasynCommonSyncIO->connect(netPortName, 0, &pCommonAsynUser_, 0);
	if(status!=asynSuccess)
		printf( "%s: connect: failed to connect to port %s with status %d\n",
				__PRETTY_FUNCTION__,netPortName, status);
	else  printf( "%s: connect: connected to port %s\n",__PRETTY_FUNCTION__,netPortName);

	status=pasynOctetSyncIO->connect( netPortName,0,&pOctetAsynUser_,0);
	if(status!=asynSuccess)
		printf( "%s: connect: failed to connect to Read port %s with status %d\n",
				__PRETTY_FUNCTION__,netPortName, status);
	else  printf( "%s: connect: connected to port %s\n",__PRETTY_FUNCTION__,netPortName);

	readEventId_ = epicsEventMustCreate(epicsEventEmpty);
	startResponseHandler();

	pollEventId_ = epicsEventMustCreate(epicsEventEmpty);
	startPoller(defaultPollPeriod);

	reqWaveEventId_ = epicsEventMustCreate(epicsEventEmpty);
	startWaveformRequester();

}

scllrfAsynPortDriver::~scllrfAsynPortDriver()
{
	isShuttingDown_ = true;
	epicsThreadSleep(0.1); // Allow threads to run and exit
	epicsEventSignal(reqWaveEventId_);
	wakeupPoller();
	wakeupReader();
	epicsThreadSleep(0.1); // Allow threads to run and exit
	pasynOctetSyncIO->disconnect(pOctetAsynUser_);
	pasynCommonSyncIO->disconnectDevice(pCommonAsynUser_);
	pasynCommonSyncIO->disconnect(pCommonAsynUser_);
}


/** Called when asyn clients call pasynInt32->read().
 * \param[in] pasynUser pasynUser structure that encodes the reason and address.
 * \param[in] value Pointer to the value to read. */
asynStatus scllrfAsynPortDriver::readInt32(asynUser *pasynUser, epicsInt32 *value)
{
	//    int function = pasynUser->reason;
	//    int addr = 0;
	asynStatus status = asynSuccess;
	epicsTimeStamp timeStamp; getTimeStamp(&timeStamp);
	//    const char *functionName = "readInt32";
	//    const char *paramName;





	/* Do callbacks so higher layers see any changes */
	status = (asynStatus) callParamCallbacks();

	return status;
}

static void regPollerC(void *drvPvt)
{
	scllrfAsynPortDriver *pscllrfDriver = (scllrfAsynPortDriver*)drvPvt;
	pscllrfDriver->regPoller();
	printf("%s: exiting\n", __PRETTY_FUNCTION__);
}

/** Starts the poller thread.
 ** Derived classes will typically call this at near the end of their constructor.
 ** Derived classes can typically use the base class implementation of the poller thread,
 ** but are free to re-implement it if necessary.
 ** \param[in] pollPeriod The time between polls. */
asynStatus scllrfAsynPortDriver::startPoller(double pollPeriod)
{
	pollPeriod_ = pollPeriod;
	epicsThreadCreate("asynPoller",
			epicsThreadPriorityMedium,
			epicsThreadGetStackSize(epicsThreadStackMedium),
			(EPICSTHREADFUNC)regPollerC, (void *)this);
	return asynSuccess;
}

void scllrfAsynPortDriver::regPoller()
{
	epicsEventWaitStatus status;

	// A canned request to read all registers
	static FpgaReg cmocReadAllRegMsg[regCount + 1] =
	{
			{ 0, 0 },
			{ flagReadMask | 0x00, blankData | (int32_t) wavesReadyMask },
			{ flagReadMask | 0x01, blankData },
			{ flagReadMask | 0x02, blankData },
			{ flagReadMask | 0x03, blankData },
			{ flagReadMask | 0x04, blankData },
			{ flagReadMask | 0x05, blankData },
			{ flagReadMask | 0x06, blankData },
			{ flagReadMask | 0x07, blankData },
			{ flagReadMask | 0x08, blankData },
			{ flagReadMask | 0x09, blankData },
			{ flagReadMask | 0x0a, blankData },
			{ flagReadMask | 0x0b, blankData },
			{ flagReadMask | 0x0c, blankData },
			{ flagReadMask | 0x0d, blankData },
			{ flagReadMask | 0x0e, blankData },
			{ flagReadMask | 0x0f, blankData },
			{ flagReadMask | 0x10, blankData },
			{ flagReadMask | 0x11, blankData },
			{ flagReadMask | 0x12, blankData },
			{ flagReadMask | 0x13, blankData },
			{ flagReadMask | 0x14, blankData },
			{ flagReadMask | 0x15, blankData },
			{ flagReadMask | 0x16, blankData },
			{ flagReadMask | 0x17, blankData },
			{ flagReadMask | 0x18, blankData },
			{ flagReadMask | 0x19, blankData },
			{ flagReadMask | 0x1a, blankData },
			{ flagReadMask | 0x1b, blankData },
			{ flagReadMask | 0x1c, blankData },
			{ flagReadMask | 0x1d, blankData },
			{ flagReadMask | 0x1e, blankData },
			{ flagReadMask | 0x1f, blankData } };
	htonFpgaRegArray(cmocReadAllRegMsg, regCount + 1);

	wakeupPoller();  /* Force on poll at startup */

	while(1) {
		if (pollPeriod_ != 0.0) status = epicsEventWaitWithTimeout(pollEventId_, pollPeriod_);
		else               status = epicsEventWait(pollEventId_);
		if (status == epicsEventWaitOK) {
			/* We got an event, rather than a timeout.  This is because other software
			 ** knows that we should do a poll.
			 **/
		}
		if (isShuttingDown_) {
			break;
		}
		sendRegRequest(cmocReadAllRegMsg, regCount + 1);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: woke up and sent a poll\n", __PRETTY_FUNCTION__);
	}
	printf("%s: exiting\n", __PRETTY_FUNCTION__);
}

/** Wakes up the poller thread to make it start polling. */
asynStatus scllrfAsynPortDriver::wakeupPoller()
{
	epicsEventSignal(pollEventId_);
	return asynSuccess;
}

/** Polls the device.
 ** Note that the first "register" regBuffer[0] is really the nonce, not data.
 ** This function will set the nonce before sending.
 ** regBuffCount is the number of FpgaReg type elements, including the nonce
 **  */
asynStatus scllrfAsynPortDriver::sendRegRequest(FpgaReg *regBuffer, unsigned int regBuffCount)
{
	size_t writtenCount;
	asynStatus status = asynSuccess;
	char * pWriteBuffer;
	pWriteBuffer = reinterpret_cast<char*>(regBuffer);

	// Throttle so that we don't overflow buffers if response handling falls behind
	if( netWaitingRequests_ > throttlePoint )
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: too many requests waiting for responses, throttling requests.\n",__PRETTY_FUNCTION__);
	while( netWaitingRequests_ > throttlePoint )
		epicsThreadSleep(throttleLoopDelay);

	lock(); // protect netSendCount and netWaitingRequests
	netSendCount_++; // increment and roll over if needed
	// use the nonce at the start of the buffer for register count, and
	// send counter. Can be used for error checking.
	regBuffer[0] = (FpgaReg)
							{ (uint32_t) htonl(netSendCount_), (int32_t) htonl(regBuffCount*sizeof(FpgaReg)) };

	status = pasynOctetSyncIO->write(pOctetAsynUser_, pWriteBuffer,
			regBuffCount*sizeof(FpgaReg), writeTimeout, &writtenCount);
	if (status != asynSuccess)
		printf("%s: failed to write. %s\n", __PRETTY_FUNCTION__,
				pOctetAsynUser_->errorMessage);
	else
	{
		netWaitingRequests_++;
		//printf("%s: wrote %u registers. %u outstanding requests\n", __PRETTY_FUNCTION__,regBuffCount,
		//		netWaitingRequests);
	}
	unlock();
	wakeupReader();

	return asynSuccess;
}



static void waveformRequesterC(void *drvPvt)
{
	printf("%s: starting\n", __PRETTY_FUNCTION__);
	scllrfAsynPortDriver *pscllrfDriver = (scllrfAsynPortDriver*)drvPvt;
	pscllrfDriver->waveformRequester();
	printf("%s: exiting\n", __PRETTY_FUNCTION__);
}

/** Starts the poller thread.
 ** Derived classes will typically call this at near the end of their constructor.
 ** Derived classes can typically use the base class implementation of the poller thread,
 ** but are free to re-implement it if necessary.
 ** \param[in] pollPeriod The time between polls. */
asynStatus scllrfAsynPortDriver::startWaveformRequester()
{
	epicsThreadCreate("waveformRequester",
			epicsThreadPriorityMedium,
			epicsThreadGetStackSize(epicsThreadStackMedium),
			(EPICSTHREADFUNC)waveformRequesterC, (void *)this);
	return asynSuccess;
}

void scllrfAsynPortDriver::waveformRequester()
{
	epicsEventWaitStatus status;
	//	unsigned int regNum;
	unsigned int regAddress;
	unsigned int i;
	unsigned int waveSegmentNumber;
	unsigned int waveSegmentOffset;
	unsigned int lastPointIndex=0; // The index of the last register in the last segment

	// A canned request to read all waveforms
	// Split up because the packet would be too big for UDP otherwise
	static FpgaReg cmocReadWaveformsMsg[waveSegmentCount][waveSegmentSize];

	for (i=0; i<waveSegmentCount; i++)
	{
		cmocReadWaveformsMsg[i][0] = (FpgaReg) {0, 0}; // space for the nonce
	}

	for (waveSegmentNumber=0; waveSegmentNumber < waveSegmentCount; waveSegmentNumber++)
	{
		for (waveSegmentOffset = 0; waveSegmentOffset < waveSegmentSize-1; waveSegmentOffset++)
		{
			regAddress = wavesStart + waveSegmentNumber*(waveSegmentSize-1) + waveSegmentOffset;
			if (regAddress > wavesEnd)
			{
				lastPointIndex = waveSegmentOffset+1;
				break;
			}

			cmocReadWaveformsMsg[waveSegmentNumber][waveSegmentOffset+1] = (FpgaReg) { flagReadMask | regAddress, blankData};
		}
	}


	htonFpgaRegArray(cmocReadWaveformsMsg[0], sizeof(cmocReadWaveformsMsg)/sizeof(FpgaReg));

	// Main polling loop
	while (1)
	{
		status = epicsEventWait(reqWaveEventId_);

		if (isShuttingDown_)
		{
			break;
		}

		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: signaled by reqWaveEventId_\n", __PRETTY_FUNCTION__);

		if (status == epicsEventWaitOK)
		{
			/* We got an event, rather than a timeout.
			 **/
			for (i=0; i<waveSegmentCount - 1; i++)
			{
				sendRegRequest(cmocReadWaveformsMsg[i], sizeof(cmocReadWaveformsMsg[0])/sizeof(FpgaReg));
			}
			// The last segment probably has a different number of points in it
			sendRegRequest(cmocReadWaveformsMsg[waveSegmentCount-1], lastPointIndex);

			newWaveRead_ = newWaveAvailable_; // Indicate that we got the signal

			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
					"%s: done sending waveform request\n", __PRETTY_FUNCTION__);
		}
	}
	printf("%s: exiting\n", __PRETTY_FUNCTION__);
}


static void responseHandlerC(void *drvPvt)
{
	scllrfAsynPortDriver *pscllrfDriver = (scllrfAsynPortDriver*)drvPvt;
	pscllrfDriver->responseHandler();
	printf("%s: exiting\n", __PRETTY_FUNCTION__);
}

/** Starts the poller thread.
 ** Derived classes will typically call this at near the end of their constructor.
 ** Derived classes can typically use the base class implementation of the poller thread,
 ** but are free to re-implement it if necessary.
 ** \param[in] pollPeriod The time between polls. */
asynStatus scllrfAsynPortDriver::startResponseHandler()
{
	readTimeout_ = readTimeout;
	epicsThreadCreate("responseHandler",
			epicsThreadPriorityMedium, epicsThreadGetStackSize(epicsThreadStackMedium),
			(EPICSTHREADFUNC) responseHandlerC, (void *) this);
	return asynSuccess;
}

/** Wakes up the reader thread to check for data. */
asynStatus scllrfAsynPortDriver::wakeupReader()
{
	epicsEventSignal(readEventId_);
	return asynSuccess;
}

void scllrfAsynPortDriver::responseHandler()
{
	asynStatus status;
	epicsEventWaitStatus waitStatus;
	//	unsigned i;
	const size_t readBufferSize = waveSegmentCount * waveSegmentSize * sizeof(FpgaReg);
	static char pReadBuffer[readBufferSize];
	FpgaReg *pRegReadback;
	size_t readCount = 0;
	int eomReason;

	while (1)
	{

		// The asyn framework doesn't allow writes while a read is blocking,
		// so the same behavior is approximated here with events.
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: Caught up with message processing, blocking for data\n", __PRETTY_FUNCTION__);
		waitStatus = epicsEventWaitWithTimeout(readEventId_, readTimeout_);

		if (waitStatus == epicsEventWaitOK) // We got an event, signaling to expect data back
		{
			while (netWaitingRequests_>0) // Writers increment atomic netWaitingRequests when they send a request
			{
				// Note: pRegReadback/pReadBuffer would have reentrancy problems, except only this one
				// thread reads it. More than one read thread and it could break.
				pRegReadback = reinterpret_cast<FpgaReg*>(pReadBuffer);
				pRegReadback[0] = (FpgaReg) {0, 0}; // Reset count, so we won't try to process data that isn't there.

				lock(); // protect netWaitingRequests from being modified by the write thread
				status = pasynOctetSyncIO->read(pOctetAsynUser_, pReadBuffer,
						readBufferSize, 0.001, &readCount, &eomReason);
				//				printf("%s: status=0x%x, readCount=%zd, eomReason=0x%x\n", __PRETTY_FUNCTION__, status, readCount, eomReason);

				//				if (status == asynSuccess || readCount >0)
				//				{
				////					netWaitingRequests--;
				//					printf("%s: woke up and read data\n", __PRETTY_FUNCTION__);
				//				} // if (status == asynSuccess)
				//				else
				//				{
				//					printf("%s: woke up but has no data yet. Expecting %u messages\n", __PRETTY_FUNCTION__,netWaitingRequests);
				//				}

				unlock(); // protect netWaitingRequests from being modified by the write thread

				ntohFpgaRegArray(pRegReadback, readCount/sizeof(FpgaReg));

				// We put the message size in the data of the first buffer element for error checking
				// and to handle multiple messages received
				while(readCount > 0)
				{
					// We can read partial messages, so watch array bounds
					if (readCount < (size_t) pRegReadback[0].data)
					{
						pRegReadback[0].data = readCount;
						//pasynOctetSyncIO->flush(pOctetAsynUser); // Should we clear out waiting partial messages?
					}
					if (pRegReadback[0].addr > lastResponseCount_ + 1)
						asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
								"%s: Missing response message, response from request #%u, expected is request #%u\n",
								__PRETTY_FUNCTION__, (unsigned) pRegReadback[0].addr,
								lastResponseCount_ + 1);
					if (pRegReadback[0].addr < lastResponseCount_ + 1)
						asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
								"%s: Out-of-order response message, response from request #%u, expected is request #%u\n",
								__PRETTY_FUNCTION__, (unsigned) pRegReadback[0].addr,
								lastResponseCount_ + 1);
					lastResponseCount_ = pRegReadback[0].addr;

					processReadbackBuffer(pRegReadback);

					// check if this was a response to the most recent request
					if (pRegReadback[0].addr < netSendCount_)
					{
						asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
								"%s: processed response from request #%u, most recent is request #%u\n",
								__PRETTY_FUNCTION__, (unsigned) pRegReadback[0].addr,
								netSendCount_);
						netWaitingRequests_--;
					}
					else
						netWaitingRequests_ = 0; // We have the most recent, no outstanding requests

					//printf("%s, buffer processing loop: readCount=%zd, pRegReadback[0].data=%d\n",__PRETTY_FUNCTION__, readCount, pRegReadback[0].data);
					readCount -= (size_t) pRegReadback[0].data;
					// The nonce in pRegReadback[0] contains the message size. Move pointer to the next message.
					pRegReadback = & pRegReadback[pRegReadback[0].data/sizeof(FpgaReg)];
					//printf("%s, buffer processing next loop: readCount=%zd, pRegReadback[0].data=%d\n",__PRETTY_FUNCTION__, readCount, pRegReadback[0].data);
				}
				epicsThreadSleep(0.002); // sleep to be nice to other threads
			} // while (netWaitingRequests>0)
		} // if (status == epicsEventWaitOK)
		if (isShuttingDown_)
		{
			break;
		}
	}
}

// Iterate over each register in the buffer returned from the FPGA
// Array passed to this function contains the nonce, with the buffer
// size at pRegReadback[0].data
asynStatus scllrfAsynPortDriver::processReadbackBuffer(const FpgaReg *pRegReadback)
{
	unsigned i;
	bool waveIsReady = false; // Made this local rather than member because of concurrency concerns
	asynStatus status;
	for (i = 1; i < pRegReadback[0].data / sizeof(FpgaReg); i++)
	{
		status = processRegReadback(&pRegReadback[i], waveIsReady);
		if (status)
			epicsSnprintf(pOctetAsynUser_->errorMessage,
					pOctetAsynUser_->errorMessageSize,
					"%s: status=%d, address=0x%x, value=%u",
					__PRETTY_FUNCTION__, status,
					(unsigned) pRegReadback[i].addr,
					(unsigned) pRegReadback[i].data);
//		else
//			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
//					"%s: address=0x%x, value=%u\n", __PRETTY_FUNCTION__,
//					(unsigned ) pRegReadback[i].addr,
//					(unsigned ) pRegReadback[i].data);
	}
	/* Do callbacks so higher layers see any changes */
	status = (asynStatus) callParamCallbacks();

	// If the waveIsReady flag is set,
	if (waveIsReady &&
			// and there isn't a pending waveform read
			(newWaveAvailable_ == newWaveRead_))
	{
		// Set the message counter with a "new waveform" notification
		// to the message counter value for the message we just received
		newWaveAvailable_ = pRegReadback[0].addr;
		epicsEventSignal(reqWaveEventId_);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,"%s: new waveform data available, signaling the waveform requester\n",
				__PRETTY_FUNCTION__);
	}

	//	cout << __PRETTY_FUNCTION__ << ": waveIsReady: " << waveIsReady << ", newWaveAvailable: ";
	//	cout << newWaveAvailable << ", newWaveRead: " << newWaveRead << "\n";

	return asynSuccess;
}

// parse register data, write to array PV
asynStatus scllrfAsynPortDriver::processWaveReadback(const FpgaReg *pFromFpga)
{
	unsigned int bufferOffset = (pFromFpga->addr & addrMask) - wavesStart;
	unsigned int waveNumber = bufferOffset % wavesCount;
	unsigned int waveIndex = bufferOffset / wavesCount;

	pWaveform_[waveNumber][waveIndex] = (epicsInt32) pFromFpga->data;

	return asynSuccess;
}

// Extract register address and data from the received message and set the appropriate
// asyn parameter.
// Some registers have a "new waveform data ready" flag. If they have this and it is set,
// set weveIsReady to true.
// Note: This function should not set waveIsReady to false. That is done by a loop in the
// calling function.
asynStatus scllrfAsynPortDriver::processRegReadback(const FpgaReg *pFromFpga, bool &waveIsReady)
{
	unsigned int i;
	asynStatus status = asynSuccess;

	/* Map address to parameter, set the parameter in the parameter library. */
	switch (pFromFpga->addr & addrMask)
	{
	case MagicAdr:
		status = (asynStatus) setIntegerParam(P_Magic,
				pFromFpga->data & MagicMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				P_MagicString, (unsigned ) pFromFpga->data & MagicMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case DspFlavorAdr:
		status = (asynStatus) setIntegerParam(P_DspFlavor,
				pFromFpga->data & DspFlavorMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				P_DspFlavorString, (unsigned ) pFromFpga->data & DspFlavorMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case BuildYearAdr:
		status = (asynStatus) setIntegerParam(P_BuildYear,
				pFromFpga->data & BuildYearMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				P_BuildYearString, (unsigned ) pFromFpga->data & BuildYearMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case BuildMonthAdr:
		status = (asynStatus) setIntegerParam(P_BuildMonth,
				pFromFpga->data & BuildMonthMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				P_BuildMonthString, (unsigned ) pFromFpga->data & BuildMonthMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case BuildDayAdr:
		status = (asynStatus) setIntegerParam(P_BuildDay,
				pFromFpga->data & BuildDayMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				P_BuildDayString, (unsigned ) pFromFpga->data & BuildDayMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case BuildHourAdr:
		status = (asynStatus) setIntegerParam(P_BuildHour,
				pFromFpga->data & BuildHourMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				P_BuildHourString, (unsigned ) pFromFpga->data & BuildHourMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case BuildMinuteAdr:
		status = (asynStatus) setIntegerParam(P_BuildMinute,
				pFromFpga->data & BuildMinuteMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				P_BuildMinuteString, (unsigned ) pFromFpga->data & BuildMinuteMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case CodeIsCleanAdr:
		status = (asynStatus) setIntegerParam(P_CodeIsClean,
				pFromFpga->data & CodeIsCleanMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				P_CodeIsCleanString, (unsigned ) pFromFpga->data & CodeIsCleanMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case ToolRevAdr:
		status = (asynStatus) setIntegerParam(P_ToolRev,
				pFromFpga->data & ToolRevMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				P_ToolRevString, (unsigned ) pFromFpga->data & ToolRevMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case UserAdr:
		status = (asynStatus) setIntegerParam(P_User,
				pFromFpga->data & UserMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				P_UserString, (unsigned ) pFromFpga->data & UserMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case BoardTypeAdr:
		status = (asynStatus) setIntegerParam(P_BoardType,
				pFromFpga->data & BoardTypeMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				P_BoardTypeString, (unsigned ) pFromFpga->data & BoardTypeMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case VersionAdr:
		status = (asynStatus) setIntegerParam(P_Version,
				pFromFpga->data & VersionMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				P_VersionString, (unsigned ) pFromFpga->data & VersionMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case GitSHA1aAdr:
		status = (asynStatus) setIntegerParam(P_GitSHA1a,
				pFromFpga->data & GitSHA1aMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				P_GitSHA1aString, (unsigned ) pFromFpga->data & GitSHA1aMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case GitSHA1bAdr:
		status = (asynStatus) setIntegerParam(P_GitSHA1b,
				pFromFpga->data & GitSHA1bMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				P_GitSHA1bString, (unsigned ) pFromFpga->data & GitSHA1bMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case GitSHA1cAdr:
		status = (asynStatus) setIntegerParam(P_GitSHA1c,
				pFromFpga->data & GitSHA1cMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				P_GitSHA1cString, (unsigned ) pFromFpga->data & GitSHA1cMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case GitSHA1dAdr:
		status = (asynStatus) setIntegerParam(P_GitSHA1d,
				pFromFpga->data & GitSHA1dMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				P_GitSHA1dString, (unsigned ) pFromFpga->data & GitSHA1dMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case GitSHA1eAdr:
		status = (asynStatus) setIntegerParam(P_GitSHA1e,
				pFromFpga->data & GitSHA1eMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				P_GitSHA1eString, (unsigned ) pFromFpga->data & GitSHA1eMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case GitSHA1fAdr:
		status = (asynStatus) setIntegerParam(P_GitSHA1f,
				pFromFpga->data & GitSHA1fMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				P_GitSHA1fString, (unsigned ) pFromFpga->data & GitSHA1fMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case GitSHA1gAdr:
		status = (asynStatus) setIntegerParam(P_GitSHA1g,
				pFromFpga->data & GitSHA1gMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				P_GitSHA1gString, (unsigned ) pFromFpga->data & GitSHA1gMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case GitSHA1hAdr:
		status = (asynStatus) setIntegerParam(P_GitSHA1h,
				pFromFpga->data & GitSHA1hMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				P_GitSHA1hString, (unsigned ) pFromFpga->data & GitSHA1hMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case GitSHA1iAdr:
		status = (asynStatus) setIntegerParam(P_GitSHA1i,
				pFromFpga->data & GitSHA1iMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				P_GitSHA1iString, (unsigned ) pFromFpga->data & GitSHA1iMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case GitSHA1jAdr:
		status = (asynStatus) setIntegerParam(P_GitSHA1j,
				pFromFpga->data & GitSHA1jMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				P_GitSHA1jString, (unsigned ) pFromFpga->data & GitSHA1jMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case GitSHA1kAdr:
		status = (asynStatus) setIntegerParam(P_GitSHA1k,
				pFromFpga->data & GitSHA1kMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				P_GitSHA1kString, (unsigned ) pFromFpga->data & GitSHA1kMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case GitSHA1lAdr:
		status = (asynStatus) setIntegerParam(P_GitSHA1l,
				pFromFpga->data & GitSHA1lMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				P_GitSHA1lString, (unsigned ) pFromFpga->data & GitSHA1lMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case GitSHA1mAdr:
		status = (asynStatus) setIntegerParam(P_GitSHA1m,
				pFromFpga->data & GitSHA1mMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				P_GitSHA1mString, (unsigned ) pFromFpga->data & GitSHA1mMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case GitSHA1nAdr:
		status = (asynStatus) setIntegerParam(P_GitSHA1n,
				pFromFpga->data & GitSHA1nMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				P_GitSHA1nString, (unsigned ) pFromFpga->data & GitSHA1nMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case GitSHA1oAdr:
		status = (asynStatus) setIntegerParam(P_GitSHA1o,
				pFromFpga->data & GitSHA1oMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				P_GitSHA1oString, (unsigned ) pFromFpga->data & GitSHA1oMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case GitSHA1pAdr:
		status = (asynStatus) setIntegerParam(P_GitSHA1p,
				pFromFpga->data & GitSHA1pMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				P_GitSHA1pString, (unsigned ) pFromFpga->data & GitSHA1pMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case GitSHA1qAdr:
		status = (asynStatus) setIntegerParam(P_GitSHA1q,
				pFromFpga->data & GitSHA1qMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				P_GitSHA1qString, (unsigned ) pFromFpga->data & GitSHA1qMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case GitSHA1rAdr:
		status = (asynStatus) setIntegerParam(P_GitSHA1r,
				pFromFpga->data & GitSHA1rMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				P_GitSHA1rString, (unsigned ) pFromFpga->data & GitSHA1rMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case GitSHA1sAdr:
		status = (asynStatus) setIntegerParam(P_GitSHA1s,
				pFromFpga->data & GitSHA1sMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				P_GitSHA1sString, (unsigned ) pFromFpga->data & GitSHA1sMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case GitSHA1tAdr:
		status = (asynStatus) setIntegerParam(P_GitSHA1t,
				pFromFpga->data & GitSHA1tMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				P_GitSHA1tString, (unsigned ) pFromFpga->data & GitSHA1tMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case wavesEnd:
		processWaveReadback(pFromFpga);
		// Do we need to verify that all points of all waveforms have been received, not missing
		// packets or some such?
		// Basic approach: when we read in the last point of the last waveform, publish them all
		for (i=0; i<wavesCount; i++)
			status = doCallbacksInt32Array(pWaveform_[i], waveBufferRegCount/wavesCount, P_Waveform, i);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: got last waveform datapoint. Publishing.\n", __PRETTY_FUNCTION__);
		break;
	default:
		if( wavesStart < (pFromFpga->addr & addrMask) && (pFromFpga->addr & addrMask) < wavesEnd )
			processWaveReadback(pFromFpga);
		else
			return asynError;
		break;
	}

	return status;
}

void htonFpgaRegArray(FpgaReg *buffer, unsigned int regCount)
{
	unsigned int i;

	for (i=0; i<regCount; i++)
	{
		buffer[i].addr = htonl(buffer[i].addr);
		buffer[i].data = htonl(buffer[i].data);
	}
}


void ntohFpgaRegArray(FpgaReg *buffer, unsigned int regCount)
{
	unsigned int i;

	for (i=0; i<regCount; i++)
	{
		buffer[i].addr = ntohl(buffer[i].addr);
		buffer[i].data = ntohl(buffer[i].data);
	}
}

/* Configuration routine.  Called directly, or from the iocsh function below */

extern "C" {

/** EPICS iocsh callable function to call constructor for the scllrfAsynPortDriver class.
 * \param[in] portName The name of the asyn port driver to be created.
 * \param[in] maxArrayLength The maximum  number of points in the volt and time arrays */
int scllrfAsynPortDriverConfigure(const char *drvPortName, const char *netPortName)
{
	new scllrfAsynPortDriver(drvPortName, netPortName);
	return asynSuccess;
}


/* EPICS iocsh shell commands */

static const iocshArg initArg0 = { "drvPortName",iocshArgString};
static const iocshArg initArg1 = { "IP port name",iocshArgString};
static const iocshArg * const initArgs[] = {&initArg0,
		&initArg1};
static const iocshFuncDef initFuncDef = {"scllrfAsynPortDriverConfigure",2,initArgs};
static void initCallFunc(const iocshArgBuf *args)
{
	scllrfAsynPortDriverConfigure(args[0].sval, args[1].sval);
}

void scllrfAsynPortDriverRegister(void)
{
	iocshRegister(&initFuncDef,initCallFunc);
}

epicsExportRegistrar(scllrfAsynPortDriverRegister);

}

