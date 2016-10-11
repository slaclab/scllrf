#! /bin/bash

edm -eolc -x -m "P=RES" -eolc scllrfRESRegisters.edl &
edm -eolc -x -m "P=RES" -eolc cmocWaveforms.edl &