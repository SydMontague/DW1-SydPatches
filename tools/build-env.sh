#!/bin/bash
# Shared by the host-side build scripts. Explicit overrides work on every host.
DW1_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
if [ "$(uname -s)" = Darwin ]; then
    DW1_TOOLCHAIN_PREFIX="${DW1_TOOLCHAIN_PREFIX:-$DW1_ROOT/.macos-toolchain/prefix}"
    export PATH="$DW1_TOOLCHAIN_PREFIX/bin:$PATH"
    ARMIPS="${ARMIPS:-armips}"
    MKPSXISO="${MKPSXISO:-mkpsxiso}"
    DUMPSXISO="${DUMPSXISO:-dumpsxiso}"
else
    ARMIPS="${ARMIPS:-$DW1_ROOT/tools/linux/armips}"
    MKPSXISO="${MKPSXISO:-$DW1_ROOT/tools/linux/mkpsxiso}"
    DUMPSXISO="${DUMPSXISO:-$DW1_ROOT/tools/linux/dumpsxiso}"
fi
export MIPS_CXX="${MIPS_CXX:-mips-g++}"

require_tool() {
    if ! command -v "$1" >/dev/null 2>&1; then
        printf 'Required tool not found: %s\nOn macOS, run bash tools/macos/setup.sh first.\n' "$1" >&2
        return 1
    fi
}
