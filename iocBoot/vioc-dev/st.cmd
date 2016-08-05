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
epicsEnvSet(TOP,"${IOC_APP}")

# ============================================
# Set MACROS for EVRs
# ============================================
# FAC = SYS0 ==> LCLS1
# FAC = SYS1 ==> FACET

# System Location:
epicsEnvSet("LOCA","B34")

#epicsEnvSet(EVR_DEV1,"EVR:B34:EVXX")
#epicsEnvSet(UNIT,"EVXX")
#epicsEnvSet(FAC,"SYS0")

#epicsEnvSet(EVR_DEV2,"EVR:B34:EVYY")
#epicsEnvSet(UNIT,"EVYY")
#epicsEnvSet(FAC,"SYS0")
# ===========================================

# Need this path to EPICS BASE so that caRepeater can be started:
# Let's figure out a way to pass this one in via the IOC's
# initial startup.cmd: another job for hookIOC.py :)
# Not needed caRepeater is started up by laci for all IOCs at
# CPU boot up.
#epicsEnvSet(PATH,"${EPICS_BASE}/bin/linuxRT-x86_64")

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
# ======================================================
# Init PMC EVR: To support Timing System Event Receiver
# ======================================================
# In this case this EVR is running in a PC under linux

# =====================================================================
# Setup for EVR:
# =====================================================================
#Add ErConfigure for each EVR before iocInit.
#
#    VME:      ErConfigure(<instance>,<address>,<vector>,<level>,0) or
#              ErConfigureWithSlot(<instance>,<address>,<vector>,<level>,0,<starting slot>)
#    PMC:      ErConfigure(<instance>,    0    ,    0   ,   0   ,1)
#    Embedded: ErConfigure(<instance>,    0    ,<vector>,<level>,2)
#
#    PCI-E:    ErConfigure(<instance>,    0    ,    0   ,   0   ,4)
#
#    where instance = EVR instance, starting from 0, incrementing by 1
#                     for each subsequent card.  Only 1 EVR instance
#                     is allowed for Embedded EVRs.
#    and   address  = VME card address, starting from 0x300000,
#                     incrementing by 0x100000 for each subsequent card
#                     (0 for PMC and Embedded)
#    and   vector   = VME or Embedded interrupt vector.
#                     For VME, start from 0x60 and increment by 0x02 for
#                     each subsequent card.
#                     (0 for PMC)
#    and   level    = VME or Embedded interrupt level.
#                     For VME, set to 4.  Can be the same for all EVRs.
#                     (0 for PMC)
#    and   0        = VME
#       or 1        = PMC
#       or 2        = Embedded
#
#       or 4        = PCI-E
#
#    and starting slot = first VME slot to start looking for EVRs.
#                        Set to 0 if their is only one CPU in the crate.
#                        (0 for PMC)
# ======================================================================
# Debug interest level for EVR Driver
# ErDebugLevel(0)

# PMC-based EVR (EVR230)
# These are the most popular
#ErConfigure(0, 0, 0, 0, 1)

# PCIe-based EVR (EVR300)
# For Industrial PCs, these desired.
#ErConfigure(0, 0, 0, 0, 4)

# Add evrInitialize (after ErConfigure) if a fiducial routine will be
# registered before iocInit driver initialization:
#evrInitialize()
# evrInitialize() is not registered with EPICS ioc shell.
# So, we will need to use cexpsh() to execute it. Like so,:
# cexpsh("-c",'evrInitialize()')

# ======= EVR Setup Complete ============================================
#
###############################################################################
## BEGIN: Load the record databases
###############################################################################
## ============================================================================
## Load iocAdmin databases to support IOC Health and monitoring
## ============================================================================
## The MACRO IOCNAME should be defined via the IOCs top level, "iocStartup.cmd"
##  found in $IOC/<iocName>/<viocName>/iocStartup.cmd
## The name must according the SLAC ICD PV naming convention.
#dbLoadRecords("db/iocAdminSoft.db","IOC=${IOC_NAME}")
#dbLoadRecords("db/iocAdminScanMon.db","IOC=${IOC_NAME}")
#
## The following database is a result of a python parser
## which looks at RELEASE_SITE and RELEASE to discover
## versions of software your IOC is referencing
## The python parser is part of iocAdmin
#dbLoadRecords("db/iocRelease.db","IOC=${IOC_NAME}")
#
## =====================================================================
## Load database for autosave status
## =====================================================================
#dbLoadRecords("db/save_restoreStatus.db", "P=${IOC_NAME}:")
#
## ========================================================
## Load EVR Databases for the Timing system
## ===========================================================================
## Change the EVR and CRD MACROs to specify your EVR Device name and instance
## Also, change the following MACROs accordingly, LOCA; UNIT
## Change SYS MACRO to match your facility:
## SYS0 = LCLS 1
## SYS1 = FACET
## SYS6 = XTA
## ===========================================================================
##dbLoadRecords("db/Pattern.db","IOC=${IOC_NAME},SYS=${FAC}")
#
## Databases for the PMC EVR230
## Note the first instance of an EVR will inherit the unit number of the parent IOC.
## All additional EVR instances will have its unit number incremented by one.
## Hence,
## EVR device number one ==> EVR=EVR:B34:EV07
## EVR device number two ==> EVR=EVR:B34:EV08
##dbLoadRecords("db/EvrPmc.db","EVR=${EVR_DEV1},CRD=0,SYS=${FAC}")
##dbLoadRecords("db/PMC-trig.db","IOC=${IOC_NAME},LOCA=${LOCA},UNIT=${UNIT},SYS=${FAC}")
#
## The EVR300 can only be used in PCs not mTCA
## Databases for the PMC EVR300
## EVR device number one ==> EVR=EVR:B34:EV07
## EVR device number two ==> EVR=EVR:B34:EV08
##dbLoadRecords("db/EvrPci.db","EVR=${EVR_DEV1},SYS=${FAC}")
##dbLoadRecords("db/PCI-trig.db","IOC=${IOC_NAME},LOCA=${LOCA},UNIT=${UNIT},SYS=${FAC}")
#
## Support for Beam Synchronous Acquisition (BSA)
#
## BSA Data Source Simulator Databases
##dbLoadRecords("db/bsaSimulator.db","DEVICE=LEPTON")
#
## BSA Database for each data source from above
##dbLoadRecords("db/Bsa.db","DEVICE=LEPTON, ATRB=COUNTER")
##dbLoadRecords("db/Bsa.db","DEVICE=LEPTON, ATRB=SINE")
#
#
#
## ========================================================
#
## =====================================================================
##Load Additional databases:
## =====================================================================
### Load record instances
dbLoadRecords("db/cmocRegisters.db","P=CMOC,PORT=cmocReg")
dbLoadRecords("db/cmocWaveforms.db","P=CMOC,PORT=cmocReg")
#
## END: Loading the record databases
#########################################################################
#
## =====================================================================
### Begin: Setup autosave/restore
## =====================================================================
#
## ============================================================
## If all PVs don't connect continue anyway
## ============================================================
#save_restoreSet_IncompleteSetsOk(1)
#
## ============================================================
## created save/restore backup files with date string
## useful for recovery.
## ============================================================
#save_restoreSet_DatedBackupFiles(1)
#
## ============================================================
## Where to find the list of PVs to save
## ============================================================
## Where "/data" is an NFS mount point setup when linuxRT target 
## boots up.
#set_requestfile_path("/data/${IOC}/autosave-req")
#
## ============================================================
## Where to write the save files that will be used to restore
## ============================================================
#set_savefile_path("/data/${IOC}/autosave")
#
## ============================================================
## Prefix that is use to update save/restore status database
## records
## ============================================================
#save_restoreSet_UseStatusPVs(1)
#save_restoreSet_status_prefix("${IOC_NAME}:")
#
### Restore datasets
#set_pass0_restoreFile("info_settings.sav")
#set_pass1_restoreFile("info_settings.sav")

# =====================================================================
# End: Setup autosave/restore
# =====================================================================

# =====================================================================
# Channel Access Security:  
# This is required if you use caPutLog.
# Set access security file
# Load common LCLS Access Configuration File
< ${ACF_INIT}
epicsThreadSleep(1.0)
# Set up communication with CMOC

# Echo from cdlx11 configuration for testing
#epicsEnvSet( FPGA_IP, "134.79.216.36")
#drvAsynIPPortConfigure("cmocIP","$(FPGA_IP):7 UDP")

# Real CMOC configuration
epicsEnvSet( FPGA_IP, "192.168.1.2")
drvAsynIPPortConfigure("cmocIP","$(FPGA_IP):3000 UDP")

# ======================================================================
### Asyn Debugging #####################################################
# ======================================================================
## Asyn messages for DIGI_Serial16
asynSetTraceMask("cmocIP",-1,0xFF)
#asynSetTraceIOMask("cmocIP",-1,ASYN_TRACEIO_HEX)

#epicsThreadSleep(1.0)
scllrfAsynPortDriverConfigure( "cmocReg","cmocIP")

# ======================================================================
### Asyn Debugging #####################################################
# ======================================================================
## Asyn messages for DIGI_Serial16
asynSetTraceMask("cmocReg",-1,0xFF)
#asynSetTraceIOMask("cmocReg",-1,ASYN_TRACEIO_HEX)
#
#epicsThreadSleep(0.2)
asynSetTraceMask("cmocIP",-1,5)
asynSetTraceMask("cmocReg",-1,13)

# =============================================================
# Start EPICS IOC Process (i.e. all threads will start running)
# =============================================================
iocInit()
#
#
## =====================================================
## Turn on caPutLogging:
## Log values only on change to the iocLogServer:
#caPutLogInit("${EPICS_CA_PUT_LOG_ADDR}")
#caPutLogShow(2)
## =====================================================
#
### Start any sequence programs
##seq sncExample,"user=gwbrownHost"
#
#
### =========================================================================
### Start autosave routines to save our data
### =========================================================================
## optional, needed if the IOC takes a very long time to boot.
## epicsThreadSleep( 1.0)
#
#cd("/data/${IOC}/autosave-req")
#
## The following command makes the autosave request files 'info_settings.req',
## and 'info_positions.req', from information (info nodes) contained in all of
## the EPICS databases that have been loaded into this IOC.
#
#makeAutosaveFiles()
#create_monitor_set("info_settings.req",60,"")
#
## ===========================================================================
#
#cd ${START_UP}
## ===========================================================================
## Setup Real-time priorities after iocInit for driver threads
## ===========================================================================
#system("/bin/su root -c `pwd`/rtPrioritySetup.cmd")
#
#cd ${IOC_BOOT}
#
## An example of using the CEXP Shell:
## cexpsh("-c",'printf("hello\n")')

