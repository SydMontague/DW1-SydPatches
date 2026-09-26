#!/bin/bash
set -euo pipefail
source "$(dirname "$0")/tools/build-env.sh"
if [ "$#" -ne 1 ] || [ ! -f "$1" ]; then
    echo 'Usage: bash extract.sh <path-to-US-disc.bin>' >&2
    exit 1
fi
require_tool "$DUMPSXISO"
# Resolve the input before changing directories; quoted paths may contain spaces.
DISC="$(cd "$(dirname "$1")" && pwd)/$(basename "$1")"
cd "$DW1_ROOT"
if [ -e ./extract ]; then
    echo 'extract/ already exists; move it aside before extracting again.' >&2
    exit 1
fi
mkdir -p ./extract
"$DUMPSXISO" -x ./extract/DIGIMON -s ./extract/disc.xml "$DISC"
