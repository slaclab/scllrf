#!../../bin/linux-x86_64/scllrf
# Later will run as:
#!../../bin/linuxRT-x86_64/scllrf
## You may have to change scllrf to something else
## everywhere it appears in this file

< envPaths

epicsEnvSet(PATH,"/usr/csite/certified/bin/")

# Hardware type [PRC, RFS, RES, INT]
epicsEnvSet("TYPE","PRC")
# System Location:
epicsEnvSet("LOCA","CMTF")
# Number within location and type: 1, 2, 3...
epicsEnvSet("N","1")
# PV prefix. SLAC standard is $(TYPE):$(LOCA):$(N):
epicsEnvSet("P","$(TYPE)$(N):")
# IP address of hardware
epicsEnvSet( FPGA_IP, "nnn.nnn.nnn.nnn")
# UDP port number. 50006 for most, 7 for echo test interface, 3000 for cmoc
epicsEnvSet( PORT, "50006")

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

# =====================================================================
#Load Additional databases:
# =====================================================================
dbLoadRecords("db/scllrf$(TYPE)extra.template","P=$(P),PORT=myReg")
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
####XXXX - Set RUN_STOP to enable polling
dbpf $(P)RUN_STOP.HIGH 0
dbpf $(P)RUN_STOP 1
####XXXX - Wait for 0.2 seconds with heavy logging still enabled
epicsThreadSleep(0.2)
####XXXX - Crank down logging levels. Max logging levels hurt performance.
asynSetTraceMask("myIP",-1,1)
asynSetTraceMask("myReg",-1,1)

