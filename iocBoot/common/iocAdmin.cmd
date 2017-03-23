# ============================================================================
# Load iocAdmin databases to support IOC Health and monitoring
# ============================================================================
# The MACRO IOCNAME should be defined via the IOCs top level, "iocStartup.cmd"
#  found in $IOC/<iocName>/<viocName>/iocStartup.cmd
# The name must according the SLAC ICD PV naming convention.

# Set IOC_PV as a prefix for autosave and iocAdmin PVs
epicsEnvSet("IOC_PV", "SIOC:$(LOCA):$(TYPE)$(N)")
dbLoadRecords("db/iocAdminSoft.db","IOC=${IOC_PV}")
dbLoadRecords("db/iocAdminScanMon.db","IOC=${IOC_PV}")

# The following database is a result of a python parser
# which looks at RELEASE_SITE and RELEASE to discover
# versions of software your IOC is referencing
# The python parser is part of iocAdmin
dbLoadRecords("db/iocRelease.db","IOC=${IOC_PV}")

