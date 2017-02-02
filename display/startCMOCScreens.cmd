#! /bin/bash

edm -eolc -x -m "P=PRC" -eolc cmocRegisters.edl &
edm -eolc -x -m "P=PRC" -eolc cmocWaveforms.edl &