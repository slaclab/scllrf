#! /bin/bash

edm -eolc -x -m "P=PRC" -eolc scllrfPRCRegisters.edl &
edm -eolc -x -m "P=PRC" -eolc cmocWaveforms.edl &