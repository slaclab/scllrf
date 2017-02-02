#! /bin/bash

edm -eolc -x -m "P=PRC1" -eolc scllrfPRCRegisters.edl &
edm -eolc -x -m "P=PRC1" -eolc PRCIQWaveforms.edl &