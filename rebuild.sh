#!/bin/bash
set -euo pipefail
source "$(dirname "$0")/tools/build-env.sh"
cd "$DW1_ROOT"
require_tool "$MIPS_CXX"
require_tool "$ARMIPS"
require_tool "$MKPSXISO"
if [ ! -f ./extract/DIGIMON/SLUS_010.32 ]; then
    echo 'Extract a US Digimon World disc with bash extract.sh <disc.bin> first.' >&2
    exit 1
fi

# Setup work directory
rm -rf ./work/ ./compiled/
cp -r ./extract/ ./work/
cp BUILD.XML ./work/BUILD.XML

# Apply filesystem changes
mv ./work/DIGIMON/ETCDAT/SYSTEM_W.TIM ./work/DIGIMON/STDDAT/SYSTEM_W.TIM

bash ./src/compile.sh

# Apply patches
"$ARMIPS" patches.asm -sym syms.txt

# create ISO
"$MKPSXISO" ./work/BUILD.XML -y -q
