#!../../bin/linuxRT-x86_64/scllrf
#!../../bin/rhel6-x86_64/scllrf
## You may have to change scllrf to something else
## everywhere it appears in this file

< envPaths
# PV name prefix parts in naming convention for Buncher
epicsEnvSet("DEVICE_TYPE", "ACCL")
epicsEnvSet("AREA","GUNB")
epicsEnvSet("POSITION", "455")
iocLogInit()
iocLogPrefix("fac=${DEVICE_TYPE} proc=${IOC} ")
# Hardware type [PRC, RFS, RES, INT]
epicsEnvSet("CHASSIS_TYPE","GUNB")
# Number within location and type: 1, 2, 3...
epicsEnvSet("CHASSIS_NAME","RFS1")
epicsEnvSet("P", "$(DEVICE_TYPE):$(AREA):$(POSITION):$(CHASSIS_NAME):")
epicsEnvSet("IOC_PV", "SIOC:$(AREA):RF02")
# IP address of hardware
epicsEnvSet( FPGA_IP, "192.168.0.201")
# UDP port number. 50006 for most, 7 for echo test interface, 3000 for cmoc, 50000 BMB7 loopback
epicsEnvSet( PORT, "50006")
# Command to set up python environment, only used in LinuxRT
epicsEnvSet( GO_PY, "source ~/cpu-gunb-rf01/GoPython.sh;") # for RT
#epicsEnvSet( GO_PY, "") # for RHEL
# RHEL needs the full path to the executable. LinuxRT doesn't, so PY_PATH can be blank
epicsEnvSet( PY_PATH, "") # for RT
#epicsEnvSet( PY_PATH, "/usr/local/lcls/package/python/python2.7.9/linux-x86_64/bin/") # for RHEL
# Directory with python init script, and where it will be run from
epicsEnvSet( PY_INIT_DIR, "/usr/local/lcls/package/lcls2_llrf/firmware/gun/run")
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
epicsEnvSet("P", "$(DEVICE_TYPE):$(AREA):$(POSITION):$(CHASSIS_NAME):")
epicsEnvSet( FPGA_IP, "192.168.0.203")
< iocBoot/common/regInterface.cmd
asynSetTraceMask("$(CHASSIS_NAME)IP",-1,1)
asynSetTraceMask("$(CHASSIS_NAME)Reg",-1,1)

dbLoadRecords("db/BuncherExtra.db")
dbLoadRecords("db/BuncherStatHistory.db")

##############################################################################
# BEGIN: Load the record databases
##############################################################################

< iocBoot/common/iocAdmin.cmd
< iocBoot/common/autoSaveConf.cmd

# END: Loading the record databases
########################################################################

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
caPutLogInit("${EPICS_CA_PUT_LOG_ADDR}")
caPutLogShow(2)
# =====================================================

## Start any sequence programs
#seq sncExample,"user=gwbrownHost"
epicsEnvSet("P", "$(DEVICE_TYPE):$(AREA):$(POSITION):")
seq PVramp, "PREFIX=$(P)"
seq centerCwDetune, "PREFIX=ACCL:GUNB:455:,REV=ACCL:GUNB:455:REV:,FWD=ACCL:GUNB:455:FWD:,CAV=ACCL:GUNB:455:,CTL=ACCL:GUNB:455:PRC:DDSA_PHSTEP_H_W"

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

# Register initialization doesn't seem to play well with autosave or PINI, so init here.
epicsThreadSleep(0.2)
epicsEnvSet("CHASSIS_NAME","RFS1")
epicsEnvSet("P", "$(DEVICE_TYPE):$(AREA):$(POSITION):$(CHASSIS_NAME):")
dbpf $(P)KEEP_W 0xFFFF
dbpf $(P)DECAYKEEP_W 0xFFFF
# The lower 16 bits of interlock status are diagnostic, the upper indicate a real trip
dbpf $(P)INLK_STATUS_R.HIHI 0x10000
dbpf $(P)POLL_PERIOD 0.1
dbpf $(P)RUN_STOP 1
seq amd78xxmon, "PREFIX=$(P)"

epicsThreadSleep(0.2)
epicsEnvSet("CHASSIS_NAME","RFS2")
epicsEnvSet("P", "$(DEVICE_TYPE):$(AREA):$(POSITION):$(CHASSIS_NAME):")
dbpf $(P)KEEP_W 0xFFFF
dbpf $(P)DECAYKEEP_W 0xFFFF
# The lower 16 bits of interlock status are diagnostic, the upper indicate a real trip
dbpf $(P)INLK_STATUS_R.HIHI 0x10000
dbpf $(P)POLL_PERIOD 0.1
dbpf $(P)RUN_STOP 1
seq amd78xxmon, "PREFIX=$(P)"

epicsThreadSleep(0.2)
epicsEnvSet("CHASSIS_NAME","PRC")
epicsEnvSet("P", "$(DEVICE_TYPE):$(AREA):$(POSITION):$(CHASSIS_NAME):")
dbpf $(P)KEEP_W 0xFFFF
dbpf $(P)DECAYKEEP_W 0x00FF
# The lower 16 bits of interlock status are diagnostic, the upper indicate a real trip
dbpf $(P)INLK_STATUS_R.HIHI 0x10000
dbpf $(P)POLL_PERIOD 0.1
dbpf $(P)RUN_STOP 1
epicsThreadSleep(1)
seq amd78xxmon, "PREFIX=$(P)"

dbpf ACCL:GUNB:455:WF_TIME_PER_POINT 4
