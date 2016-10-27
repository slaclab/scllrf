#!../../bin/linux-x86_64/scllrf
# Later will run as:
#!../../bin/linuxRT-x86_64/scllrf
## You may have to change scllrf to something else
## everywhere it appears in this file

< envPaths

# ====================================================================
# Setup some additional environment variables
# ====================================================================
# Setup environment variables

# tag log messages with IOC name
# How to escape the "vioc-dev" as the PERL program
# will try to replace it.
# So, uncomment the following and remove the backslash
epicsEnvSet("EPICS\_IOC\_LOG_CLIENT_INET","${IOC}")

# =====================================================================
# Set some facility specific MACROs for database instantiation below
# This is in effort to make IOCs Applications facility agnostic
# Some of the following variables may be defined in
# $IOC/<cpuName>/<epicsIOCName>/iocStartup.cmd
# =====================================================================
# Override the TOP variable set by envPaths:
# This is now past in via $IOC/<cpuName>/<epicsIOCName>/iocStartup.cmd
#epicsEnvSet(TOP,"${IOC_APP}")

# System Location:
epicsEnvSet("LOCA","B34")

# Need this path to EPICS BASE so that caRepeater can be started:
# Let's figure out a way to pass this one in via the IOC's
# initial startup.cmd: another job for hookIOC.py :)
# Not needed caRepeater is started up by laci for all IOCs at
# CPU boot up.
epicsEnvSet(PATH,"${EPICS_BASE}/bin/linuxRT-x86_64")

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

###########################################################
# Initialize all hardware first                           #
###########################################################

##############################################################################
# BEGIN: Load the record databases
##############################################################################
# ============================================================================
# Load iocAdmin databases to support IOC Health and monitoring
# ============================================================================
# The MACRO IOCNAME should be defined via the IOCs top level, "iocStartup.cmd"
#  found in $IOC/<iocName>/<viocName>/iocStartup.cmd
# The name must according the SLAC ICD PV naming convention.
dbLoadRecords("db/iocAdminSoft.db","IOC=${IOC}")
dbLoadRecords("db/iocAdminScanMon.db","IOC=${IOC}")

# The following database is a result of a python parser
# which looks at RELEASE_SITE and RELEASE to discover
# versions of software your IOC is referencing
# The python parser is part of iocAdmin
dbLoadRecords("db/iocRelease.db","IOC=${IOC}")

# =====================================================================
# Load database for autosave status
# =====================================================================
dbLoadRecords("db/save_restoreStatus.db", "P=${IOC}:")

# =====================================================================
#Load Additional databases:
# =====================================================================
## Load record instances
dbLoadRecords("db/PRCRegisters.db","P=PRC,PORT=prcReg")
#dbLoadRecords("db/cmocWaveforms.db","P=PRC,PORT=prcReg")
#
# END: Loading the record databases
########################################################################

# =====================================================================
## Begin: Setup autosave/restore
# =====================================================================

# ============================================================
# If all PVs don't connect continue anyway
# ============================================================
#save_restoreSet_IncompleteSetsOk(1)

# ============================================================
# created save/restore backup files with date string
# useful for recovery.
# ============================================================
#save_restoreSet_DatedBackupFiles(1)

# ============================================================
# Where to find the list of PVs to save
# ============================================================
# Where "/data" is an NFS mount point setup when linuxRT target 
# boots up.
#set_requestfile_path("data/${IOC}/autosave-req")

# ============================================================
# Where to write the save files that will be used to restore
# ============================================================
#set_savefile_path("data/${IOC}/autosave")

# ============================================================
# Prefix that is use to update save/restore status database
# records
# ============================================================
#save_restoreSet_UseStatusPVs(1)
#save_restoreSet_status_prefix("${IOC}:")

## Restore datasets
set_pass0_restoreFile("info_settings.sav")
set_pass1_restoreFile("info_settings.sav")

# =====================================================================
# End: Setup autosave/restore
# =====================================================================

# =====================================================================
# Channel Access Security:  
# This is required if you use caPutLog.
# Set access security file
# Load common LCLS Access Configuration File
#< ${ACF_INIT}
#epicsThreadSleep(1.0)
# Set up communication with PRC

# Echo from cdlx11 configuration for testing
#epicsEnvSet( FPGA_IP, "134.79.216.36")
#drvAsynIPPortConfigure("prcIP","$(FPGA_IP):7 UDP")

# Real PRC configuration
epicsEnvSet( FPGA_IP, "192.168.165.48")
drvAsynIPPortConfigure("prcIP","$(FPGA_IP):50006 UDP")
dbLoadRecords("db/asynRecord.db","P=PRC1:CMTS,R=ASYN_IP,PORT=prcIP,ADDR=0,IMAX=0,OMAX=0")

# ======================================================================
### Asyn Debugging #####################################################
# ======================================================================
## Asyn messages for DIGI_Serial16
asynSetTraceMask("prcIP",-1,0xB)
#asynSetTraceIOMask("prcIP",-1,ASYN_TRACEIO_HEX) ASYN_TRACEIO_HEX = 4
asynSetTraceIOMask("prcIP",-1,4)

#epicsThreadSleep(1.0)
scllrfPRCConfigure( "prcReg","prcIP")
dbLoadRecords("db/asynRecord.db","P=PRC1:CMTS,R=ASYN_REG,PORT=prcReg,ADDR=0,IMAX=0,OMAX=0")


# ======================================================================
### Asyn Debugging #####################################################
# ======================================================================
## Asyn messages for DIGI_Serial16
asynSetTraceMask("prcReg",-1,0xB)
#asynSetTraceIOMask("prcReg",-1,ASYN_TRACEIO_HEX) ASYN_TRACEIO_HEX = 4
asynSetTraceIOMask("prcReg",-1,4)
#
epicsThreadSleep(0.2)

# =============================================================
# Start EPICS IOC Process (i.e. all threads will start running)
# =============================================================
iocInit()
#
#
# =====================================================
# Turn on caPutLogging:
# Log values only on change to the iocLogServer:
caPutLogInit("${EPICS_CA_PUT_LOG_ADDR}")
caPutLogShow(2)
# =====================================================

## Start any sequence programs
#seq sncExample,"user=gwbrownHost"


## =========================================================================
## Start autosave routines to save our data
## =========================================================================
# optional, needed if the IOC takes a very long time to boot.
# epicsThreadSleep( 1.0)


# The following command makes the autosave request files 'info_settings.req',
# and 'info_positions.req', from information (info nodes) contained in all of
# the EPICS databases that have been loaded into this IOC.

#cd("data/${IOC}/autosave-req")
#makeAutosaveFiles()
#cd("../../..")
#create_monitor_set("info_settings.req",60,"")

# ===========================================================================

#cd ${START_UP}
# ===========================================================================
# Setup Real-time priorities after iocInit for driver threads
# ===========================================================================
#
#system("/bin/su root -c `pwd`/rtPrioritySetup.cmd")


# An example of using the CEXP Shell:
# cexpsh("-c",'printf("hello\n")')

asynSetTraceMask("prcIP",-1,1)
asynSetTraceMask("prcReg",-1,1)
dbpf("PRC:RUN_STOP.HIGH", 5)
dbpf("PRC:POLL_PERIOD", 0.5)
dbpf("PRC:DIG_DSP_TRACE_KEEP_W", 5)
epicsThreadSleep 1
dbpf("PRC:RUN_STOP", 1)
epicsThreadSleep 5


