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

    /* Create the epicsMutex for locking access to communications counters from other threads */
    comCountersMutexId_ = epicsMutexCreate();
    if (!comCountersMutexId_) {
        printf("%s ERROR: epicsMutexCreate failure\n", __PRETTY_FUNCTION__);
        return;
    }
    epicsThreadSleep(defaultPollPeriod);

	createParam(RunStopString, asynParamInt32, &p_RunStop);
	createParam(MaxParallelRequestsString, asynParamInt32,
			&p_MaxParallelRequests);
	createParam(PollPeriodString, asynParamFloat64, &p_PollPeriod);
	createParam(CommErrorCountString, asynParamInt32, &p_CommErrorCount);

	// Set these early, consider adding interlock for race condition with polling loop.
    setIntegerParam(p_RunStop, stop);
    setIntegerParam(p_MaxParallelRequests, defaultMaxParallelRequests);
    setDoubleParam(p_PollPeriod, defaultPollPeriod);
    setIntegerParam(p_CommErrorCount, 0);

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
	createParam(GitSHA1String, asynParamOctet, &p_GitSHA1);
	createParam(DspFdbkCoreMpProcCoeffString, asynParamInt32Array,
			&p_DspFdbkCoreMpProcCoeff);
	createParam(DspFdbkCoreMpProcLimString, asynParamInt32Array,
			&p_DspFdbkCoreMpProcLim);
	createParam(DspFdbkCoreMpProcSetmpString, asynParamInt32Array,
			&p_DspFdbkCoreMpProcSetmp);
	createParam(DspLpNotchLp1AKxString, asynParamInt32Array,
			&p_DspLpNotchLp1AKx);
	createParam(DspLpNotchLp1AKyString, asynParamInt32Array,
			&p_DspLpNotchLp1AKy);
	createParam(DspLpNotchLp1BKxString, asynParamInt32Array,
			&p_DspLpNotchLp1BKx);
	createParam(DspLpNotchLp1BKyString, asynParamInt32Array,
			&p_DspLpNotchLp1BKy);
	createParam(DspChanKeepString, asynParamInt32, &p_DspChanKeep);
	createParam(DspFdbkCoreCoarseScaleString, asynParamInt32,
			&p_DspFdbkCoreCoarseScale);
	createParam(DspFdbkCoreMpProcPhOffsetString, asynParamInt32,
			&p_DspFdbkCoreMpProcPhOffset);
	createParam(DspFdbkCoreMpProcSelEnString, asynParamInt32,
			&p_DspFdbkCoreMpProcSelEn);
	createParam(DspFdbkCoreMpProcSelThreshString, asynParamInt32,
			&p_DspFdbkCoreMpProcSelThresh);
	createParam(DspModuloString, asynParamInt32, &p_DspModulo);
	createParam(DspPhaseStepString, asynParamInt32, &p_DspPhaseStep);
	createParam(DspPiezoPiezoDcString, asynParamInt32, &p_DspPiezoPiezoDc);
	createParam(DspTagString, asynParamInt32, &p_DspTag);
	createParam(DspWaveSampPerString, asynParamInt32, &p_DspWaveSampPer);
	createParam(DspWaveShiftString, asynParamInt32, &p_DspWaveShift);
	createParam(Waveform1String, asynParamInt32Array, &p_Waveform);
	createParam(CircleAwString, asynParamInt32, &p_CircleAw);
	createParam(ModeCountString, asynParamInt32, &p_ModeCount);
	createParam(ModeShiftString, asynParamInt32, &p_ModeShift);
	createParam(NMechModesString, asynParamInt32, &p_NMechModes);
	createParam(DfScaleString, asynParamInt32, &p_DfScale);
	createParam(SimpleDemoString, asynParamInt32, &p_SimpleDemo);
	createParam(Cav4MechNoiseCplKOutString, asynParamInt32Array,
			&p_Cav4MechNoiseCplKOut);
	createParam(Cav4MechPiezoCplKOutString, asynParamInt32Array,
			&p_Cav4MechPiezoCplKOut);
	createParam(Cav4MechResonatorPropConstString, asynParamInt32Array,
			&p_Cav4MechResonatorPropConst);
	createParam(StaCav4ElecDot0KOutString, asynParamInt32Array,
			&p_StaCav4ElecDot0KOut);
	createParam(StaCav4ElecDot1KOutString, asynParamInt32Array,
			&p_StaCav4ElecDot1KOut);
	createParam(StaCav4ElecDot2KOutString, asynParamInt32Array,
			&p_StaCav4ElecDot2KOut);
	createParam(StaCav4ElecOuterProd0KOutString, asynParamInt32Array,
			&p_StaCav4ElecOuterProd0KOut);
	createParam(StaCav4ElecOuterProd1KOutString, asynParamInt32Array,
			&p_StaCav4ElecOuterProd1KOut);
	createParam(StaCav4ElecOuterProd2KOutString, asynParamInt32Array,
			&p_StaCav4ElecOuterProd2KOut);
	createParam(StaCav4ElecDrvCplOutCplngString, asynParamInt32,
			&p_StaCav4ElecDrvCplOutCplng);
	createParam(StaCav4ElecDrvCplOutPhOffString, asynParamInt32,
			&p_StaCav4ElecDrvCplOutPhOff);
	createParam(BeamModuloString, asynParamInt32, &p_BeamModulo);
	createParam(BeamPhaseStepString, asynParamInt32, &p_BeamPhaseStep);
	createParam(Cav4MechPrngIvaString, asynParamInt32, &p_Cav4MechPrngIva);
	createParam(Cav4MechPrngIvbString, asynParamInt32, &p_Cav4MechPrngIvb);
	createParam(Cav4MechPrngRandomRunString, asynParamInt32,
			&p_Cav4MechPrngRandomRun);
	createParam(StaACavOffsetString, asynParamInt32, &p_StaACavOffset);
	createParam(StaAForOffsetString, asynParamInt32, &p_StaAForOffset);
	createParam(StaARflOffsetString, asynParamInt32, &p_StaARflOffset);
	createParam(StaAmpLpBwString, asynParamInt32, &p_StaAmpLpBw);
	createParam(StaCav4ElecFreq0CoarseFreqString, asynParamInt32,
			&p_StaCav4ElecFreq0CoarseFreq);
	createParam(StaCav4ElecFreq1CoarseFreqString, asynParamInt32,
			&p_StaCav4ElecFreq1CoarseFreq);
	createParam(StaCav4ElecFreq2CoarseFreqString, asynParamInt32,
			&p_StaCav4ElecFreq2CoarseFreq);
	createParam(StaCav4ElecMode0BeamCplString, asynParamInt32,
			&p_StaCav4ElecMode0BeamCpl);
	createParam(StaCav4ElecMode0BwString, asynParamInt32,
			&p_StaCav4ElecMode0Bw);
	createParam(StaCav4ElecMode0DrvCplString, asynParamInt32,
			&p_StaCav4ElecMode0DrvCpl);
	createParam(StaCav4ElecMode0OutCplOutCplngString, asynParamInt32Array,
			&p_StaCav4ElecMode0OutCplOutCplng);
	createParam(StaCav4ElecMode0OutCplOutPhOffString, asynParamInt32,
			&p_StaCav4ElecMode0OutCplOutPhOff);
	createParam(StaCav4ElecMode1BeamCplString, asynParamInt32,
			&p_StaCav4ElecMode1BeamCpl);
	createParam(StaCav4ElecMode1BwString, asynParamInt32,
			&p_StaCav4ElecMode1Bw);
	createParam(StaCav4ElecMode1DrvCplString, asynParamInt32,
			&p_StaCav4ElecMode1DrvCpl);
	createParam(StaCav4ElecMode2BeamCplString, asynParamInt32,
			&p_StaCav4ElecMode2BeamCpl);
	createParam(StaCav4ElecMode2BwString, asynParamInt32,
			&p_StaCav4ElecMode2Bw);
	createParam(StaCav4ElecMode2DrvCplString, asynParamInt32,
			&p_StaCav4ElecMode2DrvCpl);
	createParam(StaCav4ElecModuloString, asynParamInt32, &p_StaCav4ElecModulo);
	createParam(StaCav4ElecPhaseStepString, asynParamInt32,
			&p_StaCav4ElecPhaseStep);
	createParam(StaComprSatCtlString, asynParamInt32, &p_StaComprSatCtl);
	createParam(StaPrngIvaString, asynParamInt32, &p_StaPrngIva);
	createParam(StaPrngIvbString, asynParamInt32, &p_StaPrngIvb);
	createParam(StaPrngRandomRunString, asynParamInt32, &p_StaPrngRandomRun);
	createParam(StaCav4ElecMode1OutCplOutCplngString, asynParamInt32,
			&p_StaCav4ElecMode1OutCplOutCplng);
	createParam(StaCav4ElecMode1OutCplOutPhOffString, asynParamInt32,
			&p_StaCav4ElecMode1OutCplOutPhOff);
	createParam(StaCav4ElecMode2OutCplOutCplngString, asynParamInt32,
			&p_StaCav4ElecMode2OutCplOutCplng);
	createParam(StaCav4ElecMode2OutCplOutPhOffString, asynParamInt32,
			&p_StaCav4ElecMode2OutCplOutPhOff);

    epicsThreadSleep(defaultPollPeriod);

    printf("%s created %ld parameters.\n",__PRETTY_FUNCTION__,NUM_CMOC_PARAMS);

	status=pasynCommonSyncIO->connect(netPortName, 0, &pCommonAsynUser_, 0);
	if(status!=asynSuccess)
		printf( "%s: connect: failed to connect to port %s with status %d\n",
				__PRETTY_FUNCTION__,netPortName, status);
	else  printf( "%s: connect: %s asynUser \"Common\" connected to port %s\n",__PRETTY_FUNCTION__, drvPortName, netPortName);

	status=pasynOctetSyncIO->connect( netPortName,0,&pOctetAsynUser_,0);
	if(status!=asynSuccess)
		printf( "%s: connect: failed to connect to Read port %s with status %d\n",
				__PRETTY_FUNCTION__,netPortName, status);
	else  printf( "%s: connect: %s asynUser \"Octet\" connected to port %s\n",__PRETTY_FUNCTION__, drvPortName,netPortName);

    // The createParam calls are queued up rather than executed right away, so
    // give them a chance to be created before we start writing to them.
    // TODO: Is there a way to check that the params were finished being created?
    epicsThreadSleep(defaultPollPeriod);

	readEventId_ = epicsEventMustCreate(epicsEventEmpty);
	startResponseHandler();

	reqWaveEventId_ = epicsEventMustCreate(epicsEventEmpty);
	startWaveformRequester();

    epicsThreadSleep(defaultPollPeriod);

	pollEventId_ = epicsEventMustCreate(epicsEventEmpty);
	startPoller(defaultPollPeriod);

    printf("%s %s initialized and threads started.\n",__PRETTY_FUNCTION__, drvPortName);
}

scllrfAsynPortDriver::~scllrfAsynPortDriver()
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


/** Called when you have the asyn parameter name and want the corresponding
 * register address.
  * \param[in] function From pAsynUser->reason, corresponding to a registered parameter.
  * \param[in] pToFpga Pointer to the {Address, Data} structure where the register address will be written
  * \param[in] nElements Number of elements to read. */
asynStatus scllrfAsynPortDriver::functionToRegister(const int function,
		FpgaReg *pToFpga)
{
	asynStatus status = asynSuccess;

	// Assume this is for a write function, doesn't have to include read only registers (so far)
	if (function == p_Magic)
	{
		pToFpga->addr = MagicAdr | flagReadMask;
	}
	else if (function == p_DspFlavor)
	{
		pToFpga->addr = DspFlavorAdr | flagReadMask;
	}
	else if (function == p_BuildYear)
	{
		pToFpga->addr = BuildYearAdr | flagReadMask;
	}
	else if (function == p_BuildMonth)
	{
		pToFpga->addr = BuildMonthAdr | flagReadMask;
	}
	else if (function == p_BuildDay)
	{
		pToFpga->addr = BuildDayAdr | flagReadMask;
	}
	else if (function == p_BuildHour)
	{
		pToFpga->addr = BuildHourAdr | flagReadMask;
	}
	else if (function == p_BuildMinute)
	{
		pToFpga->addr = BuildMinuteAdr | flagReadMask;
	}
	else if (function == p_CodeIsClean)
	{
		pToFpga->addr = CodeIsCleanAdr | flagReadMask;
	}
	else if (function == p_ToolRev)
	{
		pToFpga->addr = ToolRevAdr | flagReadMask;
	}
	else if (function == p_User)
	{
		pToFpga->addr = UserAdr | flagReadMask;
	}
	else if (function == p_BoardType)
	{
		pToFpga->addr = BoardTypeAdr | flagReadMask;
	}
	else if (function == p_Version)
	{
		pToFpga->addr = VersionAdr | flagReadMask;
	}
	else if (function == p_GitSHA1a)
	{
		pToFpga->addr = GitSHA1aAdr | flagReadMask;
	}
	else if (function == p_GitSHA1b)
	{
		pToFpga->addr = GitSHA1bAdr | flagReadMask;
	}
	else if (function == p_GitSHA1c)
	{
		pToFpga->addr = GitSHA1cAdr | flagReadMask;
	}
	else if (function == p_GitSHA1d)
	{
		pToFpga->addr = GitSHA1dAdr | flagReadMask;
	}
	else if (function == p_GitSHA1e)
	{
		pToFpga->addr = GitSHA1eAdr | flagReadMask;
	}
	else if (function == p_GitSHA1f)
	{
		pToFpga->addr = GitSHA1fAdr | flagReadMask;
	}
	else if (function == p_GitSHA1g)
	{
		pToFpga->addr = GitSHA1gAdr | flagReadMask;
	}
	else if (function == p_GitSHA1h)
	{
		pToFpga->addr = GitSHA1hAdr | flagReadMask;
	}
	else if (function == p_GitSHA1i)
	{
		pToFpga->addr = GitSHA1iAdr | flagReadMask;
	}
	else if (function == p_GitSHA1j)
	{
		pToFpga->addr = GitSHA1jAdr | flagReadMask;
	}
	else if (function == p_GitSHA1k)
	{
		pToFpga->addr = GitSHA1kAdr | flagReadMask;
	}
	else if (function == p_GitSHA1l)
	{
		pToFpga->addr = GitSHA1lAdr | flagReadMask;
	}
	else if (function == p_GitSHA1m)
	{
		pToFpga->addr = GitSHA1mAdr | flagReadMask;
	}
	else if (function == p_GitSHA1n)
	{
		pToFpga->addr = GitSHA1nAdr | flagReadMask;
	}
	else if (function == p_GitSHA1o)
	{
		pToFpga->addr = GitSHA1oAdr | flagReadMask;
	}
	else if (function == p_GitSHA1p)
	{
		pToFpga->addr = GitSHA1pAdr | flagReadMask;
	}
	else if (function == p_GitSHA1q)
	{
		pToFpga->addr = GitSHA1qAdr | flagReadMask;
	}
	else if (function == p_GitSHA1r)
	{
		pToFpga->addr = GitSHA1rAdr | flagReadMask;
	}
	else if (function == p_GitSHA1s)
	{
		pToFpga->addr = GitSHA1sAdr | flagReadMask;
	}
	else if (function == p_GitSHA1t)
	{
		pToFpga->addr = GitSHA1tAdr | flagReadMask;
	}
	else if (function == p_DspFdbkCoreMpProcCoeff)
	{
		pToFpga->addr = DspFdbkCoreMpProcCoeffAdr;
	}
	else if (function == p_DspFdbkCoreMpProcLim)
	{
		pToFpga->addr = DspFdbkCoreMpProcLimAdr;
	}
	else if (function == p_DspFdbkCoreMpProcSetmp)
	{
		pToFpga->addr = DspFdbkCoreMpProcSetmpAdr;
	}
	else if (function == p_DspLpNotchLp1AKx)
	{
		pToFpga->addr = DspLpNotchLp1AKxAdr;
	}
	else if (function == p_DspLpNotchLp1AKy)
	{
		pToFpga->addr = DspLpNotchLp1AKyAdr;
	}
	else if (function == p_DspLpNotchLp1BKx)
	{
		pToFpga->addr = DspLpNotchLp1BKxAdr;
	}
	else if (function == p_DspLpNotchLp1BKy)
	{
		pToFpga->addr = DspLpNotchLp1BKyAdr;
	}
	else if (function == p_DspChanKeep)
	{
		pToFpga->addr = DspChanKeepAdr;
	}
	else if (function == p_DspFdbkCoreCoarseScale)
	{
		pToFpga->addr = DspFdbkCoreCoarseScaleAdr;
	}
	else if (function == p_DspFdbkCoreMpProcPhOffset)
	{
		pToFpga->addr = DspFdbkCoreMpProcPhOffsetAdr;
	}
	else if (function == p_DspFdbkCoreMpProcSelEn)
	{
		pToFpga->addr = DspFdbkCoreMpProcSelEnAdr;
	}
	else if (function == p_DspFdbkCoreMpProcSelThresh)
	{
		pToFpga->addr = DspFdbkCoreMpProcSelThreshAdr;
	}
	else if (function == p_DspModulo)
	{
		pToFpga->addr = DspModuloAdr;
	}
	else if (function == p_DspPhaseStep)
	{
		pToFpga->addr = DspPhaseStepAdr;
	}
	else if (function == p_DspPiezoPiezoDc)
	{
		pToFpga->addr = DspPiezoPiezoDcAdr;
	}
	else if (function == p_DspTag)
	{
		pToFpga->addr = DspTagAdr;
	}
	else if (function == p_DspWaveSampPer)
	{
		pToFpga->addr = DspWaveSampPerAdr;
	}
	else if (function == p_DspWaveShift)
	{
		pToFpga->addr = DspWaveShiftAdr;
	}
	else if (function == p_CircleAw)
	{
		pToFpga->addr = CircleAwAdr | flagReadMask;
	}
	else if (function == p_ModeCount)
	{
		pToFpga->addr = ModeCountAdr | flagReadMask;
	}
	else if (function == p_ModeShift)
	{
		pToFpga->addr = ModeShiftAdr | flagReadMask;
	}
	else if (function == p_NMechModes)
	{
		pToFpga->addr = NMechModesAdr | flagReadMask;
	}
	else if (function == p_DfScale)
	{
		pToFpga->addr = DfScaleAdr | flagReadMask;
	}
	else if (function == p_SimpleDemo)
	{
		pToFpga->addr = SimpleDemoAdr | flagReadMask;
	}
    else if( function == p_Cav4MechNoiseCplKOut )
	{
		pToFpga->addr = Cav4MechNoiseCplKOutAdr;
	}
    else if( function == p_Cav4MechPiezoCplKOut )
	{
		pToFpga->addr = Cav4MechPiezoCplKOutAdr;
	}
	else if (function == p_Cav4MechResonatorPropConst)
	{
		pToFpga->addr = Cav4MechResonatorPropConstAdr;
	}
    else if( function == p_StaCav4ElecDot0KOut )
	{
		pToFpga->addr = StaCav4ElecDot0KOutAdr;
	}
    else if( function == p_StaCav4ElecDot1KOut )
	{
		pToFpga->addr = StaCav4ElecDot1KOutAdr;
	}
    else if( function == p_StaCav4ElecDot2KOut )
	{
		pToFpga->addr = StaCav4ElecDot2KOutAdr;
	}
    else if( function == p_StaCav4ElecOuterProd0KOut )
	{
		pToFpga->addr = StaCav4ElecOuterProd0KOutAdr;
	}
    else if( function == p_StaCav4ElecOuterProd1KOut )
	{
		pToFpga->addr = StaCav4ElecOuterProd1KOutAdr;
	}
    else if( function == p_StaCav4ElecOuterProd2KOut )
	{
		pToFpga->addr = StaCav4ElecOuterProd2KOutAdr;
	}
    else if( function == p_StaCav4ElecDrvCplOutCplng )
	{
		pToFpga->addr = StaCav4ElecDrvCplOutCplngAdr;
	}
    else if( function == p_StaCav4ElecDrvCplOutPhOff )
	{
		pToFpga->addr = StaCav4ElecDrvCplOutPhOffAdr;
	}
	else if (function == p_BeamModulo)
	{
		pToFpga->addr = BeamModuloAdr;
	}
	else if (function == p_BeamPhaseStep)
	{
		pToFpga->addr = BeamPhaseStepAdr;
	}
	else if (function == p_Cav4MechPrngIva)
	{
		pToFpga->addr = Cav4MechPrngIvaAdr;
	}
	else if (function == p_Cav4MechPrngIvb)
	{
		pToFpga->addr = Cav4MechPrngIvbAdr;
	}
	else if (function == p_Cav4MechPrngRandomRun)
	{
		pToFpga->addr = Cav4MechPrngRandomRunAdr;
	}
    else if( function == p_StaACavOffset )
	{
		pToFpga->addr = StaACavOffsetAdr;
	}
    else if( function == p_StaAForOffset )
	{
		pToFpga->addr = StaAForOffsetAdr;
	}
    else if( function == p_StaARflOffset )
	{
		pToFpga->addr = StaARflOffsetAdr;
	}
    else if( function == p_StaAmpLpBw )
	{
		pToFpga->addr = StaAmpLpBwAdr;
	}
    else if( function == p_StaCav4ElecFreq0CoarseFreq )
	{
		pToFpga->addr = StaCav4ElecFreq0CoarseFreqAdr;
	}
    else if( function == p_StaCav4ElecFreq1CoarseFreq )
	{
		pToFpga->addr = StaCav4ElecFreq1CoarseFreqAdr;
	}
    else if( function == p_StaCav4ElecFreq2CoarseFreq )
	{
		pToFpga->addr = StaCav4ElecFreq2CoarseFreqAdr;
	}
    else if( function == p_StaCav4ElecMode0BeamCpl )
	{
		pToFpga->addr = StaCav4ElecMode0BeamCplAdr;
	}
    else if( function == p_StaCav4ElecMode0Bw )
	{
		pToFpga->addr = StaCav4ElecMode0BwAdr;
	}
    else if( function == p_StaCav4ElecMode0DrvCpl )
	{
		pToFpga->addr = StaCav4ElecMode0DrvCplAdr;
	}
    else if( function == p_StaCav4ElecMode0OutCplOutCplng )
	{
		pToFpga->addr = StaCav4ElecMode0OutCplOutCplngAdr;
	}
    else if( function == p_StaCav4ElecMode0OutCplOutPhOff )
	{
		pToFpga->addr = StaCav4ElecMode0OutCplOutPhOffAdr;
	}
    else if( function == p_StaCav4ElecMode1BeamCpl )
	{
		pToFpga->addr = StaCav4ElecMode1BeamCplAdr;
	}
    else if( function == p_StaCav4ElecMode1Bw )
	{
		pToFpga->addr = StaCav4ElecMode1BwAdr;
	}
    else if( function == p_StaCav4ElecMode1DrvCpl )
	{
		pToFpga->addr = StaCav4ElecMode1DrvCplAdr;
	}
    else if( function == p_StaCav4ElecMode2BeamCpl )
	{
		pToFpga->addr = StaCav4ElecMode2BeamCplAdr;
	}
    else if( function == p_StaCav4ElecMode2Bw )
	{
		pToFpga->addr = StaCav4ElecMode2BwAdr;
	}
    else if( function == p_StaCav4ElecMode2DrvCpl )
	{
		pToFpga->addr = StaCav4ElecMode2DrvCplAdr;
	}
    else if( function == p_StaCav4ElecModulo )
	{
		pToFpga->addr = StaCav4ElecModuloAdr;
	}
    else if( function == p_StaCav4ElecPhaseStep )
	{
		pToFpga->addr = StaCav4ElecPhaseStepAdr;
	}
    else if( function == p_StaComprSatCtl )
	{
		pToFpga->addr = StaComprSatCtlAdr;
	}
    else if( function == p_StaPrngIva )
	{
		pToFpga->addr = StaPrngIvaAdr;
	}
    else if( function == p_StaPrngIvb )
	{
		pToFpga->addr = StaPrngIvbAdr;
	}
    else if( function == p_StaPrngRandomRun )
	{
		pToFpga->addr = StaPrngRandomRunAdr|flagReadMask;
	}
    else if( function == p_StaCav4ElecMode1OutCplOutCplng )
	{
		pToFpga->addr = StaCav4ElecMode1OutCplOutCplngAdr;
	}
    else if( function == p_StaCav4ElecMode1OutCplOutPhOff )
	{
		pToFpga->addr = StaCav4ElecMode1OutCplOutPhOffAdr|flagReadMask;
	}
    else if( function == p_StaCav4ElecMode2OutCplOutCplng )
	{
		pToFpga->addr = StaCav4ElecMode2OutCplOutCplngAdr;
	}
    else if( function == p_StaCav4ElecMode2OutCplOutPhOff )
	{
		pToFpga->addr = StaCav4ElecMode2OutCplOutPhOffAdr;
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
    	if (status == asynSuccess) // Yes, this function is a register write
    	{
    		regSendBuf[1].data = (uint32_t) value;
        	htonFpgaRegArray(regSendBuf, 2);
        	sendRegRequest(regSendBuf, 2);
    	}
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
	unsigned int i, uOutBuffIndex;
	uint32_t uRegAddr;
    const char *paramName;

	//getIntegerParam(P_ArrayLength, &nCopy);
	//if ((int) nElements < nCopy)
	//	nCopy = (int) nElements;

    /* Fetch the parameter string name for possible use in debugging */
    getParamName(function, &paramName);

	if(nElements < 2)
	{
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, "%s: array with %d elements, function=%d, %s\n",
				__PRETTY_FUNCTION__, (int) nElements, function, paramName);
		return status;
	}
	else
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, "%s: array with %d elements, function=%d, %s\n",
				__PRETTY_FUNCTION__, (int) nElements, function, paramName);

	status = functionToRegister(function, &regSendBuf[1]);
	if (status != asynSuccess)
		return status;

	uRegAddr = regSendBuf[1].addr;
	uOutBuffIndex = 1; // index of first register past the nonce

	for(i=0; i<nElements; ++i, ++uOutBuffIndex)
	{
		regSendBuf[uOutBuffIndex].data = (int32_t) value[i];
		regSendBuf[uOutBuffIndex].addr = (uint32_t) uRegAddr + i;

		// If there's more to send than will fit in the max message size, break
		// it up into chunks and send each chunk individually
		if(uOutBuffIndex == maxMsgSize/sizeof(FpgaReg)-1)
		{
			htonFpgaRegArray(regSendBuf, uOutBuffIndex);
			sendRegRequest(regSendBuf, uOutBuffIndex+1);

			if (status)
				epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize,
						"%s: status=%d, function=%d, sending array segment %u of %s", __PRETTY_FUNCTION__,
						status, function, i/uOutBuffIndex +1, paramName);
			else
				asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, "%s: function=%d, sending array segment %u of %s\n",
						__PRETTY_FUNCTION__, function, i/uOutBuffIndex +1, paramName);
			uOutBuffIndex = 0; // loop will increment to index of first register past the nonce
		}
	}

	htonFpgaRegArray(regSendBuf, uOutBuffIndex);
	sendRegRequest(regSendBuf, uOutBuffIndex+1);

	if (status)
		epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize,
				"%s: status=%d, function=%d, sent %s", __PRETTY_FUNCTION__,
				status, function, paramName);
	else
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, "%s: function=%d, sent %s\n",
				__PRETTY_FUNCTION__, function, paramName);

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
	static FpgaReg cmocReadAllRegMsg[readRegCount + 1] =
	{
			{ 0, 0 },
			{ flagReadMask | MagicAdr, blankData | wavesReadyMask },
			{ flagReadMask | DspFlavorAdr, blankData },
			{ flagReadMask | BuildYearAdr, blankData },
			{ flagReadMask | BuildMonthAdr, blankData },
			{ flagReadMask | BuildDayAdr, blankData },
			{ flagReadMask | BuildHourAdr, blankData },
			{ flagReadMask | BuildMinuteAdr, blankData },
			{ flagReadMask | CodeIsCleanAdr, blankData },
			{ flagReadMask | ToolRevAdr, blankData },
			{ flagReadMask | UserAdr, blankData },
			{ flagReadMask | BoardTypeAdr, blankData },
			{ flagReadMask | VersionAdr, blankData },
			{ flagReadMask | GitSHA1aAdr, blankData },
			{ flagReadMask | GitSHA1bAdr, blankData },
			{ flagReadMask | GitSHA1cAdr, blankData },
			{ flagReadMask | GitSHA1dAdr, blankData },
			{ flagReadMask | GitSHA1eAdr, blankData },
			{ flagReadMask | GitSHA1fAdr, blankData },
			{ flagReadMask | GitSHA1gAdr, blankData },
			{ flagReadMask | GitSHA1hAdr, blankData },
			{ flagReadMask | GitSHA1iAdr, blankData },
			{ flagReadMask | GitSHA1jAdr, blankData },
			{ flagReadMask | GitSHA1kAdr, blankData },
			{ flagReadMask | GitSHA1lAdr, blankData },
			{ flagReadMask | GitSHA1mAdr, blankData },
			{ flagReadMask | GitSHA1nAdr, blankData },
			{ flagReadMask | GitSHA1oAdr, blankData },
			{ flagReadMask | GitSHA1pAdr, blankData },
			{ flagReadMask | GitSHA1qAdr, blankData },
			{ flagReadMask | GitSHA1rAdr, blankData },
			{ flagReadMask | GitSHA1sAdr, blankData },
			{ flagReadMask | GitSHA1tAdr, blankData },
			{ flagReadMask | CircleAwAdr, blankData },
			{ flagReadMask | ModeCountAdr, blankData },
			{ flagReadMask | ModeShiftAdr, blankData },
			{ flagReadMask | NMechModesAdr, blankData },
			{ flagReadMask | DfScaleAdr, blankData },
			{ flagReadMask | SimpleDemoAdr, blankData },
			{ flagReadMask | StaPrngRandomRunAdr, blankData },
			{ flagReadMask | StaCav4ElecMode1OutCplOutPhOffAdr, blankData },
	};
	htonFpgaRegArray(cmocReadAllRegMsg, readRegCount + 1);

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
		sendRegRequest(cmocReadAllRegMsg, readRegCount + 1);
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
	epicsInt32 errorCount;
	asynStatus status = asynSuccess;
	epicsMutexLockStatus mutexStatus;
	char * pWriteBuffer;
	pWriteBuffer = reinterpret_cast<char*>(regBuffer);
	int maxParallelRequests;
	getIntegerParam(p_MaxParallelRequests, &maxParallelRequests);

	// Throttle so that we don't overflow buffers if response handling falls behind
	if( netWaitingRequests_ > (unsigned) maxParallelRequests )
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: too many requests waiting for responses (%u), throttling requests.\n",__PRETTY_FUNCTION__, maxParallelRequests);
	while( netWaitingRequests_ > (unsigned) maxParallelRequests )
	{
		if (isShuttingDown_)
		{
			return asynDisconnected;
		}
		epicsThreadSleep(throttleLoopDelay);
	}

	mutexStatus = epicsMutexLock(comCountersMutexId_); // protect netSendCount and netWaitingRequests
	++netSendCount_; // increment and roll over if needed
	// use the nonce at the start of the buffer for register count, and
	// send counter. Can be used for error checking.
	regBuffer[0] = (FpgaReg)
							{ (uint32_t) htonl(netSendCount_), (int32_t) htonl(regBuffCount*sizeof(FpgaReg)) };

	status = pasynOctetSyncIO->write(pOctetAsynUser_, pWriteBuffer,
			regBuffCount*sizeof(FpgaReg), writeTimeout, &writtenCount);
	if (status != asynSuccess)
	{
		asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,"%s: failed to write. %s\n", __PRETTY_FUNCTION__,
				pOctetAsynUser_->errorMessage);
		getIntegerParam(p_CommErrorCount, &errorCount);
		setIntegerParam(p_CommErrorCount, errorCount + 1);
		callParamCallbacks();
	}
	else
	{
		++netWaitingRequests_;
	}
	epicsMutexUnlock(comCountersMutexId_);
	epicsThreadSleep(0); // to allow other threads to take the CPU
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

	for (i=0; i<waveSegmentCount; ++i)
	{
		cmocReadWaveformsMsg[i][0] = (FpgaReg) {0, 0}; // space for the nonce
	}

	for (waveSegmentNumber=0; waveSegmentNumber < waveSegmentCount; ++waveSegmentNumber)
	{
		for (waveSegmentOffset = 0; waveSegmentOffset < waveSegmentSize-1; ++waveSegmentOffset)
		{
			regAddress = wavesStart + waveSegmentNumber*(waveSegmentSize-1) + waveSegmentOffset;
			if (regAddress > wavesEnd)
			{
				lastPointIndex = waveSegmentOffset+1;
				break;
			}

			cmocReadWaveformsMsg[waveSegmentNumber][waveSegmentOffset+1] = (FpgaReg) { flagReadMask | regAddress, (int32_t) ((double) blankData * sin(regAddress/360))};
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
			for (i=0; i<waveSegmentCount - 1; ++i)
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
	epicsMutexLockStatus mutexStatus;
	static char pReadBuffer[maxMsgSize];
	FpgaReg *pRegReadback;
	size_t readCount = 0; // Number of bytes to process from the network read
	epicsInt32 errorCount;
	int eomReason;
	int noDataCounter = 0; // If we try to read this many times and get no data, give up and decrement netWaitingRequests_

	while (1)
	{

		// The asyn framework doesn't allow writes while a read is blocking,
		// so the same behavior is approximated here with events.
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: Caught up with message processing, blocking for data\n", __PRETTY_FUNCTION__);
		waitStatus = epicsEventWaitWithTimeout(readEventId_, readTimeout_);

		if (isShuttingDown_)
		{
			break;
		}
		if (waitStatus == epicsEventWaitOK) // We got an event, signaling to expect data back
		{
			while (netWaitingRequests_>0) // Writers increment atomic netWaitingRequests when they send a request
			{
				// Note: pRegReadback/pReadBuffer would have reentrancy problems, except only this one
				// thread reads it. More than one read thread and it could break.
				pRegReadback = reinterpret_cast<FpgaReg*>(pReadBuffer);
				pRegReadback[0] = (FpgaReg) {0, 0}; // Reset count, so we won't try to process data that isn't there.

				mutexStatus = epicsMutexLock(comCountersMutexId_); // protect netWaitingRequests from being modified by the write thread
				status = pasynOctetSyncIO->read(pOctetAsynUser_, pReadBuffer,
						nonceSize*sizeof(FpgaReg), 0.01, &readCount, &eomReason);

				// Prevent an endless loop here while senders are throttled
				if(readCount == 0)
				{
					++noDataCounter;
					if(noDataCounter > (int)(defaultPollPeriod/throttleLoopDelay))
						netWaitingRequests_--;
					epicsMutexUnlock(comCountersMutexId_); // protect netWaitingRequests from being modified by the write thread
					continue;
				}
				else // Get the amount of data to read from the nonce presumably read in, and read that many more bytes
				{
					noDataCounter = 0;
					if(ntohl(pRegReadback[0].data) < (int) maxMsgSize)
					{ // read in the number of bytes the nonce says to expect, starting with the second register location
						asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,"%s: read %u byte nonce, says sequence # %u with %d bytes.\n",
								__PRETTY_FUNCTION__, (unsigned) readCount, ntohl(pRegReadback[0].addr), ntohl(pRegReadback[0].data));
						status = pasynOctetSyncIO->read(pOctetAsynUser_, &pReadBuffer[nonceSize*sizeof(FpgaReg)],
								ntohl(pRegReadback[0].data)-nonceSize*sizeof(FpgaReg), 0.01, &readCount, &eomReason);
						readCount += nonceSize*sizeof(FpgaReg);
						asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,"%s: got %u bytes total for response #%u\n",
								__PRETTY_FUNCTION__, (unsigned) readCount, ntohl(pRegReadback[0].addr));
					}
					else
					{
						netWaitingRequests_--;
						asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
							"%s: presumed nonce says sequence # %u with %d bytes.\n",
							__PRETTY_FUNCTION__, ntohl(pRegReadback[0].addr), ntohl(pRegReadback[0].data));
						epicsMutexUnlock(comCountersMutexId_); // protect netWaitingRequests from being modified by the write thread
						getIntegerParam(p_CommErrorCount, &errorCount);
						setIntegerParam(p_CommErrorCount, errorCount + 1);
						callParamCallbacks();
						continue;
					}

				}
				epicsMutexUnlock(comCountersMutexId_); // protect netWaitingRequests from being modified by the write thread

				ntohFpgaRegArray(pRegReadback, (readCount)/sizeof(FpgaReg));

				processReadbackBuffer(pRegReadback, readCount);
				/* Do callbacks so higher layers see any changes */
				status = (asynStatus) callParamCallbacks();

				epicsThreadSleep(0); // sleep to be nice to other threads
			} // while (netWaitingRequests>0)
		} // if (status == epicsEventWaitOK)
		else // Timed out waiting for sender to signal. Could be a network glitch, so count request as lost.
		{
			if(netWaitingRequests_>0)
			{
				asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
						"%s: receive thread timed out after %fs waiting for %d responses. Network losses?\n", __PRETTY_FUNCTION__, readTimeout_, netWaitingRequests_);
				netWaitingRequests_--;
			}
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
	int bytesLeft = readCount; // signed to make error detection easier
	epicsInt32 errorCount;

	// Check for an invalid nonce
	if(pRegReadback[0].addr < 1 || pRegReadback[0].data < 1)
		return asynError;

	// We put the message size in the data of the first buffer element for error checking
	// and to handle multiple messages received
	while(bytesLeft > 0)
	{

		if (isShuttingDown_)
		{
			break;
		}

		// We can read partial messages, so watch array bounds if received message size is smaller than expected
		if (bytesLeft < pRegReadback[0].data)
		{
			//pRegReadback[0].data = bytesLeft; // Testing made clear this isn't enough, so just bail.
			pasynOctetSyncIO->flush(pOctetAsynUser_); // Should we clear out waiting partial messages?
			if(netWaitingRequests_ > 0)
				netWaitingRequests_--;
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
					"%s: Read %u bytes from network, %d bytes left to process, but expected %u bytes\n",
					__PRETTY_FUNCTION__, readCount, bytesLeft, (unsigned) pRegReadback[0].data);
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
					"%s: Response from request #%u, expected is request #%u\n",
					__PRETTY_FUNCTION__, (unsigned) pRegReadback[0].addr,
					lastResponseCount_ + 1);
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
					"%s: Discarding this message\n",
					__PRETTY_FUNCTION__);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
			return asynError;
		}

		if (pRegReadback[0].addr > lastResponseCount_ + 1)
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
					"%s: Missing response message, got response from request #%u, expected is request #%u\n",
					__PRETTY_FUNCTION__, (unsigned) pRegReadback[0].addr,
					lastResponseCount_ + 1);

			getIntegerParam(p_CommErrorCount, &errorCount);
			printf("setting error count to %d\n", errorCount+1);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}
		if (pRegReadback[0].addr < lastResponseCount_ + 1)
		{// NOTE: In testing so far, it has only reached here from data problems other than true out-of-order data
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
					"%s: Out-of-order response message, got response from request #%u, expected is request #%u\n",
					__PRETTY_FUNCTION__, (unsigned) pRegReadback[0].addr,
					lastResponseCount_ + 1);

			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}
		lastResponseCount_ = pRegReadback[0].addr;

		// Data read from the network may contain multiple responses. Take them one at a time.
		// Use the register count from the nonce in pRegReadback[0].data to determine how much
		// to process before the next response with its own nonce.
		for (i = 1; i < pRegReadback[0].data / sizeof(FpgaReg); ++i)
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
						"%s: status=%d, couldn't process address=0x%x, value=%u",
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
		if (pRegReadback[0].addr < netSendCount_ )
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
					"%s: processed response from request #%u, most recent is request #%u\n",
					__PRETTY_FUNCTION__, (unsigned) pRegReadback[0].addr,
					netSendCount_);
			netWaitingRequests_ = netSendCount_ - pRegReadback[0].addr;
		}
		else
			netWaitingRequests_ = 0; // We have the most recent, no outstanding requests

		// decrement number of bytes left to process by the numbr of bytes we just processed
		if(bytesLeft > pRegReadback[0].data)
			bytesLeft -= pRegReadback[0].data;
		else // guard against mangled/partial messages
			bytesLeft = 0;

		// The nonce in pRegReadback[0] contains the message size. Move pointer to the next message.
		pRegReadback = & pRegReadback[pRegReadback[0].data/sizeof(FpgaReg)];
	} // while(bytesLeft > 0)

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

asynStatus scllrfAsynPortDriver::catGitSHA1()
{
	int oneByte;
	int i;
	asynStatus status;

	strGitSHA1.str("");
	strGitSHA1.clear();
	strGitSHA1<<std::hex;

	for (i=p_GitSHA1a; i<=p_GitSHA1t; i++)
	{
		status = (asynStatus) getIntegerParam(i, &oneByte);
		strGitSHA1<< std::setw(2) << oneByte;
	}
	// used with stringin reccord, which unfortunately can only handle 19 of the 20 characters
	status = setStringParam(p_GitSHA1, strGitSHA1.str().c_str());

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
	epicsInt32 errorCount;
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
		// Once we've read the last byte of the SHA1, put them together and push it
		catGitSHA1();
		if (pFromFpga->data & wavesReadyMask)
			waveIsReady = true;
		break;
	case wavesEnd|flagReadMask:
		processWaveReadback(pFromFpga);
		// Do we need to verify that all points of all waveforms have been received, not missing
		// packets or some such?
		// Basic approach: when we read in the last point of the last waveform, publish them all
		for (i=0; i<wavesCount; ++i)
			status = doCallbacksInt32Array(pWaveform_[i], waveBufferRegCount/wavesCount, p_Waveform, i);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: got last waveform datapoint. Publishing.\n", __PRETTY_FUNCTION__);
		break;
    case CircleAwAdr|flagReadMask:
	status = (asynStatus) setIntegerParam(p_CircleAw,
			pFromFpga->data);
	asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
			"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
			CircleAwString, (unsigned ) pFromFpga->data);
		break;
    case ModeCountAdr|flagReadMask:
	status = (asynStatus) setIntegerParam(p_ModeCount,
			pFromFpga->data);
	asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
			"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
			ModeCountString, (unsigned ) pFromFpga->data);
		break;
    case ModeShiftAdr|flagReadMask:
	status = (asynStatus) setIntegerParam(p_ModeShift,
			pFromFpga->data);
	asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
			"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
			ModeShiftString, (unsigned ) pFromFpga->data);
		break;
    case NMechModesAdr|flagReadMask:
	status = (asynStatus) setIntegerParam(p_NMechModes,
			pFromFpga->data);
	asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
			"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
			NMechModesString, (unsigned ) pFromFpga->data);
		break;
    case DfScaleAdr|flagReadMask:
	status = (asynStatus) setIntegerParam(p_DfScale,
			pFromFpga->data);
	asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
			"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
			DfScaleString, (unsigned ) pFromFpga->data);
		break;
    case SimpleDemoAdr|flagReadMask:
	status = (asynStatus) setIntegerParam(p_SimpleDemo,
			pFromFpga->data);
	asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
			"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
			SimpleDemoString, (unsigned ) pFromFpga->data);
		break;
    case StaPrngRandomRunAdr|flagReadMask:
	status = (asynStatus) setIntegerParam(p_StaPrngRandomRun,
			pFromFpga->data & StaPrngRandomRunMask);
	asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
			"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
			StaPrngRandomRunString,
			(unsigned ) pFromFpga->data & StaPrngRandomRunMask);
		break;
    case StaCav4ElecMode1OutCplOutPhOffAdr|flagReadMask:
	status = (asynStatus) setIntegerParam(p_StaCav4ElecMode1OutCplOutPhOff,
			pFromFpga->data & StaCav4ElecMode1OutCplOutPhOffMask);
	asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
			"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
			StaCav4ElecMode1OutCplOutPhOffString,
			(unsigned ) pFromFpga->data & StaCav4ElecMode1OutCplOutPhOffMask);
		break;
	default:
		if( wavesStart <= (pFromFpga->addr & addrMask) && (pFromFpga->addr & addrMask) < wavesEnd )
			processWaveReadback(pFromFpga);
		else
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
			status = asynError;
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
	epicsInt32 errorCount;
//  variables that may be useful for checking array data
//	asynUser *pAsynArrayUser;
//	unsigned int i;
	assert(!(pFromFpga->addr&flagReadMask)); // This function is only for read registers

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
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				DspChanKeepString, valueSet[0] & DspChanKeepMask, (unsigned ) pFromFpga->data & DspChanKeepMask);
			status = asynError;
			setParamStatus(p_DspChanKeep, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}

		break;
    case DspFdbkCoreCoarseScaleAdr:
		status = (asynStatus) getIntegerParam(p_DspFdbkCoreCoarseScale, valueSet);
		if( (valueSet[0] & DspFdbkCoreCoarseScaleMask) == (pFromFpga->data & DspFdbkCoreCoarseScaleMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				DspFdbkCoreCoarseScaleString, (unsigned ) pFromFpga->data & DspFdbkCoreCoarseScaleMask);
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				DspFdbkCoreCoarseScaleString, valueSet[0] & DspFdbkCoreCoarseScaleMask, (unsigned ) pFromFpga->data & DspFdbkCoreCoarseScaleMask);
			status = asynError;
			setParamStatus(p_DspFdbkCoreCoarseScale, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}

		break;
    case DspFdbkCoreMpProcPhOffsetAdr:
		status = (asynStatus) getIntegerParam(p_DspFdbkCoreMpProcPhOffset, valueSet);
		if( (valueSet[0] & DspFdbkCoreMpProcPhOffsetMask) == (pFromFpga->data & DspFdbkCoreMpProcPhOffsetMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				DspFdbkCoreMpProcPhOffsetString, (unsigned ) pFromFpga->data & DspFdbkCoreMpProcPhOffsetMask);
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				DspFdbkCoreMpProcPhOffsetString, valueSet[0] & DspFdbkCoreMpProcPhOffsetMask, (unsigned ) pFromFpga->data & DspFdbkCoreMpProcPhOffsetMask);
			status = asynError;
			setParamStatus(p_DspFdbkCoreMpProcPhOffset, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}

		break;
    case DspFdbkCoreMpProcSelEnAdr:
		status = (asynStatus) getIntegerParam(p_DspFdbkCoreMpProcSelEn, valueSet);
		if( (valueSet[0] & DspFdbkCoreMpProcSelEnMask) == (pFromFpga->data & DspFdbkCoreMpProcSelEnMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				DspFdbkCoreMpProcSelEnString, (unsigned ) pFromFpga->data & DspFdbkCoreMpProcSelEnMask);
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				DspFdbkCoreMpProcSelEnString, valueSet[0] & DspFdbkCoreMpProcSelEnMask, (unsigned ) pFromFpga->data & DspFdbkCoreMpProcSelEnMask);
			status = asynError;
			setParamStatus(p_DspFdbkCoreMpProcSelEn, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}

		break;
    case DspFdbkCoreMpProcSelThreshAdr:
		status = (asynStatus) getIntegerParam(p_DspFdbkCoreMpProcSelThresh, valueSet);
		if( (valueSet[0] & DspFdbkCoreMpProcSelThreshMask) == (pFromFpga->data & DspFdbkCoreMpProcSelThreshMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				DspFdbkCoreMpProcSelThreshString, (unsigned ) pFromFpga->data & DspFdbkCoreMpProcSelThreshMask);
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				DspFdbkCoreMpProcSelThreshString, valueSet[0] & DspFdbkCoreMpProcSelThreshMask, (unsigned ) pFromFpga->data & DspFdbkCoreMpProcSelThreshMask);
			status = asynError;
			setParamStatus(p_DspFdbkCoreMpProcSelThresh, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}

		break;
    case DspModuloAdr:
		status = (asynStatus) getIntegerParam(p_DspModulo, valueSet);
		if( (valueSet[0] & DspModuloMask) == (pFromFpga->data & DspModuloMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				DspModuloString, (unsigned ) pFromFpga->data & DspModuloMask);
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				DspModuloString, valueSet[0] & DspModuloMask, (unsigned ) pFromFpga->data & DspModuloMask);
			status = asynError;
			setParamStatus(p_DspModulo, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}

		break;
    case DspPhaseStepAdr:
		status = (asynStatus) getIntegerParam(p_DspPhaseStep, valueSet);
		if( (valueSet[0] & DspPhaseStepMask) == (pFromFpga->data & DspPhaseStepMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				DspPhaseStepString, (unsigned ) pFromFpga->data & DspPhaseStepMask);
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				DspPhaseStepString, valueSet[0] & DspPhaseStepMask, (unsigned ) pFromFpga->data & DspPhaseStepMask);
			status = asynError;
			setParamStatus(p_DspPhaseStep, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}

		break;
    case DspPiezoPiezoDcAdr:
		status = (asynStatus) getIntegerParam(p_DspPiezoPiezoDc, valueSet);
		if( (valueSet[0] & DspPiezoPiezoDcMask) == (pFromFpga->data & DspPiezoPiezoDcMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				DspPiezoPiezoDcString, (unsigned ) pFromFpga->data & DspPiezoPiezoDcMask);
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				DspPiezoPiezoDcString, valueSet[0] & DspPiezoPiezoDcMask, (unsigned ) pFromFpga->data & DspPiezoPiezoDcMask);
			status = asynError;
			setParamStatus(p_DspPiezoPiezoDc, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}

		break;
    case DspTagAdr:
		status = (asynStatus) getIntegerParam(p_DspTag, valueSet);
		if( (valueSet[0] & DspTagMask) == (pFromFpga->data & DspTagMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				DspTagString, (unsigned ) pFromFpga->data & DspTagMask);
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				DspTagString, valueSet[0] & DspTagMask, (unsigned ) pFromFpga->data & DspTagMask);
			status = asynError;
			setParamStatus(p_DspTag, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}

		break;
    case DspWaveSampPerAdr:
		status = (asynStatus) getIntegerParam(p_DspWaveSampPer, valueSet);
		if( (valueSet[0] & DspWaveSampPerMask) == (pFromFpga->data & DspWaveSampPerMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				DspWaveSampPerString, (unsigned ) pFromFpga->data & DspWaveSampPerMask);
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				DspWaveSampPerString, valueSet[0] & DspWaveSampPerMask, (unsigned ) pFromFpga->data & DspWaveSampPerMask);
			status = asynError;
			setParamStatus(p_DspWaveSampPer, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}

		break;
    case DspWaveShiftAdr:
		status = (asynStatus) getIntegerParam(p_DspWaveShift, valueSet);
		if( (valueSet[0] & DspWaveShiftMask) == (pFromFpga->data & DspWaveShiftMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				DspWaveShiftString, (unsigned ) pFromFpga->data & DspWaveShiftMask);
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				DspWaveShiftString, valueSet[0] & DspWaveShiftMask, (unsigned ) pFromFpga->data & DspWaveShiftMask);
			status = asynError;
			setParamStatus(p_DspWaveShift, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}

		break;
    case StaCav4ElecDrvCplOutCplngAdr:
		status = (asynStatus) getIntegerParam(p_StaCav4ElecDrvCplOutCplng, valueSet);
		if( (valueSet[0] & StaCav4ElecDrvCplOutCplngMask) == (pFromFpga->data & StaCav4ElecDrvCplOutCplngMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				StaCav4ElecDrvCplOutCplngString, (unsigned ) pFromFpga->data & StaCav4ElecDrvCplOutCplngMask);
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				StaCav4ElecDrvCplOutCplngString, valueSet[0] & StaCav4ElecDrvCplOutCplngMask, (unsigned ) pFromFpga->data & StaCav4ElecDrvCplOutCplngMask);
			status = asynError;
			setParamStatus(p_StaCav4ElecDrvCplOutCplng, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}

		break;
    case StaCav4ElecDrvCplOutPhOffAdr:
		status = (asynStatus) getIntegerParam(p_StaCav4ElecDrvCplOutPhOff, valueSet);
		if( (valueSet[0] & StaCav4ElecDrvCplOutPhOffMask) == (pFromFpga->data & StaCav4ElecDrvCplOutPhOffMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				StaCav4ElecDrvCplOutPhOffString, (unsigned ) pFromFpga->data & StaCav4ElecDrvCplOutPhOffMask);
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				StaCav4ElecDrvCplOutPhOffString, valueSet[0] & StaCav4ElecDrvCplOutPhOffMask, (unsigned ) pFromFpga->data & StaCav4ElecDrvCplOutPhOffMask);
			status = asynError;
			setParamStatus(p_StaCav4ElecDrvCplOutPhOff, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}

		break;
    case BeamModuloAdr:
		status = (asynStatus) getIntegerParam(p_BeamModulo, valueSet);
		if( (valueSet[0] & BeamModuloMask) == (pFromFpga->data & BeamModuloMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				BeamModuloString, (unsigned ) pFromFpga->data & BeamModuloMask);
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				BeamModuloString, valueSet[0] & BeamModuloMask, (unsigned ) pFromFpga->data & BeamModuloMask);
			status = asynError;
			setParamStatus(p_BeamModulo, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}

		break;
    case BeamPhaseStepAdr:
		status = (asynStatus) getIntegerParam(p_BeamPhaseStep, valueSet);
		if( (valueSet[0] & BeamPhaseStepMask) == (pFromFpga->data & BeamPhaseStepMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				BeamPhaseStepString, (unsigned ) pFromFpga->data & BeamPhaseStepMask);
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				BeamPhaseStepString, valueSet[0] & BeamPhaseStepMask, (unsigned ) pFromFpga->data & BeamPhaseStepMask);
			status = asynError;
			setParamStatus(p_BeamPhaseStep, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}

		break;
    case Cav4MechPrngIvaAdr:
		status = (asynStatus) getIntegerParam(p_Cav4MechPrngIva, valueSet);
		if( (valueSet[0] & Cav4MechPrngIvaMask) == (pFromFpga->data & Cav4MechPrngIvaMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				Cav4MechPrngIvaString, (unsigned ) pFromFpga->data & Cav4MechPrngIvaMask);
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				Cav4MechPrngIvaString, valueSet[0] & Cav4MechPrngIvaMask, (unsigned ) pFromFpga->data & Cav4MechPrngIvaMask);
			status = asynError;
			setParamStatus(p_Cav4MechPrngIva, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}

		break;
    case Cav4MechPrngIvbAdr:
		status = (asynStatus) getIntegerParam(p_Cav4MechPrngIvb, valueSet);
		if( (valueSet[0] & Cav4MechPrngIvbMask) == (pFromFpga->data & Cav4MechPrngIvbMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				Cav4MechPrngIvbString, (unsigned ) pFromFpga->data & Cav4MechPrngIvbMask);
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				Cav4MechPrngIvbString, valueSet[0] & Cav4MechPrngIvbMask, (unsigned ) pFromFpga->data & Cav4MechPrngIvbMask);
			status = asynError;
			setParamStatus(p_Cav4MechPrngIvb, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}

		break;
    case Cav4MechPrngRandomRunAdr:
		status = (asynStatus) getIntegerParam(p_Cav4MechPrngRandomRun, valueSet);
		if( (valueSet[0] & Cav4MechPrngRandomRunMask) == (pFromFpga->data & Cav4MechPrngRandomRunMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				Cav4MechPrngRandomRunString, (unsigned ) pFromFpga->data & Cav4MechPrngRandomRunMask);
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				Cav4MechPrngRandomRunString, valueSet[0] & Cav4MechPrngRandomRunMask, (unsigned ) pFromFpga->data & Cav4MechPrngRandomRunMask);
			status = asynError;
			setParamStatus(p_Cav4MechPrngRandomRun, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}

		break;
    case StaACavOffsetAdr:
		status = (asynStatus) getIntegerParam(p_StaACavOffset, valueSet);
		if( (valueSet[0] & StaACavOffsetMask) == (pFromFpga->data & StaACavOffsetMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				StaACavOffsetString, (unsigned ) pFromFpga->data & StaACavOffsetMask);
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				StaACavOffsetString, valueSet[0] & StaACavOffsetMask, (unsigned ) pFromFpga->data & StaACavOffsetMask);
			status = asynError;
			setParamStatus(p_StaACavOffset, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}

		break;
    case StaAForOffsetAdr:
		status = (asynStatus) getIntegerParam(p_StaAForOffset, valueSet);
		if( (valueSet[0] & StaAForOffsetMask) == (pFromFpga->data & StaAForOffsetMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				StaAForOffsetString, (unsigned ) pFromFpga->data & StaAForOffsetMask);
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				StaAForOffsetString, valueSet[0] & StaAForOffsetMask, (unsigned ) pFromFpga->data & StaAForOffsetMask);
			status = asynError;
			setParamStatus(p_StaAForOffset, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}

		break;
    case StaARflOffsetAdr:
		status = (asynStatus) getIntegerParam(p_StaARflOffset, valueSet);
		if( (valueSet[0] & StaARflOffsetMask) == (pFromFpga->data & StaARflOffsetMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				StaARflOffsetString, (unsigned ) pFromFpga->data & StaARflOffsetMask);
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				StaARflOffsetString, valueSet[0] & StaARflOffsetMask, (unsigned ) pFromFpga->data & StaARflOffsetMask);
			status = asynError;
			setParamStatus(p_StaARflOffset, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}

		break;
    case StaAmpLpBwAdr:
		status = (asynStatus) getIntegerParam(p_StaAmpLpBw, valueSet);
		if( (valueSet[0] & StaAmpLpBwMask) == (pFromFpga->data & StaAmpLpBwMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				StaAmpLpBwString, (unsigned ) pFromFpga->data & StaAmpLpBwMask);
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				StaAmpLpBwString, valueSet[0] & StaAmpLpBwMask, (unsigned ) pFromFpga->data & StaAmpLpBwMask);
			status = asynError;
			setParamStatus(p_StaAmpLpBw, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}

		break;
    case StaCav4ElecFreq0CoarseFreqAdr:
		status = (asynStatus) getIntegerParam(p_StaCav4ElecFreq0CoarseFreq, valueSet);
		if( (valueSet[0] & StaCav4ElecFreq0CoarseFreqMask) == (pFromFpga->data & StaCav4ElecFreq0CoarseFreqMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				StaCav4ElecFreq0CoarseFreqString, (unsigned ) pFromFpga->data & StaCav4ElecFreq0CoarseFreqMask);
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				StaCav4ElecFreq0CoarseFreqString, valueSet[0] & StaCav4ElecFreq0CoarseFreqMask, (unsigned ) pFromFpga->data & StaCav4ElecFreq0CoarseFreqMask);
			status = asynError;
			setParamStatus(p_StaCav4ElecFreq0CoarseFreq, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}

		break;
    case StaCav4ElecFreq1CoarseFreqAdr:
		status = (asynStatus) getIntegerParam(p_StaCav4ElecFreq1CoarseFreq, valueSet);
		if( (valueSet[0] & StaCav4ElecFreq1CoarseFreqMask) == (pFromFpga->data & StaCav4ElecFreq1CoarseFreqMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				StaCav4ElecFreq1CoarseFreqString, (unsigned ) pFromFpga->data & StaCav4ElecFreq1CoarseFreqMask);
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				StaCav4ElecFreq1CoarseFreqString, valueSet[0] & StaCav4ElecFreq1CoarseFreqMask, (unsigned ) pFromFpga->data & StaCav4ElecFreq1CoarseFreqMask);
			status = asynError;
			setParamStatus(p_StaCav4ElecFreq1CoarseFreq, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}

		break;
    case StaCav4ElecFreq2CoarseFreqAdr:
		status = (asynStatus) getIntegerParam(p_StaCav4ElecFreq2CoarseFreq, valueSet);
		if( (valueSet[0] & StaCav4ElecFreq2CoarseFreqMask) == (pFromFpga->data & StaCav4ElecFreq2CoarseFreqMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				StaCav4ElecFreq2CoarseFreqString, (unsigned ) pFromFpga->data & StaCav4ElecFreq2CoarseFreqMask);
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				StaCav4ElecFreq2CoarseFreqString, valueSet[0] & StaCav4ElecFreq2CoarseFreqMask, (unsigned ) pFromFpga->data & StaCav4ElecFreq2CoarseFreqMask);
			status = asynError;
			setParamStatus(p_StaCav4ElecFreq2CoarseFreq, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}

		break;
    case StaCav4ElecMode0BeamCplAdr:
		status = (asynStatus) getIntegerParam(p_StaCav4ElecMode0BeamCpl, valueSet);
		if( (valueSet[0] & StaCav4ElecMode0BeamCplMask) == (pFromFpga->data & StaCav4ElecMode0BeamCplMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				StaCav4ElecMode0BeamCplString, (unsigned ) pFromFpga->data & StaCav4ElecMode0BeamCplMask);
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				StaCav4ElecMode0BeamCplString, valueSet[0] & StaCav4ElecMode0BeamCplMask, (unsigned ) pFromFpga->data & StaCav4ElecMode0BeamCplMask);
			status = asynError;
			setParamStatus(p_StaCav4ElecMode0BeamCpl, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}

		break;
    case StaCav4ElecMode0BwAdr:
		status = (asynStatus) getIntegerParam(p_StaCav4ElecMode0Bw, valueSet);
		if( (valueSet[0] & StaCav4ElecMode0BwMask) == (pFromFpga->data & StaCav4ElecMode0BwMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				StaCav4ElecMode0BwString, (unsigned ) pFromFpga->data & StaCav4ElecMode0BwMask);
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				StaCav4ElecMode0BwString, valueSet[0] & StaCav4ElecMode0BwMask, (unsigned ) pFromFpga->data & StaCav4ElecMode0BwMask);
			status = asynError;
			setParamStatus(p_StaCav4ElecMode0Bw, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}

		break;
    case StaCav4ElecMode0DrvCplAdr:
		status = (asynStatus) getIntegerParam(p_StaCav4ElecMode0DrvCpl, valueSet);
		if( (valueSet[0] & StaCav4ElecMode0DrvCplMask) == (pFromFpga->data & StaCav4ElecMode0DrvCplMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				StaCav4ElecMode0DrvCplString, (unsigned ) pFromFpga->data & StaCav4ElecMode0DrvCplMask);
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				StaCav4ElecMode0DrvCplString, valueSet[0] & StaCav4ElecMode0DrvCplMask, (unsigned ) pFromFpga->data & StaCav4ElecMode0DrvCplMask);
			status = asynError;
			setParamStatus(p_StaCav4ElecMode0DrvCpl, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}

		break;
    case StaCav4ElecMode0OutCplOutPhOffAdr:
		status = (asynStatus) getIntegerParam(p_StaCav4ElecMode0OutCplOutPhOff, valueSet);
		if( (valueSet[0] & StaCav4ElecMode0OutCplOutPhOffMask) == (pFromFpga->data & StaCav4ElecMode0OutCplOutPhOffMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				StaCav4ElecMode0OutCplOutPhOffString, (unsigned ) pFromFpga->data & StaCav4ElecMode0OutCplOutPhOffMask);
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				StaCav4ElecMode0OutCplOutPhOffString, valueSet[0] & StaCav4ElecMode0OutCplOutPhOffMask, (unsigned ) pFromFpga->data & StaCav4ElecMode0OutCplOutPhOffMask);
			status = asynError;
			setParamStatus(p_StaCav4ElecMode0OutCplOutPhOff, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}

		break;
    case StaCav4ElecMode1BeamCplAdr:
		status = (asynStatus) getIntegerParam(p_StaCav4ElecMode1BeamCpl, valueSet);
		if( (valueSet[0] & StaCav4ElecMode1BeamCplMask) == (pFromFpga->data & StaCav4ElecMode1BeamCplMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				StaCav4ElecMode1BeamCplString, (unsigned ) pFromFpga->data & StaCav4ElecMode1BeamCplMask);
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				StaCav4ElecMode1BeamCplString, valueSet[0] & StaCav4ElecMode1BeamCplMask, (unsigned ) pFromFpga->data & StaCav4ElecMode1BeamCplMask);
			status = asynError;
			setParamStatus(p_StaCav4ElecMode1BeamCpl, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}

		break;
    case StaCav4ElecMode1BwAdr:
		status = (asynStatus) getIntegerParam(p_StaCav4ElecMode1Bw, valueSet);
		if( (valueSet[0] & StaCav4ElecMode1BwMask) == (pFromFpga->data & StaCav4ElecMode1BwMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				StaCav4ElecMode1BwString, (unsigned ) pFromFpga->data & StaCav4ElecMode1BwMask);
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				StaCav4ElecMode1BwString, valueSet[0] & StaCav4ElecMode1BwMask, (unsigned ) pFromFpga->data & StaCav4ElecMode1BwMask);
			status = asynError;
			setParamStatus(p_StaCav4ElecMode1Bw, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}

		break;
    case StaCav4ElecMode1DrvCplAdr:
		status = (asynStatus) getIntegerParam(p_StaCav4ElecMode1DrvCpl, valueSet);
		if( (valueSet[0] & StaCav4ElecMode1DrvCplMask) == (pFromFpga->data & StaCav4ElecMode1DrvCplMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				StaCav4ElecMode1DrvCplString, (unsigned ) pFromFpga->data & StaCav4ElecMode1DrvCplMask);
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				StaCav4ElecMode1DrvCplString, valueSet[0] & StaCav4ElecMode1DrvCplMask, (unsigned ) pFromFpga->data & StaCav4ElecMode1DrvCplMask);
			status = asynError;
			setParamStatus(p_StaCav4ElecMode1DrvCpl, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}

		break;
    case StaCav4ElecMode2BeamCplAdr:
		status = (asynStatus) getIntegerParam(p_StaCav4ElecMode2BeamCpl, valueSet);
		if( (valueSet[0] & StaCav4ElecMode2BeamCplMask) == (pFromFpga->data & StaCav4ElecMode2BeamCplMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				StaCav4ElecMode2BeamCplString, (unsigned ) pFromFpga->data & StaCav4ElecMode2BeamCplMask);
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				StaCav4ElecMode2BeamCplString, valueSet[0] & StaCav4ElecMode2BeamCplMask, (unsigned ) pFromFpga->data & StaCav4ElecMode2BeamCplMask);
			status = asynError;
			setParamStatus(p_StaCav4ElecMode2BeamCpl, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}

		break;
    case StaCav4ElecMode2BwAdr:
		status = (asynStatus) getIntegerParam(p_StaCav4ElecMode2Bw, valueSet);
		if( (valueSet[0] & StaCav4ElecMode2BwMask) == (pFromFpga->data & StaCav4ElecMode2BwMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				StaCav4ElecMode2BwString, (unsigned ) pFromFpga->data & StaCav4ElecMode2BwMask);
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				StaCav4ElecMode2BwString, valueSet[0] & StaCav4ElecMode2BwMask, (unsigned ) pFromFpga->data & StaCav4ElecMode2BwMask);
			status = asynError;
			setParamStatus(p_StaCav4ElecMode2Bw, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}

		break;
    case StaCav4ElecMode2DrvCplAdr:
		status = (asynStatus) getIntegerParam(p_StaCav4ElecMode2DrvCpl, valueSet);
		if( (valueSet[0] & StaCav4ElecMode2DrvCplMask) == (pFromFpga->data & StaCav4ElecMode2DrvCplMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				StaCav4ElecMode2DrvCplString, (unsigned ) pFromFpga->data & StaCav4ElecMode2DrvCplMask);
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				StaCav4ElecMode2DrvCplString, valueSet[0] & StaCav4ElecMode2DrvCplMask, (unsigned ) pFromFpga->data & StaCav4ElecMode2DrvCplMask);
			status = asynError;
			setParamStatus(p_StaCav4ElecMode2DrvCpl, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}

		break;
    case StaCav4ElecModuloAdr:
		status = (asynStatus) getIntegerParam(p_StaCav4ElecModulo, valueSet);
		if( (valueSet[0] & StaCav4ElecModuloMask) == (pFromFpga->data & StaCav4ElecModuloMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				StaCav4ElecModuloString, (unsigned ) pFromFpga->data & StaCav4ElecModuloMask);
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				StaCav4ElecModuloString, valueSet[0] & StaCav4ElecModuloMask, (unsigned ) pFromFpga->data & StaCav4ElecModuloMask);
			status = asynError;
			setParamStatus(p_StaCav4ElecModulo, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}

		break;
    case StaCav4ElecPhaseStepAdr:
		status = (asynStatus) getIntegerParam(p_StaCav4ElecPhaseStep, valueSet);
		if( (valueSet[0] & StaCav4ElecPhaseStepMask) == (pFromFpga->data & StaCav4ElecPhaseStepMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				StaCav4ElecPhaseStepString, (unsigned ) pFromFpga->data & StaCav4ElecPhaseStepMask);
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				StaCav4ElecPhaseStepString, valueSet[0] & StaCav4ElecPhaseStepMask, (unsigned ) pFromFpga->data & StaCav4ElecPhaseStepMask);
			status = asynError;
			setParamStatus(p_StaCav4ElecPhaseStep, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}

		break;
    case StaComprSatCtlAdr:
		status = (asynStatus) getIntegerParam(p_StaComprSatCtl, valueSet);
		if( (valueSet[0] & StaComprSatCtlMask) == (pFromFpga->data & StaComprSatCtlMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				StaComprSatCtlString, (unsigned ) pFromFpga->data & StaComprSatCtlMask);
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				StaComprSatCtlString, valueSet[0] & StaComprSatCtlMask, (unsigned ) pFromFpga->data & StaComprSatCtlMask);
			status = asynError;
			setParamStatus(p_StaComprSatCtl, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}

		break;
    case StaPrngIvaAdr:
		status = (asynStatus) getIntegerParam(p_StaPrngIva, valueSet);
		if( (valueSet[0] & StaPrngIvaMask) == (pFromFpga->data & StaPrngIvaMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				StaPrngIvaString, (unsigned ) pFromFpga->data & StaPrngIvaMask);
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				StaPrngIvaString, valueSet[0] & StaPrngIvaMask, (unsigned ) pFromFpga->data & StaPrngIvaMask);
			status = asynError;
			setParamStatus(p_StaPrngIva, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}

		break;
    case StaPrngIvbAdr:
		status = (asynStatus) getIntegerParam(p_StaPrngIvb, valueSet);
		if( (valueSet[0] & StaPrngIvbMask) == (pFromFpga->data & StaPrngIvbMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				StaPrngIvbString, (unsigned ) pFromFpga->data & StaPrngIvbMask);
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				StaPrngIvbString, valueSet[0] & StaPrngIvbMask, (unsigned ) pFromFpga->data & StaPrngIvbMask);
			status = asynError;
			setParamStatus(p_StaPrngIvb, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}

		break;
    case StaCav4ElecMode1OutCplOutCplngAdr:
		status = (asynStatus) getIntegerParam(p_StaCav4ElecMode1OutCplOutCplng, valueSet);
		if( (valueSet[0] & StaCav4ElecMode1OutCplOutCplngMask) == (pFromFpga->data & StaCav4ElecMode1OutCplOutCplngMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				StaCav4ElecMode1OutCplOutCplngString, (unsigned ) pFromFpga->data & StaCav4ElecMode1OutCplOutCplngMask);
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				StaCav4ElecMode1OutCplOutCplngString, valueSet[0] & StaCav4ElecMode1OutCplOutCplngMask, (unsigned ) pFromFpga->data & StaCav4ElecMode1OutCplOutCplngMask);
			status = asynError;
			setParamStatus(p_StaCav4ElecMode1OutCplOutCplng, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}

		break;
    case StaCav4ElecMode2OutCplOutCplngAdr:
		status = (asynStatus) getIntegerParam(p_StaCav4ElecMode2OutCplOutCplng, valueSet);
		if( (valueSet[0] & StaCav4ElecMode2OutCplOutCplngMask) == (pFromFpga->data & StaCav4ElecMode2OutCplOutCplngMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				StaCav4ElecMode2OutCplOutCplngString, (unsigned ) pFromFpga->data & StaCav4ElecMode2OutCplOutCplngMask);
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				StaCav4ElecMode2OutCplOutCplngString, valueSet[0] & StaCav4ElecMode2OutCplOutCplngMask, (unsigned ) pFromFpga->data & StaCav4ElecMode2OutCplOutCplngMask);
			status = asynError;
			setParamStatus(p_StaCav4ElecMode2OutCplOutCplng, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}

		break;
    case StaCav4ElecMode2OutCplOutPhOffAdr:
		status = (asynStatus) getIntegerParam(p_StaCav4ElecMode2OutCplOutPhOff, valueSet);
		if( (valueSet[0] & StaCav4ElecMode2OutCplOutPhOffMask) == (pFromFpga->data & StaCav4ElecMode2OutCplOutPhOffMask))
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: readback for address=%s, value=0x%x\n", __PRETTY_FUNCTION__,
				StaCav4ElecMode2OutCplOutPhOffString, (unsigned ) pFromFpga->data & StaCav4ElecMode2OutCplOutPhOffMask);
		else
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
				"%s: value sent to %s, value=0x%x, doesn't match echoed value=0x%x\n", __PRETTY_FUNCTION__,
				StaCav4ElecMode2OutCplOutPhOffString, valueSet[0] & StaCav4ElecMode2OutCplOutPhOffMask, (unsigned ) pFromFpga->data & StaCav4ElecMode2OutCplOutPhOffMask);
			status = asynError;
			setParamStatus(p_StaCav4ElecMode2OutCplOutPhOff, status);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}

		break;
	default:
// TODO: Add checking for arrays. Until then, we'll hit "default" for array values, so not really an error. GWB 8-23-2016
//		getIntegerParam(p_CommErrorCount, &errorCount);
//		setIntegerParam(p_CommErrorCount, errorCount + 1);

//		status = asynError;
		break;
    }

	// TODO: handle arrays

    return status;
}

void htonFpgaRegArray(FpgaReg *buffer, unsigned int regCount)
{
	unsigned int i;

	for (i=0; i<regCount; ++i)
	{
		buffer[i].addr = htonl(buffer[i].addr);
		buffer[i].data = htonl(buffer[i].data);
	}
}


void ntohFpgaRegArray(FpgaReg *buffer, unsigned int regCount)
{
	unsigned int i;

	for (i=0; i<regCount; ++i)
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

