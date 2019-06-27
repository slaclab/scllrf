TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#----------------------------------------------------
#  Optimization of db files using dbst (DEFAULT: NO)
#DB_OPT = YES

DB_INSTALLS += $(STD)/stdApp/Db/pid_control.db

#----------------------------------------------------
# Create and install (or just install)
# databases, templates, substitutions like this
DB += GUNBLlrfDev.db
DB += BMB7monitor.db
DB += BuncherLlrfDev.db
DB += GunStatHistory.db
DB += BuncherStatHistory.db

# To generate a script that renames buncher channels to gun channels, uncomment this.
#DB += BuncherWFtoGunWF.db

# Install Common databases needed by All IOCs
# ========================================================
# Install databases from iocAdmin package
# These databases support IOC Health and Monitoring
# ========================================================
ifneq ($(IOCADMIN),)
# HardIOC using RTEMS
#DB_INSTALLS += $(IOCADMIN)/db/iocAdminRTEMS.db
# SoftIOC
DB_INSTALLS += $(IOCADMIN)/db/iocAdminSoft.db

DB_INSTALLS += $(IOCADMIN)/db/iocAdminScanMon.db
DB_INSTALLS += $(IOCADMIN)/db/iocScanMonSum.db
DB_INSTALLS += $(IOCADMIN)/db/iocScanMon.db
IOCRELEASE_DB += iocRelease.db
endif
ifneq ($(SNCSEQ),)
DB_INSTALLS += $(SNCSEQ)/db/devSeqCar.db
endif

# Install Database to support reading the status of the autosave package:
ifneq ($(AUTOSAVE),)
DB_INSTALLS += $(AUTOSAVE)/db/save_restoreStatus.db
endif

# =======================================================================
# Install to deployment dir (eg. PROD)
DB_INSTALLS += $(TOP)/scllrfApp/Db/leepDrv.template
DB_INSTALLS += $(TOP)/scllrfApp/Db/GUNBLlrfRegisters.template
DB_INSTALLS += $(ASYN)/db/asynRecord.db

#----------------------------------------------------
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_TEMPLATE = <templatename>

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE
