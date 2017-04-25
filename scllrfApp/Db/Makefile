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
DB += cmocRegisters.db
DB += cmocWaveforms.db
DB += PRCextra.db
DB += RESextra.db

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
DB_INSTALLS += scllrfCommon.template
DB_INSTALLS += scllrfINTRegisters.template
DB_INSTALLS += scllrfPRCRegisters.template
DB_INSTALLS += scllrfPRCRegisterAsync.db
DB_INSTALLS += scllrfRESRegisters.template
DB_INSTALLS += $(ASYN)/db/asynRecord.db
DB_INSTALLS += ZZZextra.template
DB_INSTALLS += STMPextra.template
DB_INSTALLS += ARCTextra.template
DB_INSTALLS += CPLTextra.template
DB_INSTALLS += HELLextra.template
#DB_INSTALLS += lbnl_test_jsonRegisters.db

#----------------------------------------------------
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_TEMPLATE = <templatename>

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE
