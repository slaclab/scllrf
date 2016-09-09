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

# Use the following for RTEMS Only IOCs
#PROD_IOC_RTEMS = scllrf

# scllrf.dbd will be created and installed
DBD += scllrf.dbd

# scllrf.dbd will be made up from these files:
scllrf_DBD += base.dbd
scllrf_DBD += devIocStats.dbd
scllrf_DBD += autosaveSupport.dbd
scllrf_DBD += caPutLog.dbd
scllrf_DBD += $(CEXP_DBDS)

# ================================================================================
# The system command will be mainly used by linux hosted IOCs
# Very useful for changing kernel thread priorities for kernel drivers
# ================================================================================
scllrf_DBD += system.dbd

scllrf_DBD += drvAsynIPPort.dbd
scllrf_DBD += asynRecord.dbd
scllrf_DBD += asyn.dbd
scllrf_DBD += scllrfAsynPortDriver.dbd

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
# scllrf_SRCS += file1.c
 scllrf_SRCS += scllrfAsynPortDriver.cpp
 #scllrf_SRCS += scllrfCMOC.cpp
 #USR_CXXFLAGS += -std=c++0x

# =====================================================
# Link in the code from other libraries
# =====================================================
# Needed to link in iocAdmin support
scllrf_LIBS += devIocStats

# Needed to link in autosave support
scllrf_LIBS += autosave

# Needed to link caPutLog support
scllrf_LIBS += caPutLog

# Needed to link cexpsh() support
scllrf_LIBS += $(CEXP_LIBS)

scllrf_LIBS += asyn

# =====================================================
# END EVR libraries
# =====================================================

# NOTE: To build SNL programs, SNCSEQ must be defined
# in the <top>/configure/RELEASE file

#ifneq ($(SNCSEQ),)
#    # This builds sncExample as a component of scllrf
#    scllrf_SNCFLAGS += +r
#    scllrf_DBD += sncExample.dbd
#    scllrf_SRCS += sncExample.stt
#    scllrf_LIBS += seq pv
#
#    # The following builds sncProgram as a standalone application
#  ifneq ($(T_A),linuxRT-x86)
#    PROD_HOST += sncProgram
#    sncProgram_SNCFLAGS += +m
#    sncProgram_SRCS += sncProgram.st
#    sncProgram_LIBS += seq pv
#    sncProgram_LIBS += $(EPICS_BASE_HOST_LIBS)
#  endif
#endif

scllrf_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

