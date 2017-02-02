export PATH=/usr/bin:$PATH

# Uncomment for the hardware that your EPICS IOC will use.
# The hardware connot be shared amongst different IOCs

# set kernel thread priority for the PMC EVR 230
#/usr/bin/chrt -pf 95  `ps  -Leo pid,tid,rtprio,comm | /usr/bin/awk '/mrfevr/{printf $1}'`

# set kernel thread priority for the PCIe EVR 300
#/usr/bin/chrt -pf 95  `ps  -Leo pid,tid,rtprio,comm | /usr/bin/awk '/mrfevr3/{printf $1}'`

# sis8300 Digitizers (mTCA platform)
#/usr/bin/chrt -pf 90  `ps  -Leo pid,tid,rtprio,comm | /usr/bin/awk '/sis8300/{printf $1}'`
