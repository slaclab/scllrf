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
		isShuttingDown_(0), netSendCount_(0), lastResponseCount_ (0), netWaitingRequests_(0),
		newWaveAvailable_(0), newWaveRead_ (0), p_RunStop (stop)
{
	asynStatus status = asynSuccess;

    epicsThreadSleep(defaultPollPeriod);

    printf("%s creating %ld parameters.\n",__PRETTY_FUNCTION__,NUM_CMOC_PARAMS);

    createParam(RunStopString, asynParamInt32, (int *) &p_RunStop);
    createParam(MaxParallelRequestsString, asynParamInt32, &p_MaxParallelRequests);
    createParam(PollPeriodString, asynParamFloat64, &p_PollPeriod);
	createParam(MagicString, asynParamInt32, &p_Magic);
	createParam(DspFlavorString, asynParamInt32, &p_DspFlavor);
	createParam(BuildYearString, asynParamInt32, &p_BuildYear);
	createParam(BuildMonthString, asynParamInt32, &p_BuildMonth);
	createParam(BuildDayString, asynParamInt32, &p_BuildDay);
	createParam(BuildHourString, asynParamInt32, &p_BuildHour);
	createParam(BuildMinuteString, asynParamInt32, &p_BuildMinute);
	createParam(CodeIsCleanString, asynParamInt32, &p_CodeIsClean);
	createParam(ToolRevString, asynParamInt32, &p_ToolRev);
	createParam(UserString, asynParamInt32, &p_User);
	createParam(BoardTypeString, asynParamInt32, &p_BoardType);
	createParam(VersionString, asynParamInt32, &p_Version);
	createParam(GitSHA1aString, asynParamInt32, &p_GitSHA1a);
	createParam(GitSHA1bString, asynParamInt32, &p_GitSHA1b);
	createParam(GitSHA1cString, asynParamInt32, &p_GitSHA1c);
	createParam(GitSHA1dString, asynParamInt32, &p_GitSHA1d);
	createParam(GitSHA1eString, asynParamInt32, &p_GitSHA1e);
	createParam(GitSHA1fString, asynParamInt32, &p_GitSHA1f);
	createParam(GitSHA1gString, asynParamInt32, &p_GitSHA1g);
	createParam(GitSHA1hString, asynParamInt32, &p_GitSHA1h);
	createParam(GitSHA1iString, asynParamInt32, &p_GitSHA1i);
	createParam(GitSHA1jString, asynParamInt32, &p_GitSHA1j);
	createParam(GitSHA1kString, asynParamInt32, &p_GitSHA1k);
	createParam(GitSHA1lString, asynParamInt32, &p_GitSHA1l);
	createParam(GitSHA1mString, asynParamInt32, &p_GitSHA1m);
	createParam(GitSHA1nString, asynParamInt32, &p_GitSHA1n);
	createParam(GitSHA1oString, asynParamInt32, &p_GitSHA1o);
	createParam(GitSHA1pString, asynParamInt32, &p_GitSHA1p);
	createParam(GitSHA1qString, asynParamInt32, &p_GitSHA1q);
	createParam(GitSHA1rString, asynParamInt32, &p_GitSHA1r);
	createParam(GitSHA1sString, asynParamInt32, &p_GitSHA1s);
	createParam(GitSHA1tString, asynParamInt32, &p_GitSHA1t);
    createParam(DspFdbkCoreMpProcCoeffString, asynParamInt32Array, &p_DspFdbkCoreMpProcCoeff);
    createParam(DspFdbkCoreMpProcLimString, asynParamInt32Array, &p_DspFdbkCoreMpProcLim);
    createParam(DspFdbkCoreMpProcSetmpString, asynParamInt32Array, &p_DspFdbkCoreMpProcSetmp);
    createParam(DspLpNotchLp1AKxString, asynParamInt32Array, &p_DspLpNotchLp1AKx);
    createParam(DspLpNotchLp1AKyString, asynParamInt32Array, &p_DspLpNotchLp1AKy);
    createParam(DspLpNotchLp1BKxString, asynParamInt32Array, &p_DspLpNotchLp1BKx);
    createParam(DspLpNotchLp1BKyString, asynParamInt32Array, &p_DspLpNotchLp1BKy);
    createParam(DspChanKeepString, asynParamInt32, &p_DspChanKeep);
    createParam(DspFdbkCoreCoarseScaleString, asynParamInt32, &p_DspFdbkCoreCoarseScale);
    createParam(DspFdbkCoreMpProcPhOffsetString, asynParamInt32, &p_DspFdbkCoreMpProcPhOffset);
    createParam(DspFdbkCoreMpProcSelEnString, asynParamInt32, &p_DspFdbkCoreMpProcSelEn);
    createParam(DspFdbkCoreMpProcSelThreshString, asynParamInt32, &p_DspFdbkCoreMpProcSelThresh);
    createParam(DspModuloString, asynParamInt32, &p_DspModulo);
    createParam(DspPhaseStepString, asynParamInt32, &p_DspPhaseStep);
    createParam(DspPiezoPiezoDcString, asynParamInt32, &p_DspPiezoPiezoDc);
    createParam(DspTagString, asynParamInt32, &p_DspTag);
    createParam(DspWaveSampPerString, asynParamInt32, &p_DspWaveSampPer);
    createParam(DspWaveShiftString, asynParamInt32, &p_DspWaveShift);
    createParam(Waveform1String,asynParamInt32Array, &p_Waveform);

    epicsThreadSleep(defaultPollPeriod);

    printf("%s done creating %ld parameters.\n",__PRETTY_FUNCTION__,NUM_CMOC_PARAMS);

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

    setIntegerParam(p_RunStop, stop);
    setIntegerParam(p_MaxParallelRequests, defaultMaxParallelRequests);
    setDoubleParam(p_PollPeriod, defaultPollPeriod);

    // The createParam calls are queued up rather than executed right away, so
    // give them a chance to be created before we start writing to them.
    // TODO: Is there a way to check that the params were finished being created?
    epicsThreadSleep(defaultPollPeriod);

	readEventId_ = epicsEventMustCreate(epicsEventEmpty);
	startResponseHandler();

	reqWaveEventId_ = epicsEventMustCreate(epicsEventEmpty);
	startWaveformRequester();

    epicsThreadSleep(defaultPollPeriod);

    printf("%s starting polling thread.\n",__PRETTY_FUNCTION__);

	pollEventId_ = epicsEventMustCreate(epicsEventEmpty);
	startPoller(defaultPollPeriod);

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


/** Called when you have the asyn parameter name and want the corresponding
 * register address.
  * \param[in] function From pAsynUser->reason, corresponding to a registered parameter.
  * \param[in] pToFpga Pointer to the {Address, Data} structure where the register address will be written
  * \param[in] nElements Number of elements to read. */
asynStatus scllrfAsynPortDriver::functionToRegister(const int function, FpgaReg *pToFpga)
{
	unsigned int i;
	asynStatus status = asynSuccess;

    // Assume this is for a write function, doesn't have to include read only registers (so far)
    if( function ==  p_DspFdbkCoreMpProcCoeff)
    for(i=0; i<4; i++){
		pToFpga[i].addr = DspFdbkCoreMpProcCoeffAdr+i;
    }
    else if( function == p_DspFdbkCoreMpProcLim )
    for(i=0; i<4; i++){
		pToFpga[i].addr = DspFdbkCoreMpProcLimAdr+i;
    }
    else if( function == p_DspFdbkCoreMpProcSetmp )
    for(i=0; i<4; i++){
		pToFpga[i].addr = DspFdbkCoreMpProcSetmpAdr+i;
    }
    else if( function == p_DspLpNotchLp1AKx )
    for(i=0; i<2; i++){
		pToFpga[i].addr = DspLpNotchLp1AKxAdr+i;
    }
    else if( function == p_DspLpNotchLp1AKy )
    for(i=0; i<2; i++){
		pToFpga[i].addr = DspLpNotchLp1AKyAdr+i;
    }
    else if( function == p_DspLpNotchLp1BKx )
    for(i=0; i<2; i++){
		pToFpga[i].addr = DspLpNotchLp1BKxAdr+i;
    }
    else if( function == p_DspLpNotchLp1BKy )
    for(i=0; i<2; i++){
		pToFpga[i].addr = DspLpNotchLp1BKyAdr+i;
    }
    else if( function == p_DspChanKeep )
    {
		pToFpga->addr = DspChanKeepAdr;
    }
    else if( function == p_DspFdbkCoreCoarseScale )
    {
		pToFpga->addr = DspFdbkCoreCoarseScaleAdr;
    }
    else if( function == p_DspFdbkCoreMpProcPhOffset )
    {
		pToFpga->addr = DspFdbkCoreMpProcPhOffsetAdr;
    }
    else if( function == p_DspFdbkCoreMpProcSelEn )
    {
		pToFpga->addr = DspFdbkCoreMpProcSelEnAdr;
    }
    else if( function == p_DspFdbkCoreMpProcSelThresh )
    {
		pToFpga->addr = DspFdbkCoreMpProcSelThreshAdr;
    }
    else if( function == p_DspModulo )
    {
		pToFpga->addr = DspModuloAdr;
    }
    else if( function == p_DspPhaseStep )
    {
		pToFpga->addr = DspPhaseStepAdr;
    }
    else if( function == p_DspPiezoPiezoDc )
    {
		pToFpga->addr = DspPiezoPiezoDcAdr;
    }
    else if( function == p_DspTag )
    {
		pToFpga->addr = DspTagAdr;
    }
    else if( function == p_DspWaveSampPer )
    {
		pToFpga->addr = DspWaveSampPerAdr;
    }
    else if( function == p_DspWaveShift )
    {
		pToFpga->addr = DspWaveShiftAdr;
    }
    else
    	status = asynError;

    return status;
}

/** Called when asyn clients call pasynInt32->read().
 * \param[in] pasynUser pasynUser structure that encodes the reason and address.
 * \param[in] value Pointer to the value to read. */
asynStatus scllrfAsynPortDriver::writeInt32(asynUser *pasynUser, epicsInt32 value)
{
	int function = pasynUser->reason;
//	int addr = 0;
	asynStatus status = asynSuccess;
    const char *paramName;
    FpgaReg regSendBuf[2];

	epicsTimeStamp timeStamp; getTimeStamp(&timeStamp);

    /* Set the parameter in the parameter library. */
    status = (asynStatus) setIntegerParam(function, value);

    /* Fetch the parameter string name for possible use in debugging */
    getParamName(function, &paramName);

    if (function == p_RunStop) {
        if (value == run)
        	epicsEventSignal(pollEventId_);
    }
    else {
    	// Convert function to address & FpgaReg.
    	status = functionToRegister(function, &regSendBuf[1]);
    	if (status != asynSuccess)
    		return status;
    	regSendBuf[1].data = (uint32_t) value;
    	htonFpgaRegArray(regSendBuf, 2);
    	sendRegRequest(regSendBuf, 2);
    }

	/* Do callbacks so higher layers see any changes */
	status = (asynStatus) callParamCallbacks();

    if (status)
        epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize,
                  "%s: status=%d, function=%d, name=%s, value=%d",
				  __PRETTY_FUNCTION__, status, function, paramName, value);
    else
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER,
              "%s: function=%d, name=%s, value=%d\n",
			  __PRETTY_FUNCTION__, function, paramName, value);
    return status;
}

/** Called when asyn clients call pasynInt32Array->write().
  * The base class implementation simply prints an error message.
  * Derived classes may reimplement this function if required.
  * \param[in] pasynUser pasynUser structure that encodes the reason and address.
  * \param[in] value Pointer to the array to write.
  * \param[in] nElements Number of elements to write. */
asynStatus scllrfAsynPortDriver::writeInt32Array(asynUser *pasynUser, epicsInt32 *value,
                                size_t nElements)
{
	int function = pasynUser->reason;
	asynStatus status = asynSuccess;
//	int nCopy;
	FpgaReg regSendBuf[maxMsgSize/sizeof(FpgaReg)];
	unsigned int i;

	//getIntegerParam(P_ArrayLength, &nCopy);
	//if ((int) nElements < nCopy)
	//	nCopy = (int) nElements;

	status = functionToRegister(function, &regSendBuf[1]);
	if (status != asynSuccess)
		return status;

	for(i=0; i<nElements; i++)
	{
		regSendBuf[i+1].data = (uint32_t) value[i];
		//*nIn = nCopy;
	}
	htonFpgaRegArray(regSendBuf, nElements);
	sendRegRequest(regSendBuf, nElements);

	if (status)
		epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize,
				"%s: status=%d, function=%d", __PRETTY_FUNCTION__,
				status, function);
	else
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, "%s: function=%d\n",
				__PRETTY_FUNCTION__, function);

	return status; //(writeArray < epicsInt32 > (pasynUser, value, nElements));
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
    int runStop;

	// A canned request to read all registers
	static FpgaReg cmocReadAllRegMsg[regCount + 1] =
	{
			{ 0, 0 },
			{ flagReadMask | 0x00, blankData },
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

	epicsEventWait(pollEventId_); // Block when first created, to give subclass constructors a chance to finish
	while(1) {
		getDoubleParam(p_PollPeriod, &pollPeriod_);
        getIntegerParam(p_RunStop, &runStop);
		if (runStop == run && pollPeriod_ != 0.0) status = epicsEventWaitWithTimeout(pollEventId_, pollPeriod_);
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

/** Polls the device
  * \param[in] regBuffer array of FpgaReg messages in network byte order.
  * Note that the first "register" regBuffer[0] is really the nonce, not data.
  * This function will set the nonce before sending.
  * Other than the nonce, data in the array is not changed. This reduces the
  * processing required, since most messages are canned and repeated at regular intervals.
  * \param[in] regBuffCount is the number of FpgaReg type elements, including the nonce
 **  */
asynStatus scllrfAsynPortDriver::sendRegRequest(FpgaReg *regBuffer, unsigned int regBuffCount)
{
	assert(regBuffer != NULL);
	assert(regBuffCount >= 2);
	size_t writtenCount;
	asynStatus status = asynSuccess;
	char * pWriteBuffer;
	pWriteBuffer = reinterpret_cast<char*>(regBuffer);
	int maxParallelRequests;
	getIntegerParam(p_MaxParallelRequests, &maxParallelRequests);

	// Throttle so that we don't overflow buffers if response handling falls behind
	if( netWaitingRequests_ > (unsigned) maxParallelRequests )
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: too many requests waiting for responses, throttling requests.\n",__PRETTY_FUNCTION__);
	while( netWaitingRequests_ > (unsigned) maxParallelRequests )
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
	epicsThreadSleep(0);
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
	static char pReadBuffer[maxMsgSize];
	FpgaReg *pRegReadback;
	size_t readCount = 0; // Number of bytes to process from the network read
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
						maxMsgSize, 0.001, &readCount, &eomReason);
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

				processReadbackBuffer(pRegReadback, readCount);
				/* Do callbacks so higher layers see any changes */
				status = (asynStatus) callParamCallbacks();

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
asynStatus scllrfAsynPortDriver::processReadbackBuffer(FpgaReg *pRegReadback, unsigned int readCount)
{
	unsigned i;
	bool waveIsReady = false; // Made this local rather than member because of concurrency concerns
	asynStatus status = asynSuccess;

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

		for (i = 1; i < pRegReadback[0].data / sizeof(FpgaReg); i++)
		{
			if(pRegReadback[i].addr & flagReadMask)
			{
			status = processRegReadback(&pRegReadback[i], waveIsReady);
			}
			else
			{
				processRegWriteResponse(&pRegReadback[i]);
			}
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
		// decrement number of bytes left to process by the numbr of bytes we just processed
		readCount -= (size_t) pRegReadback[0].data;

		// The nonce in pRegReadback[0] contains the message size. Move pointer to the next message.
		pRegReadback = & pRegReadback[pRegReadback[0].data/sizeof(FpgaReg)];
		//printf("%s, buffer processing next loop: readCount=%zd, pRegReadback[0].data=%d\n",__PRETTY_FUNCTION__, readCount, pRegReadback[0].data);
	} // while(readCount > 0)

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

/**  Extract register address and data from the received message and set the appropriate
 * asyn parameter.
 * Some registers have a "new waveform data ready" flag. If they have this and it is set,
 * set weveIsReady to true.
 * Note: This function should not set waveIsReady to false. That is done by a loop in the
 * calling function.
* \param[in] pFromFpga Data returned from the FPGA for a single register
* \param[in] waveIsReady A flag that gets set to true if the appropriate bit was set by the FPGA
*/
asynStatus scllrfAsynPortDriver::processRegReadback(const FpgaReg *pFromFpga, bool &waveIsReady)
{
	unsigned int i;
	asynStatus status = asynSuccess;
	assert(pFromFpga->addr&flagReadMask); // This function is only for read registers

	/* Map address to parameter, set the parameter in the parameter library. */
	switch (pFromFpga->addr)
	{
	case MagicAdr|flagReadMask:
		status = (asynStatus) setIntegerParam(p_Magic,
				pFromFpga->data & MagicMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				MagicString, (unsigned ) pFromFpga->data & MagicMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case DspFlavorAdr|flagReadMask:
		status = (asynStatus) setIntegerParam(p_DspFlavor,
				pFromFpga->data & DspFlavorMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				DspFlavorString, (unsigned ) pFromFpga->data & DspFlavorMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case BuildYearAdr|flagReadMask:
		status = (asynStatus) setIntegerParam(p_BuildYear,
				pFromFpga->data & BuildYearMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				BuildYearString, (unsigned ) pFromFpga->data & BuildYearMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case BuildMonthAdr|flagReadMask:
		status = (asynStatus) setIntegerParam(p_BuildMonth,
				pFromFpga->data & BuildMonthMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				BuildMonthString, (unsigned ) pFromFpga->data & BuildMonthMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case BuildDayAdr|flagReadMask:
		status = (asynStatus) setIntegerParam(p_BuildDay,
				pFromFpga->data & BuildDayMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				BuildDayString, (unsigned ) pFromFpga->data & BuildDayMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case BuildHourAdr|flagReadMask:
		status = (asynStatus) setIntegerParam(p_BuildHour,
				pFromFpga->data & BuildHourMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				BuildHourString, (unsigned ) pFromFpga->data & BuildHourMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case BuildMinuteAdr|flagReadMask:
		status = (asynStatus) setIntegerParam(p_BuildMinute,
				pFromFpga->data & BuildMinuteMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				BuildMinuteString, (unsigned ) pFromFpga->data & BuildMinuteMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case CodeIsCleanAdr|flagReadMask:
		status = (asynStatus) setIntegerParam(p_CodeIsClean,
				pFromFpga->data & CodeIsCleanMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				CodeIsCleanString, (unsigned ) pFromFpga->data & CodeIsCleanMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case ToolRevAdr|flagReadMask:
		status = (asynStatus) setIntegerParam(p_ToolRev,
				pFromFpga->data & ToolRevMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				ToolRevString, (unsigned ) pFromFpga->data & ToolRevMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case UserAdr|flagReadMask:
		status = (asynStatus) setIntegerParam(p_User,
				pFromFpga->data & UserMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				UserString, (unsigned ) pFromFpga->data & UserMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case BoardTypeAdr|flagReadMask:
		status = (asynStatus) setIntegerParam(p_BoardType,
				pFromFpga->data & BoardTypeMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				BoardTypeString, (unsigned ) pFromFpga->data & BoardTypeMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case VersionAdr|flagReadMask:
		status = (asynStatus) setIntegerParam(p_Version,
				pFromFpga->data & VersionMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				VersionString, (unsigned ) pFromFpga->data & VersionMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case GitSHA1aAdr|flagReadMask:
		status = (asynStatus) setIntegerParam(p_GitSHA1a,
				pFromFpga->data & GitSHA1aMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				GitSHA1aString, (unsigned ) pFromFpga->data & GitSHA1aMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case GitSHA1bAdr|flagReadMask:
		status = (asynStatus) setIntegerParam(p_GitSHA1b,
				pFromFpga->data & GitSHA1bMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				GitSHA1bString, (unsigned ) pFromFpga->data & GitSHA1bMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case GitSHA1cAdr|flagReadMask:
		status = (asynStatus) setIntegerParam(p_GitSHA1c,
				pFromFpga->data & GitSHA1cMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				GitSHA1cString, (unsigned ) pFromFpga->data & GitSHA1cMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case GitSHA1dAdr|flagReadMask:
		status = (asynStatus) setIntegerParam(p_GitSHA1d,
				pFromFpga->data & GitSHA1dMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				GitSHA1dString, (unsigned ) pFromFpga->data & GitSHA1dMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case GitSHA1eAdr|flagReadMask:
		status = (asynStatus) setIntegerParam(p_GitSHA1e,
				pFromFpga->data & GitSHA1eMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				GitSHA1eString, (unsigned ) pFromFpga->data & GitSHA1eMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case GitSHA1fAdr|flagReadMask:
		status = (asynStatus) setIntegerParam(p_GitSHA1f,
				pFromFpga->data & GitSHA1fMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				GitSHA1fString, (unsigned ) pFromFpga->data & GitSHA1fMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case GitSHA1gAdr|flagReadMask:
		status = (asynStatus) setIntegerParam(p_GitSHA1g,
				pFromFpga->data & GitSHA1gMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				GitSHA1gString, (unsigned ) pFromFpga->data & GitSHA1gMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case GitSHA1hAdr|flagReadMask:
		status = (asynStatus) setIntegerParam(p_GitSHA1h,
				pFromFpga->data & GitSHA1hMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				GitSHA1hString, (unsigned ) pFromFpga->data & GitSHA1hMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case GitSHA1iAdr|flagReadMask:
		status = (asynStatus) setIntegerParam(p_GitSHA1i,
				pFromFpga->data & GitSHA1iMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				GitSHA1iString, (unsigned ) pFromFpga->data & GitSHA1iMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case GitSHA1jAdr|flagReadMask:
		status = (asynStatus) setIntegerParam(p_GitSHA1j,
				pFromFpga->data & GitSHA1jMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				GitSHA1jString, (unsigned ) pFromFpga->data & GitSHA1jMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case GitSHA1kAdr|flagReadMask:
		status = (asynStatus) setIntegerParam(p_GitSHA1k,
				pFromFpga->data & GitSHA1kMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				GitSHA1kString, (unsigned ) pFromFpga->data & GitSHA1kMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case GitSHA1lAdr|flagReadMask:
		status = (asynStatus) setIntegerParam(p_GitSHA1l,
				pFromFpga->data & GitSHA1lMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				GitSHA1lString, (unsigned ) pFromFpga->data & GitSHA1lMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case GitSHA1mAdr|flagReadMask:
		status = (asynStatus) setIntegerParam(p_GitSHA1m,
				pFromFpga->data & GitSHA1mMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				GitSHA1mString, (unsigned ) pFromFpga->data & GitSHA1mMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case GitSHA1nAdr|flagReadMask:
		status = (asynStatus) setIntegerParam(p_GitSHA1n,
				pFromFpga->data & GitSHA1nMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				GitSHA1nString, (unsigned ) pFromFpga->data & GitSHA1nMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case GitSHA1oAdr|flagReadMask:
		status = (asynStatus) setIntegerParam(p_GitSHA1o,
				pFromFpga->data & GitSHA1oMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				GitSHA1oString, (unsigned ) pFromFpga->data & GitSHA1oMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case GitSHA1pAdr|flagReadMask:
		status = (asynStatus) setIntegerParam(p_GitSHA1p,
				pFromFpga->data & GitSHA1pMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				GitSHA1pString, (unsigned ) pFromFpga->data & GitSHA1pMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case GitSHA1qAdr|flagReadMask:
		status = (asynStatus) setIntegerParam(p_GitSHA1q,
				pFromFpga->data & GitSHA1qMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				GitSHA1qString, (unsigned ) pFromFpga->data & GitSHA1qMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case GitSHA1rAdr|flagReadMask:
		status = (asynStatus) setIntegerParam(p_GitSHA1r,
				pFromFpga->data & GitSHA1rMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				GitSHA1rString, (unsigned ) pFromFpga->data & GitSHA1rMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case GitSHA1sAdr|flagReadMask:
		status = (asynStatus) setIntegerParam(p_GitSHA1s,
				pFromFpga->data & GitSHA1sMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				GitSHA1sString, (unsigned ) pFromFpga->data & GitSHA1sMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case GitSHA1tAdr|flagReadMask:
		status = (asynStatus) setIntegerParam(p_GitSHA1t,
				pFromFpga->data & GitSHA1tMask);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				GitSHA1tString, (unsigned ) pFromFpga->data & GitSHA1tMask);
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case wavesEnd|flagReadMask:
		processWaveReadback(pFromFpga);
		// Do we need to verify that all points of all waveforms have been received, not missing
		// packets or some such?
		// Basic approach: when we read in the last point of the last waveform, publish them all
		for (i=0; i<wavesCount; i++)
			status = doCallbacksInt32Array(pWaveform_[i], waveBufferRegCount/wavesCount, p_Waveform, i);
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

/**  Extract register address and data from the received message and set the appropriate
 * asyn parameter.
 * Some registers have a "new waveform data ready" flag. If they have this and it is set,
 * set weveIsReady to true.
 * Note: This function should not set waveIsReady to false. That is done by a loop in the
 * calling function.
* \param[in] pFromFpga Data returned from the FPGA for a single register
* \param[in] waveIsReady A flag that gets set to true if the appropriate bit was set by the FPGA
*/
asynStatus scllrfAsynPortDriver::processRegWriteResponse(const FpgaReg *pFromFpga)
{
	asynStatus status = asynSuccess;
	epicsInt32 valueSet[maxMsgSize/sizeof(FpgaReg)]; // Put the value sent to the FPGA here for comparison
//  variables that may be useful for checking array data
//	asynUser *pAsynArrayUser;
//	unsigned int i;

	/* Map address to parameter, set the parameter in the parameter library. */
	switch (pFromFpga->addr)
    {
    case DspChanKeepAdr:
		status = (asynStatus) getIntegerParam(p_DspChanKeep, valueSet);
		if( (valueSet[0] & DspChanKeepMask) == (pFromFpga->data & DspChanKeepMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				DspChanKeepString, (unsigned ) pFromFpga->data & DspChanKeepMask);
		else
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				DspChanKeepString, valueSet[0] & DspChanKeepMask, (unsigned ) pFromFpga->data & DspChanKeepMask);

		break;
    case DspFdbkCoreCoarseScaleAdr:
		status = (asynStatus) getIntegerParam(p_DspFdbkCoreCoarseScale, valueSet);
		if( (valueSet[0] & DspFdbkCoreCoarseScaleMask) == (pFromFpga->data & DspFdbkCoreCoarseScaleMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				DspFdbkCoreCoarseScaleString, (unsigned ) pFromFpga->data & DspFdbkCoreCoarseScaleMask);
		else
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				DspFdbkCoreCoarseScaleString, valueSet[0] & DspFdbkCoreCoarseScaleMask, (unsigned ) pFromFpga->data & DspFdbkCoreCoarseScaleMask);

		break;
    case DspFdbkCoreMpProcPhOffsetAdr:
		status = (asynStatus) getIntegerParam(p_DspFdbkCoreMpProcPhOffset, valueSet);
		if( (valueSet[0] & DspFdbkCoreMpProcPhOffsetMask) == (pFromFpga->data & DspFdbkCoreMpProcPhOffsetMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				DspFdbkCoreMpProcPhOffsetString, (unsigned ) pFromFpga->data & DspFdbkCoreMpProcPhOffsetMask);
		else
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				DspFdbkCoreMpProcPhOffsetString, valueSet[0] & DspFdbkCoreMpProcPhOffsetMask, (unsigned ) pFromFpga->data & DspFdbkCoreMpProcPhOffsetMask);

		break;
    case DspFdbkCoreMpProcSelEnAdr:
		status = (asynStatus) getIntegerParam(p_DspFdbkCoreMpProcSelEn, valueSet);
		if( (valueSet[0] & DspFdbkCoreMpProcSelEnMask) == (pFromFpga->data & DspFdbkCoreMpProcSelEnMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				DspFdbkCoreMpProcSelEnString, (unsigned ) pFromFpga->data & DspFdbkCoreMpProcSelEnMask);
		else
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				DspFdbkCoreMpProcSelEnString, valueSet[0] & DspFdbkCoreMpProcSelEnMask, (unsigned ) pFromFpga->data & DspFdbkCoreMpProcSelEnMask);

		break;
    case DspFdbkCoreMpProcSelThreshAdr:
		status = (asynStatus) getIntegerParam(p_DspFdbkCoreMpProcSelThresh, valueSet);
		if( (valueSet[0] & DspFdbkCoreMpProcSelThreshMask) == (pFromFpga->data & DspFdbkCoreMpProcSelThreshMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				DspFdbkCoreMpProcSelThreshString, (unsigned ) pFromFpga->data & DspFdbkCoreMpProcSelThreshMask);
		else
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				DspFdbkCoreMpProcSelThreshString, valueSet[0] & DspFdbkCoreMpProcSelThreshMask, (unsigned ) pFromFpga->data & DspFdbkCoreMpProcSelThreshMask);

		break;
    case DspModuloAdr:
		status = (asynStatus) getIntegerParam(p_DspModulo, valueSet);
		if( (valueSet[0] & DspModuloMask) == (pFromFpga->data & DspModuloMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				DspModuloString, (unsigned ) pFromFpga->data & DspModuloMask);
		else
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				DspModuloString, valueSet[0] & DspModuloMask, (unsigned ) pFromFpga->data & DspModuloMask);

		break;
    case DspPhaseStepAdr:
		status = (asynStatus) getIntegerParam(p_DspPhaseStep, valueSet);
		if( (valueSet[0] & DspPhaseStepMask) == (pFromFpga->data & DspPhaseStepMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				DspPhaseStepString, (unsigned ) pFromFpga->data & DspPhaseStepMask);
		else
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				DspPhaseStepString, valueSet[0] & DspPhaseStepMask, (unsigned ) pFromFpga->data & DspPhaseStepMask);

		break;
    case DspPiezoPiezoDcAdr:
		status = (asynStatus) getIntegerParam(p_DspPiezoPiezoDc, valueSet);
		if( (valueSet[0] & DspPiezoPiezoDcMask) == (pFromFpga->data & DspPiezoPiezoDcMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				DspPiezoPiezoDcString, (unsigned ) pFromFpga->data & DspPiezoPiezoDcMask);
		else
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				DspPiezoPiezoDcString, valueSet[0] & DspPiezoPiezoDcMask, (unsigned ) pFromFpga->data & DspPiezoPiezoDcMask);

		break;
    case DspTagAdr:
		status = (asynStatus) getIntegerParam(p_DspTag, valueSet);
		if( (valueSet[0] & DspTagMask) == (pFromFpga->data & DspTagMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				DspTagString, (unsigned ) pFromFpga->data & DspTagMask);
		else
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				DspTagString, valueSet[0] & DspTagMask, (unsigned ) pFromFpga->data & DspTagMask);

		break;
    case DspWaveSampPerAdr:
		status = (asynStatus) getIntegerParam(p_DspWaveSampPer, valueSet);
		if( (valueSet[0] & DspWaveSampPerMask) == (pFromFpga->data & DspWaveSampPerMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				DspWaveSampPerString, (unsigned ) pFromFpga->data & DspWaveSampPerMask);
		else
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				DspWaveSampPerString, valueSet[0] & DspWaveSampPerMask, (unsigned ) pFromFpga->data & DspWaveSampPerMask);

		break;
    case DspWaveShiftAdr:
		status = (asynStatus) getIntegerParam(p_DspWaveShift, valueSet);
		if( (valueSet[0] & DspWaveShiftMask) == (pFromFpga->data & DspWaveShiftMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				DspWaveShiftString, (unsigned ) pFromFpga->data & DspWaveShiftMask);
		else
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				DspWaveShiftString, valueSet[0] & DspWaveShiftMask, (unsigned ) pFromFpga->data & DspWaveShiftMask);

		break;
	default:
		status = asynError;
		break;
    }

	// TODO: handle arrays

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


extern "C" {

/* Configuration routine.  Called directly, or from the iocsh function below */

/** EPICS iocsh callable function to call constructor for the scllrfAsynPortDriver class.
 * \param[in] portName The name of the asyn port driver to be created.
 * \param[in] netPortName The name of the asynIPport this will use to communicate */
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

