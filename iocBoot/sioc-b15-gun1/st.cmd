#!../../bin/linuxRT-x86_64/scllrf
# Used to run as:
#!../../bin/rhel6-x86_64/scllrf
## You may have to change scllrf to something else
## everywhere it appears in this file

< envPaths

# System Location:
epicsEnvSet("AREA","B15")
# Hardware type [PRC, RFS, RES, INT]
epicsEnvSet("CHASSIS_TYPE","GUNB")
# Number within location and type: 1, 2, 3...
epicsEnvSet("N","1")
# PV prefix. SLAC standard is $(CHASSIS_TYPE):$(AREA):$(N):
epicsEnvSet("P", "GUN$(N):$(AREA):")
epicsEnvSet("IOC_PV", "SIOC:$(AREA):$(CHASSIS_TYPE)$(N):")
# IP address of hardware
epicsEnvSet( FPGA_IP, "192.168.165.68")
# trying out feedsim
#epicsEnvSet( FPGA_IP, "esd-pc80849")
# UDP port number. 50006 for most, 7 for echo test interface, 3000 for cmoc, 50000 BMB7 loopback
epicsEnvSet( PORT, "50006")
# Command to set up python environment, only used in LinuxRT
#epicsEnvSet( GO_PY, "source ~/cpu-b15-rf01/GoPython.sh;") # for RT
#epicsEnvSet( GO_PY, "") # for RHEL
# RHEL needs the full path to the executable. LinuxRT doesn't, so PY_PATH can be blank
epicsEnvSet( PY_PATH, "/afs/slac/g/lcls/package/python/python2.7.9/linux-x86_64/bin/") # for RHEL
#epicsEnvSet( PY_PATH, "") # for RT
# Directory with python init script, and where it will be run from
epicsEnvSet( PY_INIT_DIR, "/afs/slac/g/lcls/package/llrf_doolittle/b15/lcls2_llrf/firmware/gun/run")
# Bit file name, as relative path from the above directory
epicsEnvSet( BIT_FILE, "../gun.12-20-17.bit")
# If this chassis has a subclass, by convention called extra, set its name
# here so that scllrf$(CHASSIS_TYPE)$(EXTRA)Configure( "myReg","myIP") resolves correctly
epicsEnvSet( EXTRA, "Extra")
# This will work for the gun, which is not sc.
epicsEnvSet( SC, "")


< ../common/generalInit.cmd
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
####XXXX A bit of a hack to get the right iocAdmin PVs

< iocBoot/common/iocAdmin.cmd
< iocBoot/common/autoSaveConf.cmd

# =====================================================================
#Load Additional databases:
# =====================================================================
dbLoadRecords("db/GUNBExtra.db","P=$(P),PORT=myReg")
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
seq PVramp, "PREFIX=$(P)"

< iocBoot/common/autoSaveStart.cmd

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
#dbpf $(P)RUN_STOP.HIGH 0.11
dbpf $(P)RUN_STOP 1
epicsThreadSleep(0.2)
asynSetTraceMask("myIP",-1,1)
asynSetTraceMask("myReg",-1,1)

