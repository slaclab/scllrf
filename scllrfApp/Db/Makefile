TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#----------------------------------------------------
#  Optimization of db files using dbst (DEFAULT: NO)
#DB_OPT = YES

#----------------------------------------------------
# Create and install (or just install)
# databases, templates, substitutions like this
DB += PRCextra.db
DB += GunExtra.db
DB += BMB7monitor.db
DB += cryo-plc.db

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

# Install Database to support reading the status of the autosave package:
ifneq ($(AUTOSAVE),)
DB_INSTALLS += $(AUTOSAVE)/db/save_restoreStatus.db
endif

# =======================================================================
# Install to deployment dir (eg. PROD)
DB_INSTALLS += $(TOP)/scllrfApp/Db/scllrfCommon.template
DB_INSTALLS += $(TOP)/scllrfApp/Db/scllrfINTRegisters.template
DB_INSTALLS += $(TOP)/scllrfApp/Db/scllrfPRCRegisters.template
DB_INSTALLS += $(TOP)/scllrfApp/Db/scllrfRESRegisters.template
DB_INSTALLS += $(TOP)/scllrfApp/Db/GunRegisters.template
DB_INSTALLS += $(TOP)/scllrfApp/Db/RESextra.db
DB_INSTALLS += $(ASYN)/db/asynRecord.db
DB_INSTALLS += $(TOP)/scllrfApp/Db/ZZZextra.template
DB_INSTALLS += $(TOP)/scllrfApp/Db/STMPextra.template
DB_INSTALLS += $(TOP)/scllrfApp/Db/ARCTextra.template
DB_INSTALLS += $(TOP)/scllrfApp/Db/CPLTextra.template
DB_INSTALLS += $(TOP)/scllrfApp/Db/HELLextra.template
DB_INSTALLS += $(TOP)/scllrfApp/Db/DEGKextra.template
DB_INSTALLS += $(TOP)/scllrfApp/Db/STEPextra.template

#----------------------------------------------------
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_TEMPLATE = <templatename>

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE
