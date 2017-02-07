#==============================================================
#
#  Abs:  Soft IOC pre-startup initialization (Development)
#
#  Name: soft_pre_st.cmd
#
#  Facility: LCLS Controls
#
#  Auth: 05-Jun-2007, Author's Name   (USERNAME)
#  Rev:  dd-mmm-yyyy, Reviewer's Name (USERNAME)
#--------------------------------------------------------------
#  Mod:
#       29-May-2013, K. Luchini       (LUCHINI):
#         remove SLC_PROXI_IP
#         add environment variable ACF_COMMON and ACF_FILE 
#         use macro in ACF_INIT and chg acf command file
#         since init_acf.cmd uses the macro ACF_FILE
#       31-Oct-2011, K. Luchini       (LUCHINI):
#        remove variable IOC_PRIM_MAP used by slc-aware only
#       20-Mar-2008, K. Luchini       (LUCHINI):
#        changed IOC_DATA and IOC_PRIM_MAP location
#       04-Dev-2007, K. Luchini       (LUCHINI):
#        change ip's from production to dev
#
#==============================================================
#
# use the SLAC NTP server
epicsEnvSet("EPICS_TS_NTP_INET",    "134.79.18.40")

# use the MCCDEV CA ports
epicsEnvSet("EPICS_CA_SERVER_PORT",   "5066")
epicsEnvSet("EPICS_CA_REPEATER_PORT", "5067")

# start the iocLogClient so messages are sent to 
# iocLogAndFwdServer on rhel5-32b
epicsEnvSet("EPICS_IOC_LOG_PORT", "7004")
#epicsEnvSet("EPICS_IOC_LOG_INET", "134.79.120.185")

# Send the caPutLog messages are sent to iocLogAndFwdServer
#epicsEnvSet("EPICS_CA_PUT_LOG_ADDR","134.79.120.185:7004")

# Get location of ioc data  on surrey01.
epicsEnvSet("IOC_DATA","/nfs/slac/g/lcls/epics/ioc/data")

# Define common Access Security initialization script
epicsEnvSet("ACF_COMMON","/afs/slac/g/lcls/epics/iocCommon/All/Dev/Acf")
epicsEnvSet("ACF_FILE"  ,"${ACF_COMMON}/lclsSecurity.acf")
epicsEnvSet("ACF_INIT"  ,"${ACF_COMMON}/soft_init_acf.cmd")

# Enable ioc error logging
setIocLogDisable 0

#End of script

