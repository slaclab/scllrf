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
PROD_IOC = gunbLlrf
#LIBRARY_IOC= gunbLlrf
# Use the following for RTEMS Only IOCs
#PROD_IOC_RTEMS = gunbLlrf

# gunbLlrf.dbd will be created and installed
DBD += gunbLlrf.dbd

# gunbLlrf.dbd will be made up from these files:
gunbLlrf_DBD += base.dbd

ifneq ($(IOCADMIN),)
gunbLlrf_DBD += devIocStats.dbd
endif
ifneq ($(AUTOSAVE),)
gunbLlrf_DBD += asSupport.dbd
endif
ifneq ($(CAPUTLOG),)
gunbLlrf_DBD += caPutLog.dbd
endif
gunbLlrf_DBD += $(CEXP_DBDS)

# ================================================================================
# The system command will be mainly used by linux hosted IOCs
# Very useful for changing kernel thread priorities for kernel drivers
# ================================================================================
gunbLlrf_DBD += system.dbd

gunbLlrf_DBD += drvAsynIPPort.dbd
gunbLlrf_DBD += asyn.dbd
gunbLlrf_DBD += leepDrv.dbd
gunbLlrf_DBD += calcSupport.dbd
gunbLlrf_DBD += stdSupport.dbd
gunbLlrf_DBD += history.dbd
gunbLlrf_DBD += PVAServerRegister.dbd
gunbLlrf_DBD += qsrv.dbd

# ======================================================================
# <name>_registerRecordDeviceDriver.cpp will be created from <name>.dbd
gunbLlrf_SRCS += gunbLlrf_registerRecordDeviceDriver.cpp

gunbLlrf_SRCS_DEFAULT += gunbLlrfMain.cpp

gunbLlrf_SRCS_vxWorks += -nil-

# ========================================================================
# ========================================================================
# Add locally compiled object code
# For example, subroutines
# ========================================================================
gunbLlrf_SRCS += leepDrv.cpp
gunbLlrf_SRCS += GUNBLlrfDrv.cpp
gunbLlrf_SRCS += GUNBLlrfDev.cpp
gunbLlrf_SRCS += PVramp.stt
gunbLlrf_SRCS += centerCwDetune.stt
gunbLlrf_SRCS += amc78xxmon.stt
gunbLlrf_SRCS += BMB7Sup.c
USR_CXXFLAGS += -std=c++0x
USR_CXXFLAGS += -g -I$(TOP)/newmat10
# =====================================================
# Link in the code from other libraries
# =====================================================
# Needed to link in iocAdmin support
ifneq ($(IOCADMIN),)
gunbLlrf_LIBS += devIocStats
endif

# Needed to link in autosave support
ifneq ($(AUTOSAVE),)
gunbLlrf_LIBS += autosave
endif

# Needed to link caPutLog support
ifneq ($(CAPUTLOG),)
gunbLlrf_LIBS += caPutLog
endif

# Needed to link cexpsh() support
gunbLlrf_LIBS += $(CEXP_LIBS)
gunbLlrf_SYS_LIBS += $(CEXP_SYS_LIBS)

gunbLlrf_LIBS += asyn
gunbLlrf_LIBS += calc
gunbLlrf_LIBS += sscan
gunbLlrf_LIBS += history
gunbLlrf_LIBS += newmat
gunbLlrf_LIBS += qsrv
gunbLlrf_LIBS += std
# EPICS_BASE_PVA_CORE_LIBS comes from pvAccessCPP and is defined at <>/cfg/CONFIG_PVACCESS_MODULE
gunbLlrf_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)
gunbLlrf_LIBS += $(EPICS_BASE_IOC_LIBS)
newmat_DIR = $(TOP)/newmat10/

# =====================================================
# END EVR libraries
# =====================================================

# NOTE: To build SNL programs, SNCSEQ must be defined
# in the <top>/configure/RELEASE file

ifneq ($(SNCSEQ),)
    gunbLlrf_DBD += devSeqCar.dbd
    gunbLlrf_LIBS += seq seqCar pv

    # The following builds sncProgram as a standalone application
#  ifneq ($(T_A),linuxRT-x86)
#    PROD_HOST += sncProgram
#    sncProgram_SNCFLAGS += +m
#    sncProgram_SRCS += sncProgram.st
#    sncProgram_LIBS += seq pv
#    sncProgram_LIBS += $(EPICS_BASE_HOST_LIBS)
#  endif
endif

gunbLlrf_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

#USR_CXXFLAGS += -std=c++11

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE
#USR_CXXFLAGS += -std=c++11
