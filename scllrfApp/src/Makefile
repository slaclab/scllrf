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
PROD_IOC = scllrf
#LIBRARY_IOC= scllrf
# Use the following for RTEMS Only IOCs
#PROD_IOC_RTEMS = scllrf

# scllrf.dbd will be created and installed
DBD += scllrf.dbd

# scllrf.dbd will be made up from these files:
scllrf_DBD += base.dbd

ifneq ($(IOCADMIN),)
scllrf_DBD += devIocStats.dbd
endif
ifneq ($(AUTOSAVE),)
scllrf_DBD += asSupport.dbd
endif
ifneq ($(CAPUTLOG),)
scllrf_DBD += caPutLog.dbd
endif
scllrf_DBD += $(CEXP_DBDS)

# ================================================================================
# The system command will be mainly used by linux hosted IOCs
# Very useful for changing kernel thread priorities for kernel drivers
# ================================================================================
scllrf_DBD += system.dbd

scllrf_DBD += drvAsynIPPort.dbd
scllrf_DBD += asyn.dbd
scllrf_DBD += scllrfAsynPortDriver.dbd
scllrf_DBD += calcSupport.dbd
scllrf_DBD += history.dbd
scllrf_DBD += ether_ip.dbd
scllrf_DBD += PVAServerRegister.dbd
scllrf_DBD += qsrv.dbd

# ======================================================================
# <name>_registerRecordDeviceDriver.cpp will be created from <name>.dbd
scllrf_SRCS += scllrf_registerRecordDeviceDriver.cpp

scllrf_SRCS_DEFAULT += scllrfMain.cpp

scllrf_SRCS_vxWorks += -nil-

# ========================================================================
# ========================================================================
# Add locally compiled object code
# For example, subroutines
# ========================================================================
 scllrf_SRCS += scllrfAsynPortDriver.cpp
# scllrf_SRCS += scllrfINT.cpp
# scllrf_SRCS += scllrfPRC.cpp
# scllrf_SRCS += scllrfPRCextra.cpp
# scllrf_SRCS += scllrfRES.cpp
# scllrf_SRCS += scllrfRESextra.cpp
 scllrf_SRCS += GUNB.cpp
 scllrf_SRCS += GUNBExtra.cpp
 scllrf_SRCS += PVramp.stt
 scllrf_SRCS += centerCwDetune.stt
 scllrf_SRCS += amc78xxmon.stt
# scllrf_SRCS += INTmbboCopy.st
# scllrf_SRCS += INTres2tmp.c
# scllrf_SRCS += INTarcTest.st
# scllrf_SRCS += INTheLevel.st
# scllrf_SRCS += INTssaRfKill.st
# scllrf_SRCS += INTdegkToVolt.st# scllrf_SRCS += RES_HiHwLink.st
 scllrf_SRCS += BMB7Sup.c
USR_CXXFLAGS += -std=c++0x
USR_CXXFLAGS += -g -I$(TOP)/newmat10
# =====================================================
# Link in the code from other libraries
# =====================================================
# Needed to link in iocAdmin support
ifneq ($(IOCADMIN),)
scllrf_LIBS += devIocStats
endif

# Needed to link in autosave support
ifneq ($(AUTOSAVE),)
scllrf_LIBS += autosave
endif

# Needed to link caPutLog support
ifneq ($(CAPUTLOG),)
scllrf_LIBS += caPutLog
endif

# Needed to link cexpsh() support
scllrf_LIBS += $(CEXP_LIBS)

scllrf_LIBS += asyn
scllrf_LIBS += calc
scllrf_LIBS += sscan
scllrf_LIBS += history
scllrf_LIBS += ether_ip
scllrf_LIBS += newmat
scllrf_LIBS += qsrv
# EPICS_BASE_PVA_CORE_LIBS comes from pvAccessCPP and is defined at <>/cfg/CONFIG_PVACCESS_MODULE
scllrf_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)
scllrf_LIBS += $(EPICS_BASE_IOC_LIBS)
newmat_DIR = $(TOP)/newmat10/

# =====================================================
# END EVR libraries
# =====================================================

# NOTE: To build SNL programs, SNCSEQ must be defined
# in the <top>/configure/RELEASE file

ifneq ($(SNCSEQ),)
    scllrf_DBD += devSeqCar.dbd
    scllrf_LIBS += seq seqCar pv

    # The following builds sncProgram as a standalone application
#  ifneq ($(T_A),linuxRT-x86)
#    PROD_HOST += sncProgram
#    sncProgram_SNCFLAGS += +m
#    sncProgram_SRCS += sncProgram.st
#    sncProgram_LIBS += seq pv
#    sncProgram_LIBS += $(EPICS_BASE_HOST_LIBS)
#  endif
endif

scllrf_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

#USR_CXXFLAGS += -std=c++11

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE
#USR_CXXFLAGS += -std=c++11
