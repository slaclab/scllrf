#! /bin/bash

# Setup edm environment
#source /reg/g/pcds/setup/epicsenv-3.14.12.sh

edm -eolc -x -m "P=PRC1:,R=ASYN_IP,PORT=prcIP" asynRecord.edl &
edm -eolc -x -m "P=PRC1:,R=ASYN_REG,PORT=prcReg" asynRecord.edl &

