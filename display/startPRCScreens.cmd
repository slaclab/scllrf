#! /bin/bash

edm -eolc -x -m "P=PRC1" -eolc scllrfPRCRegisters.edl &
edm -eolc -x -m "P=PRC1" -eolc PRCIQWaveforms.edl &
edm -eolc -x -m "P=PRC1" -eolc PRCIandQWaveforms.edl &
edm -eolc -x -m "P=PRC1" -eolc PRCIandQ16bitWaveforms.edl &
edm -eolc -x -m "P=PRC1" -eolc PRCIQ16bitWaveforms.edl &
edm -eolc -x -m "P=PRC1" -eolc scllrfPRCBits.edl &

