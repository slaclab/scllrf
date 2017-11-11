#!../../bin/rhel6-x86_64/scllrf
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

## Initialize EtherIP driver and define PLC
drvEtherIP_init()
# drvEtherIP_define_PLC <name>, <ip_addr>, <slot>
drvEtherIP_define_PLC("PLC_B15","192.168.1.21", 0)
EIP_verbosity(0)
#drvEtherIP_default_rate(5)

# =====================================================================
#Load Additional databases:
# =====================================================================
dbLoadRecords("db/cryo-plc.db","CM=01,area=B15,plc=PLC_B15")
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
#dbpf $(P)RUN_STOP.HIGH 0.11
#dbpf $(P)RUN_STOP 1
epicsThreadSleep(0.2)
asynSetTraceMask("myIP",-1,1)
asynSetTraceMask("myReg",-1,1)

