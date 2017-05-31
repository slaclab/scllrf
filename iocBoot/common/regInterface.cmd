# ====================================================================
# Setup some additional environment variables
# ====================================================================
# Setup environment variables

# tag log messages with IOC name
# How to escape the "vioc-dev" as the PERL program
# will try to replace it.
# So, uncomment the following and remove the backslash
epicsEnvSet("EPICS\_IOC\_LOG_CLIENT_INET","${IOC}")

# Need this path to EPICS BASE so that caRepeater can be started:
# Let's figure out a way to pass this one in via the IOC's
# initial startup.cmd: another job for hookIOC.py :)
# Not needed caRepeater is started up by laci for all IOCs at
# CPU boot up.
epicsEnvSet(PATH,"${EPICS_BASE}/bin/$(ARCH)")

# ========================================================
# Support Large Arrays/Waveforms; Number in Bytes
# Please calculate the size of the largest waveform
# that you support in your IOC.  Do not just copy numbers
# from other apps.  This will only lead to an exhaustion
# of resources and problems with your IOC.
# The default maximum size for a channel access array is
# 16K bytes.
# ========================================================
# Uncomment and set appropriate size for your application:
#epicsEnvSet("EPICS_CA_MAX_ARRAY_BYTES", "800000")

# END: Additional environment variables
# ====================================================================

cd ${TOP}
# ====================================================
## Register all support components
dbLoadDatabase("dbd/scllrf.dbd",0,0)
scllrf_registerRecordDeviceDriver(pdbbase)
# ====================================================

# =====================================================================
# Set some facility specific MACROs for database instantiation below
# This is in effort to make IOCs Applications facility agnostic
# Some of the following variables may be defined in
# $IOC/<cpuName>/<epicsIOCName>/iocStartup.cmd
# =====================================================================
# Override the TOP variable set by envPaths:
# This is now past in via $IOC/<cpuName>/<epicsIOCName>/iocStartup.cmd
#epicsEnvSet(TOP,"${IOC_APP}")

# Set up communication with FPGA
drvAsynIPPortConfigure("myIP","$(FPGA_IP):$(PORT) UDP")
dbLoadRecords("db/asynRecord.db","P=$(P),R=ASYN_IP,PORT=myIP,ADDR=0,IMAX=0,OMAX=0")


#epicsThreadSleep(1.0)
scllrf$(TYPE)$(EXTRA)Configure( "myReg","myIP")
dbLoadRecords("db/asynRecord.db","P=$(P),R=ASYN_REG,PORT=myReg,ADDR=0,IMAX=0,OMAX=0")

epicsThreadSleep(0.2)

# =====================================================================
#Load Additional databases:
# =====================================================================
## Load record instances
dbLoadRecords("db/scllrfCommon.template", "TYPE=$(TYPE),P=$(P),PORT=myReg")
