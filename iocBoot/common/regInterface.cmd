# Do chassis configuration that is needed before PVs connect
#The following line works for RHEL
####XXXX It's a pain to do this every time while testing
system("${GO_PY=} cd ${PY_INIT_DIR}; ${PY_PATH=}python gun.py -a ${FPGA_IP} -b ${BIT_FILE}  -r -c ${CHASSIS_REG}")
cd ${TOP}

# =====================================================================
# Set some facility specific MACROs for database instantiation below
# This is in effort to make IOCs Applications facility agnostic
# Some of the following variables may be defined in
# $IOC/<cpuName>/<epicsIOCName>/iocStartup.cmd
# =====================================================================
# Override the TOP variable set by envPaths:
# This is now past in via $IOC/<cpuName>/<epicsIOCName>/iocStartup.cmd
#epicsEnvSet(TOP,"${IOC_APP}")

# Set up communication with FPGA
drvAsynIPPortConfigure("$(CHASSIS_NAME)IP","$(FPGA_IP):$(PORT) UDP")
dbLoadRecords("db/asynRecord.db","P=$(P),R=ASYN_IP,PORT=$(CHASSIS_NAME)IP,ADDR=0,IMAX=0,OMAX=0")

# ======================================================================
### Asyn Debugging #####################################################
# ======================================================================
## Asyn messages for DIGI_Serial16
#asynSetTraceMask("$(CHASSIS_NAME)IP",-1,ASYN_TRACE_ERROR)
#asynSetTraceMask("$(CHASSIS_NAME)IP",-1,0xB)
#asynSetTraceIOMask("$(CHASSIS_NAME)IP",-1,ASYN_TRACEIO_HEX) ASYN_TRACEIO_HEX = 4
#asynSetTraceIOMask("$(CHASSIS_NAME)IP",-1,4)


#epicsThreadSleep(1.0)
$(SC=scllrf)$(CHASSIS_TYPE)$(EXTRA=extra)Configure( "$(CHASSIS_NAME)Reg","$(CHASSIS_NAME)IP")
dbLoadRecords("db/asynRecord.db","P=$(P),R=ASYN_REG,PORT=$(CHASSIS_NAME)Reg,ADDR=0,IMAX=0,OMAX=0")

# ======================================================================
### Asyn Debugging #####################################################
# ======================================================================
## Asyn messages for DIGI_Serial16
#asynSetTraceMask("$(CHASSIS_NAME)Reg",-1,0xB)
#asynSetTraceIOMask("$(CHASSIS_NAME)Reg",-1,ASYN_TRACEIO_HEX) ASYN_TRACEIO_HEX = 4
#asynSetTraceIOMask("$(CHASSIS_NAME)Reg",-1,4)

epicsThreadSleep(0.2)

# =====================================================================
#Load Additional databases:
# =====================================================================
## Load record instances
dbLoadRecords("db/scllrfCommon.template", "CHASSIS_TYPE=$(CHASSIS_TYPE),P=$(P),PORT=$(CHASSIS_NAME)Reg,SC=$(SC=scllrf)")

bmb7Configure("$(CHASSIS_NAME)BMB7", "$(FPGA_IP)", "0")
dbLoadRecords("db/BMB7monitor.db", "PORT=$(CHASSIS_NAME)BMB7,P=$(DEVICE_TYPE):$(AREA):$(POSITION),R=$(CHASSIS_NAME):")
