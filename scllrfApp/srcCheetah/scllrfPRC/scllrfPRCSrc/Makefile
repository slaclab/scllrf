TOP=../../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#==================================================
# Build an IOC support library

LIBRARY_IOC += scllrfPRC

DBD += scllrfPRCDriverSupport.dbd

# The following are compiled and added to the Support library
scllrfPRC_SRCS += scllrfPRCDriver.cpp

scllrfPRC_LIBS += DeviceLibrary
scllrfPRC_LIBS += cpswAsynPortDriverSupport
scllrfPRC_LIBS += asyn
scllrfPRC_LIBS += $(EPICS_BASE_IOC_LIBS)
USR_INCLUDES += $(CPSW_FRAMEWORK_INCLUDE)
USR_INCLUDES += $(BOOST_INCLUDE)
include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE


