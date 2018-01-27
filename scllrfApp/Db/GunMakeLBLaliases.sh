#!/bin/bash
# Converting generated register names to LBNL's naming convention, for the Matlab scripts to use
# For all writable records, make an alias in lower case with _W replaced by _ao
sed 's|record(.*"$(P)\(.*\)_W"|alias( "$(P)\1_W", "$(P)\L\1_ao"|' GunRegisters.template | grep alias > GunRegisterAlias.template
# For all writable records, make another alias ending in _bo, because currently the register map doesn't distinguish bits from analogs
sed 's|record(.*"$(P)\(.*\)_W"|alias( "$(P)\1_W", "$(P)\L\1_bo"|' GunRegisters.template | grep alias >> GunRegisterAlias.template
# For all readable records, make an alias in lower case, and drop the _R
sed 's|record(.*"$(P)\(.*\)_R"|alias( "$(P)\1_R", "$(P)\L\1"|' GunRegisters.template | grep alias >> GunRegisterAlias.template
