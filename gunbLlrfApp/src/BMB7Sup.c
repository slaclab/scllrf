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

#include "BMB7Sup.h"

/*
 * ASYN subaddresses
 * Augment protocol command groups
 */
//#define A_HI_STATISTICS         0x0E00

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
    asynInterface        asynInt32;
    void                *asynInt32InterruptPvt;
    asynInterface        asynUInt32Digital;
    void                *asynUInt32DigitalInterruptPvt;

    /*
     * Link to lower ports
     */
    portLink             cmdLink;
    unsigned int         identifier;
    int                  cellInfo;
    portLink             monitorLink;


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


/*
 * asynUInt32Digital methods -- there are none.
 * Everything is handled in SCAN="I/O Intr" callback.
 */
static asynUInt32Digital uint32DigitalMethods;

/*
 * asynInt32 methods
 */


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
int32Read(void *pvt, asynUser *pasynUser, epicsInt32 *value)
{
    drvPvt *pdpvt = (drvPvt *)pvt;
    asynStatus status;
    int address;

    if ((status = pasynManager->getAddr(pasynUser, &address)) != asynSuccess)
        return status;

    status = crankBMB7monitor(pasynUser, pdpvt);
    return status;
}

static asynInt32 int32Methods = { NULL, int32Read };

/*
 * Create a new lower port and set up a link to it
 */
static asynStatus
setLink(drvPvt *pdpvt, const char *address, const char *ext, portLink *link, int udpPort, int priority)
{
    asynStatus status;

    link->portName = callocMustSucceed(1, strlen(pdpvt->portName)+strlen(ext)+1, "bmb7Conf");
    sprintf(link->portName, "%s%s", pdpvt->portName, ext);
    link->hostInfo = callocMustSucceed(1, strlen(address)+20, "bmb7Conf");
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
bmb7Configure(const char *portName, const char *address,
               int priority)
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
    pdpvt = (drvPvt *)callocMustSucceed(1, sizeof(drvPvt), "bmb7Conf");
    pdpvt->portName = epicsStrDup(portName);


    /*
     * Create the ports that we'll use to communicate with the BMB7.
     */
    if ((setLink(pdpvt, address, "_MON", &pdpvt->monitorLink,
                                CC_MONITOR_UDP_PORT, priority) != asynSuccess))
        return;

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

}

/*
 * IOC shell command registration
 */
static const iocshArg bmb7ConfigureArg0 ={ "port",             iocshArgString};
static const iocshArg bmb7ConfigureArg1 ={ "address",          iocshArgString};
static const iocshArg bmb7ConfigureArg2 ={ "priority",         iocshArgInt};
static const iocshArg *bmb7ConfigureArgs[] = {
                    &bmb7ConfigureArg0, &bmb7ConfigureArg1,
                    &bmb7ConfigureArg2};
static const iocshFuncDef bmb7ConfigureFuncDef =
                                     {"bmb7Configure", 3, bmb7ConfigureArgs};
static void bmb7ConfigureCallFunc(const iocshArgBuf *args)
{
    bmb7Configure(args[0].sval, args[1].sval,
                   args[2].ival);
}

static void
bmb7_RegisterCommands(void)
{
    iocshRegister(&bmb7ConfigureFuncDef, bmb7ConfigureCallFunc);
}
epicsExportRegistrar(bmb7_RegisterCommands);
