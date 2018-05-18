#!../../bin/linuxRT-x86_64/scllrf
#!../../bin/rhel6-x86_64/scllrf
## You may have to change scllrf to something else
## everywhere it appears in this file

< envPaths
# PV name prefix parts in naming convention for Buncher
epicsEnvSet("DEVICE_TYPE", "ACCL")
epicsEnvSet("AREA","GUNB")
epicsEnvSet("POSITION", "455")
# Hardware type [PRC, RFS, RES, INT]
epicsEnvSet("CHASSIS_TYPE","GUNB")
# Number within location and type: 1, 2, 3...
epicsEnvSet("CHASSIS_NAME","RFS1")
epicsEnvSet("P", "$(DEVICE_TYPE):$(AREA):$(POSITION):$(CHASSIS_NAME):")
epicsEnvSet("IOC_PV", "SIOC:$(AREA):RF02")
# IP address of hardware
epicsEnvSet( FPGA_IP, "192.168.0.201")
# trying out feedsim
#epicsEnvSet( FPGA_IP, "esd-pc80849")
# UDP port number. 50006 for most, 7 for echo test interface, 3000 for cmoc, 50000 BMB7 loopback
epicsEnvSet( PORT, "50006")
# Command to set up python environment, only used in LinuxRT
#epicsEnvSet( GO_PY, "source ~/cpu-b15-rf01/GoPython.sh;") # for RT
#epicsEnvSet( GO_PY, "") # for RHEL
# RHEL needs the full path to the executable. LinuxRT doesn't, so PY_PATH can be blank
#epicsEnvSet( PY_PATH, "") # for RT
epicsEnvSet( PY_PATH, "/afs/slac/g/lcls/package/python/python2.7.9/linux-x86_64/bin/") # for RHEL
# Directory with python init script, and where it will be run from
epicsEnvSet( PY_INIT_DIR, "/afs/slac.stanford.edu/u/re/gwbrown/w/lcls2_llrf/firmware/gun/run")
# Bit file name, as relative path from the above directory
epicsEnvSet( BIT_FILE, "../gun.bit")
# The "chassis" register sets the type of chassis: 0:gun prc,1:gun rfs,2:buncher prc,3:buncher rfs
epicsEnvSet(CHASSIS_REG, "3")
# If this chassis has a subclass, by convention called extra, set its name
# here so that scllrf$(CHASSIS_TYPE)$(EXTRA)Configure( "myReg","$(P)myIP") resolves correctly
epicsEnvSet( EXTRA, "Extra")
# This will work for the gun, which is not sc.
epicsEnvSet( SC, "")

< ../common/generalInit.cmd
# regInterface.cmd leaves us in $(TOP) directory

< iocBoot/common/regInterface.cmd
asynSetTraceMask("$(CHASSIS_NAME)IP",-1,1)
asynSetTraceMask("$(CHASSIS_NAME)Reg",-1,1)

epicsEnvSet("CHASSIS_NAME","RFS2")
epicsEnvSet(CHASSIS_REG, "3")
epicsEnvSet("P", "$(DEVICE_TYPE):$(AREA):$(POSITION):$(CHASSIS_NAME):")
epicsEnvSet( FPGA_IP, "192.168.0.202")
< iocBoot/common/regInterface.cmd
asynSetTraceMask("$(CHASSIS_NAME)IP",-1,1)
asynSetTraceMask("$(CHASSIS_NAME)Reg",-1,1)

epicsEnvSet("CHASSIS_NAME","PRC")
epicsEnvSet(CHASSIS_REG, "2")
epicsEnvSet("P", "$(DEVICE_TYPE):$(AREA):$(POSITION):")
epicsEnvSet( FPGA_IP, "192.168.0.203")
< iocBoot/common/regInterface.cmd
asynSetTraceMask("$(CHASSIS_NAME)IP",-1,1)
asynSetTraceMask("$(CHASSIS_NAME)Reg",-1,1)

#dbLoadRecords("db/BuncherRfCalib.db")
dbLoadRecords("db/BuncherExtra.db")

##############################################################################
# BEGIN: Load the record databases
##############################################################################
####XXXX A bit of a hack to get the right iocAdmin PVs

< iocBoot/common/iocAdmin.cmd
< iocBoot/common/autoSaveConf.cmd

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
epicsEnvSet("P", "$(DEVICE_TYPE):$(AREA):$(POSITION):")
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

epicsThreadSleep(0.2)
epicsEnvSet("CHASSIS_NAME","RFS1")
epicsEnvSet("P", "$(DEVICE_TYPE):$(AREA):$(POSITION):$(CHASSIS_NAME):")
#dbpf $(P)CHASSIS_W 3
dbpf $(P)WAVE_SAMP_PER_W 32
#dbpf $(P)EXT_TRIG_SEL_W 1
dbpf $(P)POLL_PERIOD 0.4
dbpf $(P)RUN_STOP 1
#seq PVramp, "PREFIX=$(P)"

epicsThreadSleep(0.2)
epicsEnvSet("CHASSIS_NAME","RFS2")
epicsEnvSet("P", "$(DEVICE_TYPE):$(AREA):$(POSITION):$(CHASSIS_NAME):")
#dbpf $(P)CHASSIS_W 3
dbpf $(P)WAVE_SAMP_PER_W 32
#dbpf $(P)EXT_TRIG_SEL_W 1
dbpf $(P)POLL_PERIOD 0.4
dbpf $(P)RUN_STOP 1

epicsThreadSleep(0.2)
epicsEnvSet("CHASSIS_NAME","PRC")
epicsEnvSet("P", "$(DEVICE_TYPE):$(AREA):$(POSITION):")
#dbpf $(P)CHASSIS_W 2
dbpf $(P)WAVE_SAMP_PER_W 32
#dbpf $(P)EXT_TRIG_SEL_W 0
dbpf $(P)POLL_PERIOD 0.4
dbpf $(P)RUN_STOP 1
seq PVramp, "PREFIX=$(P)"
epicsThreadSleep(0.2)

