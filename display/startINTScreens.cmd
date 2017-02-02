#! /bin/bash

edm -eolc -x -m "P=ICC" -eolc scllrfICCRegisters.edl &
edm -eolc -x -m "P=ICC" -eolc cmocWaveforms.edl &