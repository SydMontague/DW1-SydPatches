#!/bin/bash

# Setup work directory
rm -r ./work/
rm -r ./compiled/
cp -r ./extract/ ./work/
cp BUILD.XML ./work/BUILD.XML

# Apply filesystem changes
mv ./work/DIGIMON/ETCDAT/SYSTEM_W.TIM ./work/DIGIMON/STDDAT/SYSTEM_W.TIM

sh ./src/compile.sh

# Apply patches
./tools/linux/armips patches.asm

# create ISO
./tools/linux/mkpsxiso ./work/BUILD.XML -y -q