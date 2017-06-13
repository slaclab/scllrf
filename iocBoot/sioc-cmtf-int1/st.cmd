#!../../bin/rhel-6-ia32/scllrf
# Later will run as:
#!../../bin/rhel-6-ia32/scllrf
## You may have to change scllrf to something else
## everywhere it appears in this file

< envPaths

#PV=SRFCMTFLLRETURN, iocsoftsrfcmtfcm, srfbat0 (srfl00), 129.57.200.99
#epicsEnvSet("EPICS_ADDR_LIST","129.57.231.255 129.57.200.99")

# System Location:
epicsEnvSet("LOCA","CMTF")
# Hardware type [PRC, RES, INT]
epicsEnvSet("TYPE","INT")
# Number within location and type: 1, 2, 3...
epicsEnvSet("N","1")
# PV prefix name
epicsEnvSet("P","INT1:")
# IP address of hardware
#................................epicsEnvSet( FPGA_IP, "SET IP ADDRESS HERE")
epicsEnvSet( FPGA_IP, "129.57.231.89")
# UDP port number. 50006 for most, 7 for echo test interface, 3000 for cmoc
epicsEnvSet( PORT, "50006")
# If this chassis has a subclass, by convention called extra, set its name
# here so that scllrf$(TYPE)$(EXTRA)Configure( "myReg","myIP") resolves correctly
epicsEnvSet( EXTRA, "")

< ../common/regInterface.cmd
# regInterface.cmd leaves us in $(TOP) directory

####XXXX Turn on heavy logging for development
# ======================================================================
### Asyn Debugging #####################################################
# ======================================================================
## Asyn messages for DIGI_Serial16
#asynSetTraceMask("myIP",-1,ASYN_TRACE_ERROR)
asynSetTraceMask("myIP",-1,0xB)
#asynSetTraceIOMask("myIP",-1,ASYN_TRACEIO_HEX) ASYN_TRACEIO_HEX = 4
asynSetTraceIOMask("myIP",-1,4)

# ======================================================================
### Asyn Debugging #####################################################
# ======================================================================
## Asyn messages for DIGI_Serial16
asynSetTraceMask("myReg",-1,0xB)
#asynSetTraceIOMask("myReg",-1,ASYN_TRACEIO_HEX) ASYN_TRACEIO_HEX = 4
asynSetTraceIOMask("myReg",-1,4)
####XXXX End Turn on heavy logging for development

##############################################################################
# BEGIN: Load the record databases
##############################################################################
#< iocBoot/common/iocAdmin.cmd
#< iocBoot/common/autoSaveConf.cmd

#
#
#
#
#
#
#
#
#
#
#ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
#
#ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
#
#ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
#
#ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
# System database:
#...<startup.system.db
dbLoadRecords("/usr/srfsite/op/prod_R3.14.12.3.J0/system/6-4/db/systemsoftrfslacint1.db")
#...<startup.iocAdminLib.db
dbLoadRecords("/usr/srfsite/op/prod_R3.14.12.3.J0/iocAdminLib/1-1/db/iocAdminLibsoftrfslacint1.db")
#ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
#
#ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
#
#ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
#
#ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
#
#
#
#
#
#
#
#
#
#


# =====================================================================
#Load Additional databases:
# =====================================================================
#dbLoadRecords("db/$(TYPE)extra.db","P=$(P),PORT=myReg")
#++++++++++++++ +++++++++ ++++++++++++ +++++++++++ +++++++++++++ ++++++++++++++
dbLoadRecords("db/ZZZextra.template","P=$(P)")
dbLoadRecords("db/STMPextra.template","P=$(P)")
dbLoadRecords("db/ARCTextra.template","P=$(P)")
dbLoadRecords("db/CPLTextra.template","P=$(P)")
dbLoadRecords("db/HELLextra.template","P=$(P)")
#++++++++++++++ +++++++++ ++++++++++++ +++++++++++ +++++++++++++ ++++++++++++++
#
# END: Loading the record databases
########################################################################

# =====================================================================
# Channel Access Security:  
# This is required if you use caPutLog.
# Set access security file
# Load common LCLS Access Configuration File
#< ${ACF_INIT}
#epicsThreadSleep(1.0)


# =============================================================
# Start EPICS IOC Process (i.e. all threads will start running)
# =============================================================
epicsEnvSet("EPICS_CA_ADDR_LIST","129.57.231.255 129.57.200.99")
pwd
iocInit()
#
#
# =====================================================
# Turn on caPutLogging:
# Log values only on change to the iocLogServer:
#caPutLogInit("${EPICS_CA_PUT_LOG_ADDR}")
#caPutLogShow(2)
# =====================================================

## Start any sequence programs
#seq sncExample,"user=gwbrownHost"
#++++++++++++++ +++++++++ ++++++++++++ +++++++++++ +++++++++++++ ++++++++++++++
#seq &INTmbboCopy,"PREFC=INT1"
seq &INTarcTest,"PREFC=INT1"
seq &INTheLevel,"PREFC=INT1"
#++++++++++++++ +++++++++ ++++++++++++ +++++++++++ +++++++++++++ ++++++++++++++

#< iocBoot/common/autoSaveStart.cmd

# ===========================================================================

#cd ${START_UP}
# ===========================================================================
# Setup Real-time priorities after iocInit for driver threads
# ===========================================================================
#
#system("/bin/su root -c `pwd`/rtPrioritySetup.cmd")


# An example of using the CEXP Shell:
# cexpsh("-c",'printf("hello\n")')

####XXXX Run a quick test, for dev only
dbpf $(TYPE)$(N):RUN_STOP.HIGH 0
dbpf $(TYPE)$(N):RUN_STOP 1
epicsThreadSleep(0.2)
asynSetTraceMask("myIP",-1,1)
asynSetTraceMask("myReg",-1,1)


#++++++++++++++ +++++++++ ++++++++++++ +++++++++++ +++++++++++++ ++++++++++++++
< /usr/devuser/lahti/RF_SLAC.2/scllrf/iocBoot/sioc-cmtf-int1/aslo.init
< /usr/devuser/lahti/RF_SLAC.2/scllrf/iocBoot/sioc-cmtf-int1/prec.init
#++++++++++++++ +++++++++ ++++++++++++ +++++++++++ +++++++++++++ ++++++++++++++


