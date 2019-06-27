TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#========================================

# ================================================
# Build the IOC application
# ================================================
# This will attempt to build an IOC application
# for every target including the host computer.
PROD_IOC = gunLlrf
#LIBRARY_IOC= gunLlrf
# Use the following for RTEMS Only IOCs
#PROD_IOC_RTEMS = gunLlrf

# gunLlrf.dbd will be created and installed
DBD += gunLlrf.dbd

# gunLlrf.dbd will be made up from these files:
gunLlrf_DBD += base.dbd

ifneq ($(IOCADMIN),)
gunLlrf_DBD += devIocStats.dbd
endif
ifneq ($(AUTOSAVE),)
gunLlrf_DBD += asSupport.dbd
endif
ifneq ($(CAPUTLOG),)
gunLlrf_DBD += caPutLog.dbd
endif
gunLlrf_DBD += $(CEXP_DBDS)

# ================================================================================
# The system command will be mainly used by linux hosted IOCs
# Very useful for changing kernel thread priorities for kernel drivers
# ================================================================================
gunLlrf_DBD += system.dbd

gunLlrf_DBD += drvAsynIPPort.dbd
gunLlrf_DBD += asyn.dbd
gunLlrf_DBD += leepDrv.dbd
gunLlrf_DBD += calcSupport.dbd
gunLlrf_DBD += stdSupport.dbd
gunLlrf_DBD += history.dbd
gunLlrf_DBD += PVAServerRegister.dbd
gunLlrf_DBD += qsrv.dbd

# ======================================================================
# <name>_registerRecordDeviceDriver.cpp will be created from <name>.dbd
gunLlrf_SRCS += gunLlrf_registerRecordDeviceDriver.cpp

gunLlrf_SRCS_DEFAULT += gunLlrfMain.cpp

gunLlrf_SRCS_vxWorks += -nil-

# ========================================================================
# ========================================================================
# Add locally compiled object code
# For example, subroutines
# ========================================================================
gunLlrf_SRCS += leepDrv.cpp
gunLlrf_SRCS += GUNBLlrfDrv.cpp
gunLlrf_SRCS += GUNBLlrfDev.cpp
gunLlrf_SRCS += PVramp.stt
gunLlrf_SRCS += centerCwDetune.stt
gunLlrf_SRCS += amc78xxmon.stt
gunLlrf_SRCS += BMB7Sup.c
USR_CXXFLAGS += -std=c++0x
USR_CXXFLAGS += -g -I$(TOP)/newmat10
# =====================================================
# Link in the code from other libraries
# =====================================================
# Needed to link in iocAdmin support
ifneq ($(IOCADMIN),)
gunLlrf_LIBS += devIocStats
endif

# Needed to link in autosave support
ifneq ($(AUTOSAVE),)
gunLlrf_LIBS += autosave
endif

# Needed to link caPutLog support
ifneq ($(CAPUTLOG),)
gunLlrf_LIBS += caPutLog
endif

# Needed to link cexpsh() support
gunLlrf_LIBS += $(CEXP_LIBS)
gunLlrf_SYS_LIBS += $(CEXP_SYS_LIBS)

gunLlrf_LIBS += asyn
gunLlrf_LIBS += calc
gunLlrf_LIBS += sscan
gunLlrf_LIBS += history
gunLlrf_LIBS += newmat
gunLlrf_LIBS += qsrv
gunLlrf_LIBS += std
# EPICS_BASE_PVA_CORE_LIBS comes from pvAccessCPP and is defined at <>/cfg/CONFIG_PVACCESS_MODULE
gunLlrf_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)
gunLlrf_LIBS += $(EPICS_BASE_IOC_LIBS)
newmat_DIR = $(TOP)/newmat10/

# =====================================================
# END EVR libraries
# =====================================================

# NOTE: To build SNL programs, SNCSEQ must be defined
# in the <top>/configure/RELEASE file

ifneq ($(SNCSEQ),)
    gunLlrf_DBD += devSeqCar.dbd
    gunLlrf_LIBS += seq seqCar pv

    # The following builds sncProgram as a standalone application
#  ifneq ($(T_A),linuxRT-x86)
#    PROD_HOST += sncProgram
#    sncProgram_SNCFLAGS += +m
#    sncProgram_SRCS += sncProgram.st
#    sncProgram_LIBS += seq pv
#    sncProgram_LIBS += $(EPICS_BASE_HOST_LIBS)
#  endif
endif

gunLlrf_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

#USR_CXXFLAGS += -std=c++11

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE
#USR_CXXFLAGS += -std=c++11
