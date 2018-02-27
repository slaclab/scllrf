## =========================================================================
## Start autosave routines to save our data
## =========================================================================
# optional, needed if the IOC takes a very long time to boot.
epicsThreadSleep( 1.0)

# The following command makes the autosave request files 'info_settings.req',
# and 'info_positions.req', from information (info nodes) contained in all of
# the EPICS databases that have been loaded into this IOC.

cd("/data/${IOC}/autosave-req")
makeAutosaveFiles()
cd("..")
create_monitor_set("info_settings.req",60,"")
create_monitor_set("info_positions.req",10,"")

# ===========================================================================

cd ${START_UP}