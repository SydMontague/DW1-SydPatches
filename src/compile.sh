#!/bin/bash

cd "$(dirname "$0")"
mkdir ../compiled/ -p

# Flags explanation:
# -O3                               -> highest optimization level
# -T linker.ld                      -> custom linker script, merged .bss into .data section
# -mabi=32                          -> 32 bit ABI
# -march=r3000                      -> target R3000 CPU, used in PSX
# -r                                -> relocateable output/partial linking
# -mel                              -> little endian
# -nostdlib                         -> don't use stdlib, since we don't have one on PSX
# -mexplicit-relocs                 -> use explicit relocations, required by -mno-gpopt
# -mno-shared                       -> don't use position independent code, might be useless?
# -fno-zero-initialized-in-bss      -> prevent the use of GP relative memory and memory sections that are not inserted
# -mno-gpopt                        -> same as above
# -fno-inline-functions             -> disable automatic inlining of functions, to save space and guarantee the symbols
# -fno-inline-small-functions          use 'inline' or 'constexpr' keywords to mark functions for explicit inline
# -msoft-float                      -> we don't have a floating point unit, will cause linking errors when using floats
# -fno-exceptions                   -> we don't have exceptions
# -mno-check-zero-division          -> don't emit trap instructions, saves space
# -Wno-builtin-declaration-mismatch -> don't warn about custom implemented standard functions
FLAGS="-O3 -T linker.ld -mabi=32 -march=r3000 -r -mel -nostdlib -mexplicit-relocs -mno-shared -fno-zero-initialized-in-bss -mno-gpopt -fno-inline-functions -msoft-float -fno-inline-small-functions -fno-exceptions -mno-check-zero-division -Wno-builtin-declaration-mismatch"

mips-g++ Utils.cpp UIElements.cpp -o ../compiled/utils.lib $FLAGS
mips-g++ Font.cpp Font5px.cpp Font7px.cpp -o ../compiled/font.lib $FLAGS
mips-g++ ItemEffects.cpp -o ../compiled/ItemEffects.lib $FLAGS
mips-g++ ItemFunctions.cpp -o ../compiled/ItemFunctions.lib $FLAGS
mips-g++ CustomUI.cpp -o ../compiled/CustomUI.lib $FLAGS
mips-g++ CustomInit.cpp -o ../compiled/CustomInit.lib $FLAGS
mips-g++ Evolution.cpp -o ../compiled/Evolution.lib $FLAGS
mips-g++ StatsView.cpp -o ../compiled/StatsView.lib $FLAGS
mips-g++ StatsViewData.cpp -o ../compiled/StatsViewData.lib $FLAGS

mips-g++ Model.cpp GameObjects.cpp Files.cpp Math.cpp Main.cpp Partner.cpp GUI.cpp -o ../compiled/Model.lib $FLAGS

cd -