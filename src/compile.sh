#!/bin/bash

cd "$(dirname "$0")"
mkdir ../compiled/ -p

FLAGS="-O3 -mabi=32 -march=r3000 -r -mel -nostdlib -mexplicit-relocs -mno-shared -fno-zero-initialized-in-bss -mno-gpopt -fno-inline-functions -fno-inline-small-functions -fno-exceptions -mno-check-zero-division -Wno-builtin-declaration-mismatch"

mips-g++ Utils.cpp UIElements.cpp -o ../compiled/utils.lib $FLAGS
mips-g++ Font.cpp Font5px.cpp Font7px.cpp -o ../compiled/font.lib $FLAGS
mips-g++ ItemEffects.cpp -o ../compiled/ItemEffects.lib $FLAGS
mips-g++ ItemFunctions.cpp -o ../compiled/ItemFunctions.lib $FLAGS
mips-g++ CustomUI.cpp -o ../compiled/CustomUI.lib $FLAGS
mips-g++ CustomInit.cpp -o ../compiled/CustomInit.lib $FLAGS
mips-g++ Evolution.cpp -o ../compiled/Evolution.lib $FLAGS
mips-g++ StatsView.cpp -o ../compiled/StatsView.lib $FLAGS
mips-g++ StatsViewData.cpp -o ../compiled/StatsViewData.lib $FLAGS
mips-g++ Partner.cpp -o ../compiled/Partner.lib $FLAGS

cd -