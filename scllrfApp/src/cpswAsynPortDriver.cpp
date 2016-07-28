/*
 * cpswAsynDriver.cpp
 * 
 * CPSW asyn driver base class that inherits from the asynPortDriver
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>

#include <sys/types.h>
#include <sys/stat.h>


#include <epicsTypes.h>
#include <epicsTime.h>
#include <epicsThread.h>
#include <epicsString.h>
#include <epicsTimer.h>
#include <epicsMutex.h>
#include <epicsEvent.h>
#include <iocsh.h>

#include "cpswAsynPortDriver.h"
#include <cpsw_api_user.h>
#include <epicsExport.h>

#include <asynOctetSyncIO.h>


static const char *driverName="cpswAsynDriver";
static void asynPollerC(void *drvPvt);


/** Constructor for the cpswAsynDriver class.
  * Calls constructor for the asynPortDriver base class.
  * \param[in] portName The name of the asyn port driver to be created.
  * \param[in] path The path to the peripherial as built by the builder api
  * \param[in] nelms The number of elements of this device (max addr)
  * \paarm[in] nEntries The number of asyn params to be created for each device
  *
  * */
cpswAsynDriver::cpswAsynDriver(const char *portName, Path p, int nelms, int nEntries) 
   : asynPortDriver(portName, 
                    0, /* maxAddr */ 
                    (nEntries + 1),
                    asynInt32Mask | asynFloat64Mask | asynOctetMask | asynDrvUserMask | asynInt32ArrayMask, /* Interface mask */
                    asynInt32Mask | asynFloat64Mask | asynOctetMask | asynEnumMask | asynInt32ArrayMask,  /* Interrupt mask */
                    1, /* asynFlags.  This driver does block and it is not multi-device, so flag is 1 */
                    1, /* Autoconnect */
                    0, /* Default priority */
                    0), /* Default stack size*/   
                    shuttingDown_(0) 
{
    asynStatus status;
    int i;
    const char *functionName = "cpswAsynDriver";

    ScalVals.reserve(nEntries + 1);
    ScalVals.resize(nEntries + 1);
    
    asynPortDriver::createParam(syncFromDevString, asynParamInt32, &p_syncFromDev);

    pollEventId_ = epicsEventMustCreate(epicsEventEmpty);
    path_ = p;
}

/** Called when asyn clients call pasynInt32->write().
  * For all parameters it sets the value in the parameter library and calls any registered callbacks..
  * \param[in] pasynUser pasynUser structure that encodes the reason and address.
  * \param[in] value Value to write. */
asynStatus cpswAsynDriver::writeInt32(asynUser *pasynUser, epicsInt32 value)
{
    int function = pasynUser->reason;
    int addr=0;
    asynStatus status = asynSuccess;
    const char *paramName;
    const char* functionName = "writeInt32";

    uint32_t u32 = (uint32_t) value;
    status = getAddress(pasynUser, &addr); if (status != asynSuccess) return(status);
    
    /* Fetch the parameter string name for possible use in debugging */
    getParamName(function, &paramName);
    if( function == p_syncFromDev ) {
        status = ScalValsToParam();
    }
    else if(ScalVals[function] != NULL) {
        try {
        ScalVals[function]->setVal( &u32, 1 );
        } catch (CPSWError &e ) {
            printf("CPSW Error: %s\n", e.getInfo().c_str());
            status = asynError;
        }
    }
    if (status != asynSuccess) return (status);

    setIntegerParam(addr, function, value); 

    /* Do callbacks so higher layers see any changes */
    status = (asynStatus) callParamCallbacks();
    
    if (status) 
        epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize, 
                  "%s:%s: status=%d, function=%d, name=%s, value=%d", 
                  driverName, functionName, status, function, paramName, value);
    else        
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
              "%s:%s: function=%d, name=%s, value=%d\n", 
              driverName, functionName, function, paramName, value);
    return status;
}

/** Called when asyn clients call pasynInt32->read().
  * \param[in] pasynUser pasynUser structure that encodes the reason and address.
  * \param[in] value Pointer to the value to read. */
asynStatus cpswAsynDriver::readInt32(asynUser *pasynUser, epicsInt32 *value)
{
    int function = pasynUser->reason;
    int addr = 0;
    asynStatus status = asynSuccess;
    epicsTimeStamp timeStamp; getTimeStamp(&timeStamp);
    const char *functionName = "readInt32";
    const char *paramName;
    
    uint32_t u32;

    status = getAddress(pasynUser, &addr); if (status != asynSuccess) return(status);

    /* Fetch the parameter string name for possible use in debugging */
    getParamName(function, &paramName);
    if (ScalVals[function] != NULL) {
        scalValToIntegerParam(function, value);
    }
    if (status != asynSuccess) return (status);
    
    /* Set the timestamp */
    pasynUser->timestamp = timeStamp;


    if (status) 
        epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize, 
                  "%s:%s: status=%d, function=%d", 
                  driverName, functionName, status, function);
    else        
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
              "%s:%s: function=%d\n", 
              driverName, functionName, function);
    return status;
}

/** Called when asyn clients call pasynOctet->read().
  * \param[in] pasynUser pasynUser structure that encodes the reason and address.
  * \param[in] value Pointer to the value to read. 
  * \param[in] maxChars The size of the buffer
  * \param[in] nActual The number of characters actually read
  * \param[in] eomReason End of message reason, why the read was terminated */
asynStatus cpswAsynDriver::readOctet(asynUser *pasynUser, char *value, size_t maxChars,
                                        size_t *nActual, int *eomReason)
{
    int function = pasynUser->reason;
    int addr = 0;
    asynStatus status = asynSuccess;
    epicsTimeStamp timeStamp; getTimeStamp(&timeStamp);
    const char *functionName = "readOctet";
    const char *paramName;
    
//    status = getAddress(pasynUser, &addr); if (status != asynSuccess) return(status);
    getAddress(pasynUser, &addr);
    /* Fetch the parameter string name for possible use in debugging */
    getParamName(function, &paramName);
    if(ScalVals[function] != NULL) {
        try { 
            *nActual = (size_t) ScalVals[function]->getVal( (uint8_t*)value, maxChars );
        } catch (CPSWError &e ) {
            printf("CPSW Error: %s\n", e.getInfo().c_str());
            status = asynError;
         }
    }
    if (status != asynSuccess) return (status);
    /* Set the timestamp */
    setStringParam(addr, function, value); 

    /* Do callbacks so higher layers see any changes */
    status = (asynStatus) callParamCallbacks();
    pasynUser->timestamp = timeStamp;

    if (status) 
        epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize, 
                  "%s:%s: status=%d, function=%d", 
                  driverName, functionName, status, function);
    else        
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
              "%s:%s: function=%d\n", 
              driverName, functionName, function);
    return status;
}

/* Called when asyn clients call pasynInt32Array->read().
 * The base class implementation simply prints an error message.  
 * Derived classes may reimplement this function if required.
 * \param[in] pasynUser pasynUser structure that encodes the reason and address.
 * \param[in] value Pointer to the array to read.
 * \param[in] nElements Number of elements to read.
 * \param[out] nIn Number of elements actually read. */

asynStatus cpswAsynDriver::readInt32Array(asynUser *pasynUser, epicsInt32 *value, size_t nElements, size_t *nIn)
{
    int function = pasynUser->reason;
    int addr = 0;
    asynStatus status = asynSuccess;
    epicsTimeStamp timeStamp; getTimeStamp(&timeStamp);
    const char *functionName = "readInt32Array";
    const char *paramName;
//    status = getAddress(pasynUser, &addr); if (status != asynSuccess) return(status);
    getAddress(pasynUser, &addr);
    /* Fetch the parameter string name for possible use in debugging */
    getParamName(function, &paramName);

    try {
        if (ScalVals[function] != NULL) {
            ScalVals[function]->getVal( (uint32_t *) value, nElements );
        }
    }
    catch (CPSWError &e) {
        printf("CPSW Error: %s\n", e.getInfo().c_str());
        status = asynError;
    }

    /* Do callbacks so higher layers see any changes */
    status = (asynStatus) callParamCallbacks();
    pasynUser->timestamp = timeStamp;

    if (status)
        epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize,
                  "%s:%s: status=%d, function=%d",
                  driverName, functionName, status, function);
    else
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER,
              "%s:%s: function=%d\n",
              driverName, functionName, function);

    return status;
}


asynStatus cpswAsynDriver::writeInt32Array(asynUser *pasynUser, epicsInt32 *value, size_t nElements)
{
    int function = pasynUser->reason;
    int addr = 0;
    asynStatus status = asynSuccess;
    epicsTimeStamp timeStamp; getTimeStamp(&timeStamp);
    const char *functionName = "readInt32Array";
    const char *paramName;
//    status = getAddress(pasynUser, &addr); if (status != asynSuccess) return(status);
    getAddress(pasynUser, &addr);
    /* Fetch the parameter string name for possible use in debugging */
    try {
        if (ScalVals[function] != NULL) {
            if (ScalVals[function]->getNelms() == nElements) {
                ScalVals[function]->setVal( (uint32_t *) value, nElements );
            }
            else {
                status = asynError;
            }
        }
    }
    catch (CPSWError &e) {
        printf("CPSW Error: %s\n", e.getInfo().c_str());
        status = asynError;
    }

    if (status)
        epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize,
                  "%s:%s: status=%d, function=%d",
                  driverName, functionName, status, function);
    else
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER,
              "%s:%s: function=%d\n",
              driverName, functionName, function);

    return status;
}

 
asynStatus cpswAsynDriver::createParam(          const char *name, asynParamType type, int *index, ScalVal(*create)(Path p))
{
    int list;
    asynStatus status;

    // All parameters lists support the same parameters, so add the parameter name to all lists
    for (list=0; list<this->maxAddr; list++) {
        status = createParam(list, name, type, index, create);
        if (status) return asynError;
    }
    return asynSuccess;
}

asynStatus cpswAsynDriver::createParam(int list, const char *name, asynParamType type, int *index, ScalVal(*create)(Path p))
{
    asynStatus status = asynSuccess;
    asynPortDriver::createParam(list, name, type, index);
    Path pOut;
    try {
       pOut = path_->findByName(name);
       ScalVal sval = IScalVal::create(pOut);
       ScalVals[*index] = sval;
    } catch (CPSWError &e) {
       printf("%s: CPSWError: %s\n", name, e.getInfo().c_str());     
       return asynError;
    }
    return status;
}

/** Starts the poller thread.
 ** Derived classes will typically call this at near the end of their constructor.
 ** Derived classes can typically use the base class implementation of the poller thread,
 ** but are free to reimplement it if necessary.
 ** \param[in] pollPeriod The time between polls. */
asynStatus cpswAsynDriver::startPoller(double pollPeriod)
{
  pollPeriod_ = pollPeriod;
  epicsThreadCreate("asynPoller", 
                    epicsThreadPriorityLow,
                    epicsThreadGetStackSize(epicsThreadStackMedium),
                    (EPICSTHREADFUNC)asynPollerC, (void *)this);                
  return asynSuccess;
}      

/** Wakes up the poller thread to make it start polling. */
asynStatus cpswAsynDriver::wakeupPoller()
{ 
  epicsEventSignal(pollEventId_);                                                                         
  return asynSuccess;
} 
  
/** Polls the device.
 ** The base class cpswPoller thread calls this method once.
 ** This base class implementation does nothing.  Derived classes can implement this method if there
 ** are parameters that need to be polled. */
asynStatus cpswAsynDriver::poll()    
{                                                                        
  return asynSuccess;
} 
  
static void asynPollerC(void *drvPvt)
{ 
  cpswAsynDriver *pCpswDriver = (cpswAsynDriver*)drvPvt;      
  pCpswDriver->asynPoller();                                                              
} 

void cpswAsynDriver::asynPoller()
{
  double timeout;
  int status;

  timeout = pollPeriod_;
  wakeupPoller();  /* Force on poll at startup */

  while(1) {
    if (timeout != 0.) status = epicsEventWaitWithTimeout(pollEventId_, timeout);
    else               status = epicsEventWait(pollEventId_);
    if (status == epicsEventWaitOK) {
 /* We got an event, rather than a timeout.  This is because other software
 ** knows that we should do a poll.
 **/
    }
    lock();
    if (shuttingDown_) {
      unlock();
      break;
    }
    poll();
    unlock();
  }
}

/** Get a scalVal and set the asyn param in the param library
 *
 * \param[in]  function The integer function from asyn param library
 * \param[out] value    The int32 value returned from ScalVal
 *
 * */
asynStatus cpswAsynDriver::scalValToIntegerParam(int function, epicsInt32 *value)
{
    asynStatus status = asynSuccess;
    uint32_t u32;
    lock();
    try {
        ScalVals[function]->getVal( &u32, 1 );
    }
    catch(CPSWError &e) {
        status = asynError;
        goto bail;
    }
    *value = (epicsInt32) u32;
    setIntegerParam(0, function, *value);
    callParamCallbacks();
bail:
    unlock();
    return status;
}

/** Get all ScalVals, set asyn params, do callbacks */

asynStatus cpswAsynDriver::ScalValsToParam()
{
    asynStatus status = asynSuccess;
    epicsInt32 value;
    //for( std::vector<ScalVal>::iterator it = ScalVals.begin(); it != ScalVals.end(); it++ ) {
    for( unsigned int i = 0; i < ScalVals.size(); i++ ) {
        if( ScalVals[i] != NULL )
            status = scalValToIntegerParam(i, &value);
    }

    return status;
}

/** Get a scalVal and set the asyn param in the param library
 *
 * \param[in]  function The integer function from asyn param library
 * \param[out] value    The int32 value returned from the param library
 */ 
asynStatus cpswAsynDriver::integerParamToScalVal(int function, epicsInt32 *value)
{
    asynStatus status = asynSuccess;
    uint32_t u32;
    lock();
    status = getIntegerParam(0, function, value);
    if (status != asynSuccess)
        goto bail;
    u32 = (uint32_t) *value;
    try {
        ScalVals[function]->setVal( &u32, 1);
    }
    catch(CPSWError &e) {
        status = asynError;
        goto bail;
    }
bail:
    unlock();
    return status;
}

extern "C" {

/** EPICS iocsh callable function to call constructor for the cpswAsynDriver class.
  * \param[in] portName The name of the asyn port driver to be created.
  * \param[in] maxPoints The maximum  number of points in the volt and time arrays */
int cpswAsynDriverConfigure(const char *portName, const char *path)
{
    Path p;
    int nelms = 1;
    int nentries = 0;

    try {
//        Path p = root->findByName( path );
    } catch (CPSWError &e ) {
        printf("CPSW Error: %s\n", e.getInfo().c_str());
    }
    new cpswAsynDriver(portName, p, nelms, nentries);
    return(asynSuccess);
}


/* EPICS iocsh shell commands */

static const iocshArg initArg0 = { "portName",iocshArgString};
static const iocshArg initArg1 = { "path",iocshArgString};
static const iocshArg * const initArgs[] = {&initArg0,
                                            &initArg1};
static const iocshFuncDef initFuncDef = {"cpswAsynDriverConfigure",2,initArgs};
static void initCallFunc(const iocshArgBuf *args)
{
    cpswAsynDriverConfigure(args[0].sval, args[1].sval);
}

void cpswAsynDriverRegister(void)
{
    iocshRegister(&initFuncDef,initCallFunc);
}

epicsExportRegistrar(cpswAsynDriverRegister);

}

