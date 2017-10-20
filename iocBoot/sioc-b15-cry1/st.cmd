#!../../bin/rhel6-x86_64/scllrf
# Later will run as:
#!../../bin/linuxRT-x86_64/scllrf
## You may have to change scllrf to something else
## everywhere it appears in this file

< envPaths

# System Location:
epicsEnvSet("LOCA","B15")
# Hardware type [PRC, RFS, RES, INT]
epicsEnvSet("TYPE","GUNB")
# Number within location and type: 1, 2, 3...
epicsEnvSet("N","0101")
# PV prefix. SLAC standard is $(TYPE):$(LOCA):$(N):
epicsEnvSet("P", "GUN$(N):$(LOCA):")
# IP address of hardware
epicsEnvSet( FPGA_IP, "192.168.165.69")
# UDP port number. 50006 for most, 7 for echo test interface, 3000 for cmoc
epicsEnvSet( PORT, "50006")
# If this chassis has a subclass, by convention called extra, set its name
# here so that scllrf$(TYPE)$(EXTRA)Configure( "myReg","myIP") resolves correctly
epicsEnvSet( EXTRA, "")
# This will work for the gun, which is not sc.
epicsEnvSet( SC, "")

< iocBoot/common/iocAdmin.cmd
#< iocBoot/common/autoSaveConf.cmd

# =====================================================================
#Load Additional databases:
# =====================================================================
dbLoadRecords("db/cryo-plc.db","CM=01,area=B15,plc=PLC-B15")
#
# END: Loading the record databases
########################################################################

## Initialize EtherIP driver and define PLC
drvEtherIP_init()
# drvEtherIP_define_PLC <name>, <ip_addr>, <slot>
drvEtherIP_define_PLC("PLC_B15","plc-b15", 1)
EIP_verbosity(0)
#drvEtherIP_default_rate(5)

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
#dbpf $(P)RUN_STOP.HIGH 0.11
#dbpf $(P)RUN_STOP 1
epicsThreadSleep(0.2)
asynSetTraceMask("myIP",-1,1)
asynSetTraceMask("myReg",-1,1)

