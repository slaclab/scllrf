/*
 * BPM device support
 */
#include <string.h>
#include <stdint.h> /* R3.14 version of epicsTypes.h has no 64 bit support */
#include <math.h>
#include <initHooks.h>
#include <epicsStdio.h>
#include <epicsString.h>
#include <epicsExit.h>
#include <epicsEvent.h>
#include <epicsThread.h>
#include <epicsTime.h>
#include <epicsMutex.h>
#include <epicsExport.h>
#include <cantProceed.h>
#include <alarm.h>
#include <iocsh.h>
#include <devLib.h>
#include <asynDriver.h>
#include <asynOctet.h>
#include <asynInt32.h>
#include <asynUInt32Digital.h>
#include <drvAsynIPPort.h>
#include <asynInt32Array.h>
#include <asynFloat32Array.h>
#include <asynFloat64Array.h>
#include <asynCommonSyncIO.h>

#include "../../bpmccS_u-p.tmp/src/cellControllerProtocol.h"

/*
 * Time conversion
 */
#define NOMINAL_RF_FREQUENCY 499.642e6
#define NS_PER_TICK (1.0e9 / ((NOMINAL_RF_FREQUENCY / 4)))
#define NOMINAL_FAST_UPDATE_FREQUENCY (NOMINAL_RF_FREQUENCY/328.0/152.0)

/*
 * ASYN subaddresses
 * Augment protocol command groups
 */
#define A_HI_STATISTICS         0x0E00

/*
 * Number of times to retry a command
 */
#define COMMAND_RETRY_LIMIT 4

/*
 * Cell controller monitoring
 */
#define CC_MONITOR_UDP_PORT 50001
#define CC_MONITOR_REQUEST_REG_LIMIT     63
#define CC_MONITOR_RESPONSE_CAPACITY    187

/*
 * Links to lower port
 */
typedef struct portLink {
    char             *portName;
    char             *hostInfo;
    asynUser         *pasynUserCommon;
    asynUser         *pasynUserOctet;
    asynInterface    *poctet;
    int               isCommunicating;
} portLink;

/*
 * Driver private storage
 */
typedef struct drvPvt {
    /*
     * Asyn interfaces we provide
     */
    char                *portName;
    asynInterface        asynCommon;
    asynInterface        asynOctet;
    asynInterface        asynInt32;
    void                *asynInt32InterruptPvt;
    asynInterface        asynUInt32Digital;
    void                *asynUInt32DigitalInterruptPvt;
    asynInterface        asynInt32Array;
    asynInterface        asynFloat32Array;
    asynInterface        asynFloat64Array;

    /*
     * Link to lower ports
     */
    portLink             cmdLink;
    unsigned int         identifier;
    int                  cellInfo;
    portLink             monitorLink;

    /*
     * Pilot tone PLL low-level I/O
     */
    asynStatus           pllRegIOstatus;
    epicsUInt32          pllRegIOvalue;

    /*
     * EEBI coefficients
     */
    epicsInt32           EEBIselect;
    epicsInt32           EEBIoffset0;
    epicsInt32           EEBIlimit0;
    epicsInt32           EEBIoffset1;
    epicsInt32           EEBIlimit1;
    epicsInt32           EEBIskewLimit;
    epicsInt32           EEBIcurrentThreshold;

    /*
     * Statistics
     */
    unsigned long        commandCount[COMMAND_RETRY_LIMIT+1];
    unsigned long        commandFailedCount;

    /*
     * Monitoring
     */
    unsigned char         monitorRequest[2*CC_MONITOR_REQUEST_REG_LIMIT];
    unsigned char         monitorResponse[CC_MONITOR_RESPONSE_CAPACITY];
    int                   monitorIsActive;
    epicsTimeStamp        monitorWhenFailed;
} drvPvt;


static void
report(void *pvt, FILE *fp, int details)
{
    drvPvt *pdpvt = (drvPvt *)pvt;

    if (!pdpvt->cmdLink.isCommunicating)
        fprintf(fp, " Command UDP  not communicating\n");
    if (!pdpvt->monitorLink.isCommunicating)
        fprintf(fp, " Monitor UDP  not communicating\n");
}

static asynStatus
connect(void *pvt, asynUser *pasynUser)
{
    pasynManager->exceptionConnect(pasynUser);
    return asynSuccess;
}

static asynStatus
disconnect(void *pvt, asynUser *pasynUser)
{
    pasynManager->exceptionDisconnect(pasynUser);
    return asynSuccess;
}
static asynCommon commonMethods = { report, connect, disconnect };

static asynStatus
cmdWriteRead(drvPvt *pdpvt, asynUser *pasynUser,
             struct ccProtocolPacket *cmdp, size_t cmdSize,
             struct ccProtocolPacket *replyp, size_t *replySize)
{
    asynStatus status;
    int retryCount;
    int eom;
    size_t ntrans;
    asynOctet *pasynOctet = pdpvt->cmdLink.poctet->pinterface;
    int omitSend = 0;

    if (!pdpvt->cmdLink.isCommunicating) {
        status = pasynCommonSyncIO->connectDevice(pdpvt->cmdLink.pasynUserCommon);
        if (status != asynSuccess) {
            asynPrint(pasynUser, ASYN_TRACE_ERROR,
                            "%s failed to connect to %s: %s\n",
                                pdpvt->cmdLink.portName,
                                pdpvt->cmdLink.hostInfo,
                                pdpvt->cmdLink.pasynUserOctet->errorMessage);
            return status;
        }
        pdpvt->cmdLink.isCommunicating = 1;
    }
    cmdp->magic = CC_PROTOCOL_MAGIC;
    cmdp->identifier = ++pdpvt->identifier;
    cmdp->cellInfo = pdpvt->cellInfo;
    pdpvt->cmdLink.pasynUserOctet->timeout = 0.2;
    for (retryCount = 0 ; ; ) {
        pasynManager->lockPort(pdpvt->cmdLink.pasynUserOctet);
        if (omitSend) {
            status = asynSuccess;
            omitSend = 0;
        }
        else {
            asynPrint(pasynUser, ASYN_TRACEIO_DRIVER,
                    "ID:%d CMD:%#X ARGS:%d\n",
                                cmdp->identifier,
                                cmdp->command,
                                (int)CC_PROTOCOL_SIZE_TO_ARG_COUNT(cmdSize));
            status = pasynOctet->write(pdpvt->cmdLink.poctet->drvPvt,
                                          pdpvt->cmdLink.pasynUserOctet,
                                          (const char *)cmdp, cmdSize, &ntrans);
        }
        if (status == asynSuccess) {
            status = pasynOctet->read(pdpvt->cmdLink.poctet->drvPvt,
                                      pdpvt->cmdLink.pasynUserOctet,
                                      (char *)replyp, sizeof *replyp,
                                      replySize, &eom);
        }
        pasynManager->unlockPort(pdpvt->cmdLink.pasynUserOctet);
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER,
                        "Read status %d, count %zu\n", (int)status, *replySize);
        if (status == asynSuccess) {
            if ((*replySize >= CC_PROTOCOL_ARG_COUNT_TO_SIZE(0))
             && (*replySize <= CC_PROTOCOL_ARG_COUNT_TO_SIZE(CC_PROTOCOL_ARG_CAPACITY))) {
                if (replyp->identifier == cmdp->identifier) {
                    pdpvt->commandCount[retryCount]++;
                    return asynSuccess;
                }
                epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize,
                            "Sent command %u, got reply %u",
                                             (unsigned int)cmdp->identifier,
                                             (unsigned int)replyp->identifier);
                status = asynError;
                omitSend = 1;
            }
            else {
                epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize,
                                    "Got %lu bytes", (unsigned long)*replySize);
                status = asynError;
            }
        }
        if (++retryCount > COMMAND_RETRY_LIMIT)
            break;
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, "%s retry: %s\n",
                   pdpvt->cmdLink.portName,
                   status == asynTimeout ? "Timeout" : pasynUser->errorMessage);
        pdpvt->cmdLink.pasynUserOctet->timeout = retryCount / 2.0;
    }
    if (status == asynTimeout)
        epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize,
                                                                    "Timeout");
    if (pasynCommonSyncIO->disconnectDevice(pdpvt->cmdLink.pasynUserCommon) !=
                                                                    asynSuccess)
        asynPrint(pasynUser, ASYN_TRACE_ERROR, "%s can't disconnect: %s\n",
                                pdpvt->cmdLink.portName,
                                pdpvt->cmdLink.pasynUserCommon->errorMessage);
    pdpvt->cmdLink.isCommunicating = 0;
    pdpvt->commandFailedCount++;
    return status;
}

/*
 * asynOctet methods
 */
static asynStatus
octetRead(void *pvt, asynUser *pasynUser, char *data, size_t maxchars, size_t *nTransferred, int *eomReason)
{
    drvPvt *pdpvt = (drvPvt *)pvt;
    asynStatus status;
    int address, ahi, alo, idx;
    struct ccProtocolPacket cmd, reply;
    size_t replySize, len;
    time_t buildDate;

    if ((status = pasynManager->getAddr(pasynUser, &address)) != asynSuccess)
        return status;
    ahi = address & CC_PROTOCOL_CMD_MASK_HI;
    alo = address & CC_PROTOCOL_CMD_MASK_LO;
    idx = address & CC_PROTOCOL_CMD_MASK_IDX;
    if ((ahi != CC_PROTOCOL_CMD_HI_LONGIN)
     || (alo != 0)
     || ((idx != CC_PROTOCOL_CMD_LONGIN_IDX_FIRMWARE_BUILD_DATE)
      && (idx != CC_PROTOCOL_CMD_LONGIN_IDX_SOFTWARE_BUILD_DATE)))
        return asynError;
    cmd.command = address;
    status = cmdWriteRead(pdpvt, pasynUser, &cmd,
                                              CC_PROTOCOL_ARG_COUNT_TO_SIZE(0),
                                              &reply, &replySize);
    if (status != asynSuccess) return status;
    if (CC_PROTOCOL_SIZE_TO_ARG_COUNT(replySize) != 1) return asynError;
    buildDate = reply.args[0];
    len = strftime(data, maxchars, "%F %T", localtime(&buildDate));
    if (len == 0) {
        epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize,
                                            "Target string buffer too small");
        return asynError;
    }
    len++; /* Include '\0' */
    *nTransferred = len;
    if (eomReason) {
        *eomReason = ASYN_EOM_END;
        if (len == maxchars) *eomReason |= ASYN_EOM_CNT;
    }
    return asynSuccess;
}

static asynOctet octetMethods = { NULL, octetRead };

/*
 * asynInt32 methods
 */
static void
processSysmonReply(drvPvt *pdpvt, struct ccProtocolPacket *replyp, int argc)
{
    ELLLIST *pclientList;
    interruptNode *pnode;
    extern volatile int interruptAccept;

    if (!interruptAccept) return;
    pasynManager->interruptStart(pdpvt->asynInt32InterruptPvt, &pclientList);
    pnode = (interruptNode *)ellFirst(pclientList);
    while (pnode) {
        asynInt32Interrupt *int32Interrupt = pnode->drvPvt;
        pnode = (interruptNode *)ellNext(&pnode->node);
        if ((int32Interrupt->addr & CC_PROTOCOL_CMD_MASK_HI) ==
                                                    CC_PROTOCOL_CMD_HI_SYSMON) {
            int idx = int32Interrupt->addr & ~CC_PROTOCOL_CMD_MASK_HI;
            epicsUInt16 value;
            if (idx >= argc*2) {
                int32Interrupt->pasynUser->auxStatus = asynError;
                value = 0;
            }
            else {
                int32Interrupt->pasynUser->auxStatus = asynSuccess;
                value = replyp->args[idx/2] >> ((idx & 0x1) ? 16 : 0);
            }
            int32Interrupt->callback(int32Interrupt->userPvt,
                                     int32Interrupt->pasynUser,
                                     value);
        }
    }
    pasynManager->interruptEnd(pdpvt->asynInt32InterruptPvt);
}

static void
processLinkstatsReply(drvPvt *pdpvt, struct ccProtocolPacket *replyp, int argc)
{
    ELLLIST *pclientList;
    interruptNode *pnode;
    extern volatile int interruptAccept;

    if (!interruptAccept) return;
    pasynManager->interruptStart(pdpvt->asynInt32InterruptPvt, &pclientList);
    pnode = (interruptNode *)ellFirst(pclientList);
    while (pnode) {
        asynInt32Interrupt *int32Interrupt = pnode->drvPvt;
        int idx = int32Interrupt->addr & CC_PROTOCOL_CMD_MASK_IDX;
        pnode = (interruptNode *)ellNext(&pnode->node);
        if ((int32Interrupt->addr & CC_PROTOCOL_CMD_MASK_HI) ==
                                                CC_PROTOCOL_CMD_HI_LINKSTATS) {
            epicsUInt32 value;
            if (idx >= argc) {
                int32Interrupt->pasynUser->auxStatus = asynError;
                value = 0;
            }
            else {
                int32Interrupt->pasynUser->auxStatus = asynSuccess;
                value = replyp->args[idx];
            }
            int32Interrupt->callback(int32Interrupt->userPvt,
                                     int32Interrupt->pasynUser,
                                     value);
        }
    }
    pasynManager->interruptEnd(pdpvt->asynInt32InterruptPvt);
}

/*
 * Get time from data or system
 */
static void
setTimestamp(int isValid,
              epicsUInt32 seconds, epicsUInt32 ticks, epicsTimeStamp *ts)
{
    if (isValid && (seconds != 0)) {
        if (seconds > POSIX_TIME_AT_EPICS_EPOCH)
            ts->secPastEpoch = seconds - POSIX_TIME_AT_EPICS_EPOCH;
        else
            ts->secPastEpoch = seconds;
        ts->nsec = ticks * NS_PER_TICK;
        if (ts->nsec >= 1000000000) {
            ts->secPastEpoch += ts->nsec / 1000000000;
            ts->nsec %= 1000000000;
        }
    }
    else {
        epicsTimeGetCurrent(ts);
    }
}

static void
processEEBIreply(drvPvt *pdpvt, struct ccProtocolPacket *replyp, int argc)
{
    ELLLIST *pclientList;
    interruptNode *pnode;
    epicsTimeStamp ts;
    extern volatile int interruptAccept;

    if (!interruptAccept) return;
    setTimestamp((argc > 0), replyp->args[2], replyp->args[3], &ts);
    pasynManager->interruptStart(pdpvt->asynUInt32DigitalInterruptPvt, &pclientList);
    pnode = (interruptNode *)ellFirst(pclientList);
    while (pnode) {
        asynUInt32DigitalInterrupt *uint32DigitalInterrupt = pnode->drvPvt;
        int idx = uint32DigitalInterrupt->addr & CC_PROTOCOL_CMD_MASK_IDX;
        pnode = (interruptNode *)ellNext(&pnode->node);
        if ((uint32DigitalInterrupt->addr & CC_PROTOCOL_CMD_MASK_HI) ==
                                                CC_PROTOCOL_CMD_HI_GET_EEBI) {
            epicsUInt32 value;
            if (idx >= argc) {
                uint32DigitalInterrupt->pasynUser->auxStatus = asynError;
                value = 0;
            }
            else {
                uint32DigitalInterrupt->pasynUser->auxStatus = asynSuccess;
                uint32DigitalInterrupt->pasynUser->timestamp = ts;
                value = replyp->args[idx];
            }
            uint32DigitalInterrupt->callback(uint32DigitalInterrupt->userPvt,
                                     uint32DigitalInterrupt->pasynUser,
                                     value);
        }
    }
    pasynManager->interruptEnd(pdpvt->asynUInt32DigitalInterruptPvt);
}

static void
processBMB7MonitorResponse(drvPvt *pdpvt, int fault)
{
    ELLLIST *pclientList;
    interruptNode *pnode;
    extern volatile int interruptAccept;
    static const struct {
        unsigned char   hi;  /* Index into reply packet to hi byte of value */
        unsigned char   hiMask;      /* Valid bits in hi byte */
        unsigned char   signedWidth; /* 0 means unsigned */
    } addrMap[] = {
        { 138, 0x1F,  0 }, /*  0: LTC2990 A0 */
        { 136, 0x3F,  0 },
        { 134, 0x7F, 15 },
        {  58, 0x7F, 15 },
        { 132, 0x7F, 15 },
        {  56, 0x7F, 15 },

        { 130, 0x1F,  0 }, /*  6: LTC2990 A1 */
        { 128, 0x3F,  0 },
        { 126, 0x7F, 15 },
        {  54, 0x7F, 15 },
        { 124, 0x7F, 15 },
        {  52, 0x7F, 15 },

        { 122, 0x1F,  0 }, /* 12: LTC2990 A2 */
        { 120, 0x3F,  0 },
        { 118, 0x7F, 15 },
        {  50, 0x7F, 15 },

        { 114, 0x1F,  0 }, /* 16: LTC2990 B0 */
        { 112, 0x3F,  0 },
        { 110, 0x7F, 15 },
        {  46, 0x7F, 15 },
        { 108, 0x7F, 15 },
        {  44, 0x7F, 15 },

        { 106, 0x1F,  0 }, /* 22: LTC2990 B1 */
        { 104, 0x3F,  0 },
        { 102, 0x7F, 15 },
        {  42, 0x7F, 15 },
        { 100, 0x7F, 15 },
        {  40, 0x7F, 15 },

        {  98, 0x1F,  0 }, /* 28: LTC2990 B2 */
        {  96, 0x3F,  0 },
        {  94, 0x7F, 15 },
        {  38, 0x7F, 15 },
        {  92, 0x7F, 15 },
        {  36, 0x7F, 15 },

        {  90, 0x1F,  0 }, /* 34: LTC2990 C0 */
        {  88, 0x3F,  0 },
        {  86, 0x7F, 15 },
        {  34, 0x7F, 15 },
        {  84, 0x7F, 15 },
        {  32, 0x7F, 15 },

        {  82, 0x1F,  0 }, /* 40: LTC2990 C1 */
        {  80, 0x3F,  0 },
        {  78, 0x7F, 15 },
        {  30, 0x7F, 15 },
        {  76, 0x7F, 15 },
        {  28, 0x7F, 15 },

        {  74, 0x1F,  0 }, /* 46: LTC2990 C2 */
        {  72, 0x3F,  0 },
        {  70, 0x7F, 15 },
        {  26, 0x1F, 15 },
        {  68, 0x1F,  0 },

        {  66, 0x1F,  0 }, /* 51: LTC2990 C3 */
        {  64, 0x3F,  0 },
        {  62, 0x7F, 15 },
        {  22, 0x7F, 15 }
    };

    if (!interruptAccept) return;
    pasynManager->interruptStart(pdpvt->asynInt32InterruptPvt, &pclientList);
    pnode = (interruptNode *)ellFirst(pclientList);
    while (pnode) {
        asynInt32Interrupt *int32Interrupt = pnode->drvPvt;
        int idx = int32Interrupt->addr & ~CC_PROTOCOL_CMD_MASK_HI;
        pnode = (interruptNode *)ellNext(&pnode->node);
        if ((int32Interrupt->addr & CC_PROTOCOL_CMD_MASK_HI) ==
                                            CC_PROTOCOL_CMD_HI_BMB7_MONITOR) {
            epicsInt32 value;
            if (fault || (idx >= (sizeof addrMap / sizeof addrMap[0]))) {
                int32Interrupt->pasynUser->auxStatus = asynError;
                value = 0;
            }
            else {
                int h = CC_MONITOR_RESPONSE_CAPACITY-1-addrMap[idx].hi;
                int32Interrupt->pasynUser->auxStatus = asynSuccess;
                value = ((pdpvt->monitorResponse[h]&addrMap[idx].hiMask) << 8) |
                          pdpvt->monitorResponse[h+1];
                if (addrMap[idx].signedWidth) {
                    if (value >= (1 << (addrMap[idx].signedWidth - 1)))
                        value -= 1 << addrMap[idx].signedWidth;
                }
            }
            int32Interrupt->callback(int32Interrupt->userPvt,
                                     int32Interrupt->pasynUser,
                                     value);
        }
    }
    pasynManager->interruptEnd(pdpvt->asynInt32InterruptPvt);
}

/*
 * Set a mask/value request
 */
static void
setBMB7monitorRequest(drvPvt *pdpvt, unsigned int regIdx, int mask, int value)
{
    if (regIdx >= CC_MONITOR_REQUEST_REG_LIMIT) return;
    pdpvt->monitorRequest[1*CC_MONITOR_REQUEST_REG_LIMIT-1-regIdx] = mask;
    pdpvt->monitorRequest[2*CC_MONITOR_REQUEST_REG_LIMIT-1-regIdx] = value;
}

static asynStatus
writeReadBMB7monitor(asynUser *pasynUser, drvPvt *pdpvt)
{
    asynStatus status;
    asynOctet *pasynOctet = pdpvt->monitorLink.poctet->pinterface;
    size_t ntrans;
    int eomReason;

    pdpvt->monitorLink.pasynUserOctet->timeout = 0.2;
    pasynManager->lockPort(pdpvt->monitorLink.pasynUserOctet);
    status = pasynOctet->write(pdpvt->monitorLink.poctet->drvPvt,
                                      pdpvt->monitorLink.pasynUserOctet,
                                      (const char *)pdpvt->monitorRequest,
                                      2*CC_MONITOR_REQUEST_REG_LIMIT, &ntrans);
    if (status == asynSuccess) {
        status = pasynOctet->read(pdpvt->monitorLink.poctet->drvPvt,
                                      pdpvt->monitorLink.pasynUserOctet,
                                      (char *)pdpvt->monitorResponse,
                                      CC_MONITOR_RESPONSE_CAPACITY,
                                      &ntrans, &eomReason);
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER,
                    "Monitor read status %d, count %zu\n", (int)status, ntrans);
    }
    pasynManager->unlockPort(pdpvt->monitorLink.pasynUserOctet);
    if (status != asynSuccess) {
        epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize,
                            "Monitor transfer failed: \"%s\"",
                            pdpvt->monitorLink.pasynUserOctet->errorMessage);
        if (pasynCommonSyncIO->disconnectDevice(pdpvt->monitorLink.pasynUserCommon) !=
                                                                    asynSuccess)
            asynPrint(pasynUser, ASYN_TRACE_ERROR, "%s can't disconnect: %s\n",
                            pdpvt->monitorLink.portName,
                            pdpvt->monitorLink.pasynUserCommon->errorMessage);
        processBMB7MonitorResponse(pdpvt, 1);
        epicsTimeGetCurrent(&pdpvt->monitorWhenFailed);
        pdpvt->monitorLink.isCommunicating = 0;
    }
    return status;
}

static asynStatus
crankBMB7monitor(asynUser *pasynUser, drvPvt *pdpvt)
{
    asynStatus status;
    epicsTimeStamp now;

    if (!pdpvt->monitorLink.isCommunicating) {
        epicsTimeGetCurrent(&now);
        if (epicsTimeDiffInSeconds(&now, &pdpvt->monitorWhenFailed) < 10)
            return asynError;
        status = pasynCommonSyncIO->connectDevice(pdpvt->monitorLink.pasynUserCommon);
        if (status != asynSuccess) {
            asynPrint(pasynUser, ASYN_TRACE_ERROR,
                        "%s failed to connect to %s: %s\n",
                            pdpvt->monitorLink.portName,
                            pdpvt->monitorLink.hostInfo,
                            pdpvt->monitorLink.pasynUserOctet->errorMessage);
            return status;
        }
        pdpvt->monitorLink.isCommunicating = 1;
        pdpvt->monitorIsActive = 0;
    }
    if (pdpvt->monitorIsActive) {
        if ((status = writeReadBMB7monitor(pasynUser, pdpvt)) != asynSuccess)
            return status;
        if (pdpvt->monitorResponse[CC_MONITOR_RESPONSE_CAPACITY-1-3] == 0x1) {
            processBMB7MonitorResponse(pdpvt, 0);
            pdpvt->monitorIsActive = 0;
        }
        else if (--pdpvt->monitorIsActive == 0) {
            processBMB7MonitorResponse(pdpvt, 1);
            return asynTimeout;
        }
    }
    else {
        setBMB7monitorRequest(pdpvt, 18, 0x10, 0x10);
        status = writeReadBMB7monitor(pasynUser, pdpvt);
        setBMB7monitorRequest(pdpvt, 18, 0x10, 0x00);
        if (status != asynSuccess)
            return status;
        if ((status = writeReadBMB7monitor(pasynUser, pdpvt)) != asynSuccess)
            return status;
        pdpvt->monitorIsActive = 10;
    }
    return asynSuccess;
}

static asynStatus
int32Write(void *pvt, asynUser *pasynUser, epicsInt32 value)
{
    drvPvt *pdpvt = (drvPvt *)pvt;
    asynStatus status;
    int address, ahi;
    int argc = 0;
    struct ccProtocolPacket cmd, reply;
    size_t replySize;

    if ((status = pasynManager->getAddr(pasynUser, &address)) != asynSuccess)
        return status;
    ahi = address & CC_PROTOCOL_CMD_MASK_HI;
    cmd.command = address;
    if (ahi == CC_PROTOCOL_CMD_HI_SET_EEBI_CONFIG) {
        cmd.args[argc++] = pdpvt->EEBIselect;
        cmd.args[argc++] = pdpvt->EEBIoffset0;
        cmd.args[argc++] = pdpvt->EEBIoffset1;
        cmd.args[argc++] = pdpvt->EEBIlimit0;
        cmd.args[argc++] = pdpvt->EEBIlimit1;
        cmd.args[argc++] = pdpvt->EEBIskewLimit;
        cmd.args[argc++] = pdpvt->EEBIcurrentThreshold;
        cmd.args[argc++] = value;
    }
    else {
        cmd.args[argc++] = value;
    }
    status = cmdWriteRead(pdpvt, pasynUser, &cmd,
                      CC_PROTOCOL_ARG_COUNT_TO_SIZE(argc), &reply, &replySize);
    if (ahi == CC_PROTOCOL_CMD_HI_PLL_REG_IO) {
        pdpvt->pllRegIOstatus = status;
        pdpvt->pllRegIOvalue = reply.args[0];
    }
    return status;
}

static asynStatus
int32Read(void *pvt, asynUser *pasynUser, epicsInt32 *value)
{
    drvPvt *pdpvt = (drvPvt *)pvt;
    asynStatus status;
    int address;
    size_t replySize;

    if ((status = pasynManager->getAddr(pasynUser, &address)) != asynSuccess)
        return status;

    status = crankBMB7monitor(pasynUser, pdpvt);
    return status;
}

static asynInt32 int32Methods = { int32Write, int32Read };

/*
 * Create a new lower port and set up a link to it
 */
static asynStatus
setLink(drvPvt *pdpvt, const char *address, const char *ext, portLink *link, int udpPort, int priority)
{
    asynStatus status;

    link->portName = callocMustSucceed(1, strlen(pdpvt->portName)+strlen(ext)+1, "bpmccConf");
    sprintf(link->portName, "%s%s", pdpvt->portName, ext);
    link->hostInfo = callocMustSucceed(1, strlen(address)+20, "bpmccConf");
    sprintf(link->hostInfo, "%s:%d UDP", address, udpPort);
    /* No autoconnect, No process EOS */
    drvAsynIPPortConfigure(link->portName, link->hostInfo, priority, 1, 1);
    status = pasynCommonSyncIO->connect(link->portName, -1,
                                       &link->pasynUserCommon, NULL);
    if (status != asynSuccess) {
        printf("Can't set asynCommonSyncIO for port \"%s\".\n", link->portName);
        return asynError;
    }
    link->pasynUserOctet= pasynManager->createAsynUser(NULL, NULL);
    status = pasynManager->connectDevice(link->pasynUserOctet, link->portName, -1);
    if (status != asynSuccess) {
        printf("Can't find asyn port \"%s\".\n", link->portName);
        return asynError;
    }
    link->poctet = pasynManager->findInterface(link->pasynUserOctet, asynOctetType, 0);
    if (link->poctet == NULL) {
        printf("Can't find octet interface for \"%s\".\n", link->portName);
        return asynError;
    }
    link->pasynUserOctet->timeout = 5.0;
    return asynSuccess;
}

static void
bpmccConfigure(const char *portName, const char *address,
               int cellIndex, int cellCount, int bpmCount,
               int priority,
               int EEBIselect0, int EEBIselect1,
               int EEBIoffset0, int EEBIoffset1,
               int EEBIlimit0 , int EEBIlimit1,
               int EEBIskewLimit, int EEBIcurrentThreshold)
{
    drvPvt *pdpvt;
    asynStatus status;
    int addressLen;

    /*
     * Handle defaults
     */
    if (priority == 0) priority = epicsThreadPriorityMedium;
    if ((portName == NULL) || (*portName == '\0')
     || (address == NULL)  || (*address == '\0')) {
        printf("Required argument not present!\n");
        return;
    }
    if ((cellCount <= 0) || (cellCount > CC_PROTOCOL_MAX_CELLS)) {
        printf("Invalid cell count (must be [1..%d]\n", CC_PROTOCOL_MAX_CELLS);
        return;
    }
    if ((bpmCount < 0) || (bpmCount > CC_PROTOCOL_MAX_BPM_PER_CELL)) {
        printf("Invalid BPM count (must be [0..%d]\n", CC_PROTOCOL_MAX_BPM_PER_CELL);
        return;
    }
    if ((cellIndex < 0) || (cellIndex > 31)) {
        printf("Invalid cell index (must be [0..31]\n");
        return;
    }

    /*
     * Set up information for connection to BPM
     */
    if (strchr(address, ':') != NULL) {
        printf("Cell controller host info must not specify port.\n");
        return;
    }
    addressLen = strlen(address);
    if (address[addressLen - 1] == '*') {
        addressLen--;
        printf("Warning -- Broadcast designator ignored.\n");
    }

    /*
     * Set up local storage
     */
    pdpvt = (drvPvt *)callocMustSucceed(1, sizeof(drvPvt), "bpmccConf");
    pdpvt->portName = epicsStrDup(portName);
    pdpvt->cellInfo = (bpmCount << 16) | (cellCount << 8) | cellIndex;

    /*
     * Create the ports that we'll use to communicate with the BMB7.
     */
    if ((setLink(pdpvt, address, "_MON", &pdpvt->monitorLink,
                                CC_MONITOR_UDP_PORT, priority) != asynSuccess))
        return;

    /*
     * Set up EEBI if so requested
     */
    if (EEBIselect0 != EEBIselect1) {
        printf("Errant Electron Beam Interlock operation enabled.\n");
        int plane0 = (EEBIselect0 >= CC_PROTOCOL_FOFB_CAPACITY_PER_PLANE);
        int plane1 = (EEBIselect1 >= CC_PROTOCOL_FOFB_CAPACITY_PER_PLANE);
        pdpvt->EEBIselect =
                (((plane1 * CC_PROTOCOL_FOFB_CAPACITY_PER_PLANE) |
                  (EEBIselect1 % CC_PROTOCOL_FOFB_CAPACITY_PER_PLANE)) << 16) |
                 ((plane0 * CC_PROTOCOL_FOFB_CAPACITY_PER_PLANE) |
                  (EEBIselect0 % CC_PROTOCOL_FOFB_CAPACITY_PER_PLANE));
        pdpvt->EEBIoffset0 = EEBIoffset0;
        pdpvt->EEBIlimit0 = EEBIlimit0;
        pdpvt->EEBIoffset1 = EEBIoffset1;
        pdpvt->EEBIlimit1 = EEBIlimit1;
        pdpvt->EEBIskewLimit = EEBIskewLimit;
        pdpvt->EEBIcurrentThreshold = EEBIcurrentThreshold;
    }

    /*
     * Create our port
     */
    status = pasynManager->registerPort(pdpvt->portName,
                                        ASYN_CANBLOCK|ASYN_MULTIDEVICE,
                                        1, 0, 0);
    if(status != asynSuccess) {
        printf("registerPort failed\n");
        return;
    }
    pdpvt->asynCommon.interfaceType = asynCommonType;
    pdpvt->asynCommon.pinterface  = &commonMethods;
    pdpvt->asynCommon.drvPvt = pdpvt;
    status = pasynManager->registerInterface(pdpvt->portName, &pdpvt->asynCommon);
    if (status != asynSuccess) {
        printf("registerInterface failed\n");
        return;
    }
    pdpvt->asynOctet.interfaceType = asynOctetType;
    pdpvt->asynOctet.pinterface  = &octetMethods;
    pdpvt->asynOctet.drvPvt = pdpvt;
    status = pasynOctetBase->initialize(pdpvt->portName, &pdpvt->asynOctet, 0, 0, 0);
    if (status != asynSuccess) {
        printf("pasynOctetBase->initialize failed\n");
        return;
    }
    pdpvt->asynInt32.interfaceType = asynInt32Type;
    pdpvt->asynInt32.pinterface  = &int32Methods;
    pdpvt->asynInt32.drvPvt = pdpvt;
    status = pasynInt32Base->initialize(pdpvt->portName, &pdpvt->asynInt32);
    if (status != asynSuccess) {
        printf("pasynInt32Base->initialize failed\n");
        return;
    }
    pasynManager->registerInterruptSource(pdpvt->portName,
                                         &pdpvt->asynInt32,
                                         &pdpvt->asynInt32InterruptPvt);
    pdpvt->asynUInt32Digital.interfaceType = asynUInt32DigitalType;
    pdpvt->asynUInt32Digital.pinterface  = &uint32DigitalMethods;
    pdpvt->asynUInt32Digital.drvPvt = pdpvt;
    status = pasynUInt32DigitalBase->initialize(pdpvt->portName, &pdpvt->asynUInt32Digital);
    if (status != asynSuccess) {
        printf("pasynUInt32DigitalBase->initialize failed\n");
        return;
    }
    pasynManager->registerInterruptSource(pdpvt->portName,
                                         &pdpvt->asynUInt32Digital,
                                         &pdpvt->asynUInt32DigitalInterruptPvt);
    pdpvt->asynInt32Array.interfaceType = asynInt32ArrayType;
    pdpvt->asynInt32Array.pinterface  = &int32ArrayMethods;
    pdpvt->asynInt32Array.drvPvt = pdpvt;
    status = pasynInt32ArrayBase->initialize(pdpvt->portName, &pdpvt->asynInt32Array);
    if (status != asynSuccess) {
        printf("pasynInt32ArrayBase->initialize failed\n");
        return;
    }
    pdpvt->asynFloat32Array.interfaceType = asynFloat32ArrayType;
    pdpvt->asynFloat32Array.pinterface  = &float32ArrayMethods;
    pdpvt->asynFloat32Array.drvPvt = pdpvt;
    status = pasynFloat32ArrayBase->initialize(pdpvt->portName, &pdpvt->asynFloat32Array);
    if (status != asynSuccess) {
        printf("pasynFloat32ArrayBase->initialize failed\n");
        return;
    }
    pdpvt->asynFloat64Array.interfaceType = asynFloat64ArrayType;
    pdpvt->asynFloat64Array.pinterface  = &float64ArrayMethods;
    pdpvt->asynFloat64Array.drvPvt = pdpvt;
    status = pasynFloat64ArrayBase->initialize(pdpvt->portName, &pdpvt->asynFloat64Array);
    if (status != asynSuccess) {
        printf("pasynFloat64ArrayBase->initialize failed\n");
        return;
    }
}

/*
 * IOC shell command registration
 */
static const iocshArg bpmccConfigureArg0 ={ "port",             iocshArgString};
static const iocshArg bpmccConfigureArg1 ={ "address",          iocshArgString};
static const iocshArg bpmccConfigureArg2 ={ "cell Index",       iocshArgInt};
static const iocshArg bpmccConfigureArg3 ={ "cell Count",       iocshArgInt};
static const iocshArg bpmccConfigureArg4 ={ "cell BPM count",   iocshArgInt};
static const iocshArg bpmccConfigureArg5 ={ "priority",         iocshArgInt};
static const iocshArg bpmccConfigureArg6 ={ "EEBIselect0",      iocshArgInt};
static const iocshArg bpmccConfigureArg7 ={ "EEBIselect1",      iocshArgInt};
static const iocshArg bpmccConfigureArg8 ={ "EEBIoffset0",      iocshArgInt};
static const iocshArg bpmccConfigureArg9 ={ "EEBIoffset1",      iocshArgInt};
static const iocshArg bpmccConfigureArg10={ "EEBIlimit0",       iocshArgInt};
static const iocshArg bpmccConfigureArg11={ "EEBIlimit1",       iocshArgInt};
static const iocshArg bpmccConfigureArg12={ "EEBIskewLimit",    iocshArgInt};
static const iocshArg bpmccConfigureArg13={ "EEBIcurrentThreshold",iocshArgInt};
static const iocshArg *bpmccConfigureArgs[] = {
                    &bpmccConfigureArg0, &bpmccConfigureArg1,
                    &bpmccConfigureArg2, &bpmccConfigureArg3,
                    &bpmccConfigureArg4, &bpmccConfigureArg5,
                    &bpmccConfigureArg6, &bpmccConfigureArg7,
                    &bpmccConfigureArg8, &bpmccConfigureArg9,
                    &bpmccConfigureArg10,&bpmccConfigureArg11,
                    &bpmccConfigureArg12,&bpmccConfigureArg13};
static const iocshFuncDef bpmccConfigureFuncDef =
                                     {"bpmccConfigure", 14, bpmccConfigureArgs};
static void bpmccConfigureCallFunc(const iocshArgBuf *args)
{
    bpmccConfigure(args[0].sval, args[1].sval,
                   args[2].ival, args[3].ival, args[4].ival, args[5].ival,
                   args[6].ival, args[7].ival, args[8].ival, args[9].ival,
                   args[10].ival,args[11].ival,args[12].ival,args[13].ival);
}

static void
bpmcc_RegisterCommands(void)
{
    iocshRegister(&bpmccConfigureFuncDef, bpmccConfigureCallFunc);
}
epicsExportRegistrar(bpmcc_RegisterCommands);
