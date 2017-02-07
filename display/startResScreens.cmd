#! /bin/bash
#! /bin/bash

edm -eolc -x -m "P=RES1" -eolc scllrfRESRegisters.edl &
#edm -eolc -x -m "P=RES" -eolc cmocWaveforms.edl &
edm -eolc -x -m "P=RES1" -eolc res4cavity.edl &

