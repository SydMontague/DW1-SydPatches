#!/bin/bash

cd "$(dirname "$0")"
mkdir ../compiled/ -p

FLAGS="-O3 -mabi=32 -march=r3000 -r -mel -nostdlib -mno-shared -fno-zero-initialized-in-bss -mno-gpopt -fno-inline -fno-exceptions -mno-check-zero-division -Wno-builtin-declaration-mismatch"

mips-elf-g++ Utils.cpp -o ../compiled/utils.lib $FLAGS
mips-elf-g++ Font.cpp Font5px.cpp -o ../compiled/font.lib $FLAGS
mips-elf-g++ ItemEffects.cpp -o ../compiled/ItemEffects.lib $FLAGS
mips-elf-g++ ItemFunctions.cpp -o ../compiled/ItemFunctions.lib $FLAGS
mips-elf-g++ CustomUI.cpp -o ../compiled/CustomUI.lib $FLAGS
mips-elf-g++ CustomInit.cpp -o ../compiled/CustomInit.lib $FLAGS

cd -