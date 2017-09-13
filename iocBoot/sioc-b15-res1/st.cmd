#!../../bin/linuxRT-x86_64/scllrf
# Later will run as:
#!../../bin/linuxRT-x86_64/scllrf
## You may have to change scllrf to something else
## everywhere it appears in this file

< envPaths

# System Location:
epicsEnvSet("LOCA","B15")
# Hardware type [PRC, RFS, RES, INT]
epicsEnvSet("TYPE","RES")
# Number within location and type: 1, 2, 3...
epicsEnvSet("N","1")
# PV prefix. SLAC standard is $(TYPE):$(LOCA):$(N):
epicsEnvSet("P", "$(TYPE)$(N):$(LOCA):")
# IP address of hardware
epicsEnvSet( FPGA_IP, "192.168.165.70")
# UDP port number. 50006 for most, 7 for echo test interface, 3000 for cmoc
epicsEnvSet( PORT, "50006")
# If this chassis has a subclass, by convention called extra, set its name
# here so that scllrf$(TYPE)$(EXTRA)Configure( "myReg","myIP") resolves correctly
epicsEnvSet( EXTRA, "extra")

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
< iocBoot/common/iocAdmin.cmd
#< iocBoot/common/autoSaveConf.cmd

# =====================================================================
#Load Additional databases:
# =====================================================================
dbLoadRecords("db/$(TYPE)extra.db","P=$(P),PORT=myReg")
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
dbpf $(P)PIEZO_WAVE_KEEP_L 65535
dbpf $(P)PIEZO_WAVE_KEEP_H 65535
dbpf $(P)RUN_STOP.HIGH 0.10
dbpf $(P)RUN_STOP 1
dbpf $(P)MOTOR1_ACC 12.34
epicsThreadSleep(0.2)
asynSetTraceMask("myIP",-1,1)
asynSetTraceMask("myReg",-1,1)

