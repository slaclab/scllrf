/* cpswAsynDriver.h
 *
 * This file defines the base class for cpsw asyn drivers.  It uses the cpsw framework ScalVal
 * for register access.  It also has a pure virtual function poll to be implemented by any 
 * derived classes that need a cyclic task.
 *
 */

#ifndef CPSW_ASYN_DRIVER_H
#define CPSW_ASYN_DRIVER_H

#include "asynPortDriver.h"
#include <cpsw_api_user.h>

#include <epicsEvent.h>
#include <epicsTypes.h>


#define syncFromDevString    "SYNC_FROM_DEV"

/** Class that demonstrates the use of the asynPortDriver base class to greatly simplify the task
  * of writing an asyn port driver.
  * This class interfaces to the CPSW framework and uses the ScalVal interface for register access.
  */
class cpswAsynDriver : public asynPortDriver {
public:
    cpswAsynDriver(const char *portName, Path p, int nelms, int nEntries);
                 
    /* These are the methods that we override from asynPortDriver */
    virtual asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);
    virtual asynStatus readInt32(asynUser *pasynUser, epicsInt32 *value);
    virtual asynStatus readOctet(asynUser *pasynUser, char *value, size_t maxChars,
                                        size_t *nActual, int *eomReason);
    virtual asynStatus readInt32Array(asynUser *pasynUser, epicsInt32 *value,
                                        size_t nElements, size_t *nIn);
    virtual asynStatus writeInt32Array(asynUser *pasynUser, epicsInt32 *value,
                                        size_t nElements);

    virtual asynStatus createParam(          const char *name, asynParamType type, int *index, ScalVal(*create)(Path p));
    virtual asynStatus createParam(int list, const char *name, asynParamType type, int *index, ScalVal(*create)(Path p));

    /* These are the methods that are new to this class
     * They define a polling mechanism for derived classes */
    virtual asynStatus startPoller(double pollPeriod);
    virtual asynStatus wakeupPoller();
    virtual asynStatus poll();
    void asynPoller();


protected:
    /** cpsw path */
    Path path_;

    epicsEventId pollEventId_;    /**< Event ID to wake up poller */
    double pollPeriod_;           /**< The time between polls */
    int shuttingDown_;

    std::vector<ScalVal> ScalVals;

    virtual asynStatus scalValToIntegerParam(int function, epicsInt32 *value);
    virtual asynStatus integerParamToScalVal(int function, epicsInt32 *value);

    virtual asynStatus ScalValsToParam();
private:
    /* Our data */
    int p_syncFromDev;
};
#endif
