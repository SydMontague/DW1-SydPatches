#!/bin/bash

cd "$(dirname "$0")"
mkdir ../compiled/ -p

FLAGS="-Os -mabi=32 -march=r3000 -r -mel -nostdlib -mno-shared -fno-zero-initialized-in-bss -mno-gpopt -fno-inline -fno-exceptions -Wno-builtin-declaration-mismatch"

mips-elf-g++ Utils.cpp -o ../compiled/utils.lib $FLAGS
mips-elf-g++ Font.cpp Font5px.cpp -o ../compiled/font.lib $FLAGS
mips-elf-g++ setTrainingBoost.cpp -o ../compiled/setTrainingBoost.lib $FLAGS
mips-elf-g++ CustomUI.cpp -o ../compiled/CustomUI.lib $FLAGS

cd -