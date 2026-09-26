#!/bin/bash
# Native host tools; target code remains MIPS R3000. No retail input is needed.
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
if [ "$(uname -s)" != Darwin ]; then
    echo 'This setup script is for macOS; use the devcontainer on Linux.' >&2
    exit 1
fi
for tool in brew cmake curl git make patch shasum; do
    command -v "$tool" >/dev/null || { echo "Missing $tool; see tools/macos/README.md" >&2; exit 1; }
done
BREW_PREFIX="$(brew --prefix)"
for dep in gmp mpfr libmpc texinfo; do
    [ -d "$BREW_PREFIX/opt/$dep" ] || { echo "Run: brew install cmake gmp mpfr libmpc texinfo" >&2; exit 1; }
done
CACHE="${1:-$ROOT/.macos-toolchain}"
mkdir -p "$CACHE"
CACHE="$(cd "$CACHE" && pwd)"
PREFIX="$CACHE/prefix"
JOBS="${JOBS:-8}"
mkdir -p "$CACHE/src" "$CACHE/build/binutils" "$CACHE/build/gcc" "$PREFIX/bin"
export PATH="$PREFIX/bin:$BREW_PREFIX/opt/texinfo/bin:$PATH"

archive() {
    local name="$1" url="$2" checksum="$3"
    if [ ! -f "$CACHE/src/$name.tar.xz" ]; then
        curl -fL --retry 3 "$url" -o "$CACHE/src/$name.tar.xz.part"
        mv "$CACHE/src/$name.tar.xz.part" "$CACHE/src/$name.tar.xz"
    fi
    printf '%s  %s\n' "$checksum" "$CACHE/src/$name.tar.xz" | shasum -a 256 -c -
    if [ ! -d "$CACHE/src/$name" ]; then
        tar -xf "$CACHE/src/$name.tar.xz" -C "$CACHE/src"
    fi
}
archive gcc-14.2.0 https://ftp.gnu.org/gnu/gcc/gcc-14.2.0/gcc-14.2.0.tar.xz a7b39bc69cbf9e25826c5a60ab26477001f7c08d85cec04bc0e29cabed6f3cc9
archive binutils-2.43 https://ftp.gnu.org/gnu/binutils/binutils-2.43.tar.xz b53606f443ac8f01d1d5fc9c39497f2af322d99e14cea5c0b4b124d630379365
if ! grep -q 'COP0_REG_P (regno) || COP2_REG_P (regno) || COP3_REG_P (regno)' "$CACHE/src/gcc-14.2.0/gcc/config/mips/mips.cc"; then
    patch "$CACHE/src/gcc-14.2.0/gcc/config/mips/mips.cc" "$ROOT/.devcontainer/mips.cc.patch"
fi
(
    cd "$CACHE/build/binutils"
    if [ ! -f Makefile ]; then
        "$CACHE/src/binutils-2.43/configure" --prefix="$PREFIX" --target=mips --program-prefix=mips- \
            --disable-nls --with-gnu-as --with-gnu-ld --disable-werror
    fi
    make -j"$JOBS"
    make install
)
(
    cd "$CACHE/build/gcc"
    if [ ! -f Makefile ]; then
        "$CACHE/src/gcc-14.2.0/configure" --prefix="$PREFIX" --target=mips --program-prefix=mips- \
            --disable-nls --disable-shared --disable-multilib --disable-threads --without-headers \
            --enable-languages=c,c++ --with-gnu-as --with-gnu-ld \
            --with-gmp="$BREW_PREFIX" --with-mpfr="$BREW_PREFIX" --with-mpc="$BREW_PREFIX"
    fi
    make all-gcc -j"$JOBS"
    make install-gcc
)

checkout() {
    local name="$1" url="$2" revision="$3"
    if [ ! -d "$CACHE/src/$name" ]; then
        git clone "$url" "$CACHE/src/$name"
        git -C "$CACHE/src/$name" checkout --detach "$revision"
    fi
    if [ "$(git -C "$CACHE/src/$name" rev-parse HEAD)" != "$revision" ]; then
        echo "Unexpected $name revision in $CACHE/src/$name; use a fresh cache directory." >&2
        exit 1
    fi
    git -C "$CACHE/src/$name" submodule update --init --recursive
}
checkout armips https://github.com/Kingcom/armips.git 62adab4ef30da765f5cf22a451eb08a59c54dc8b
checkout mkpsxiso https://github.com/Lameguy64/mkpsxiso.git a6b11ea86e67c189137ac50a4066044b3fdb0525
for name in armips mkpsxiso; do
    cmake -S "$CACHE/src/$name" -B "$CACHE/build/$name" -DCMAKE_BUILD_TYPE=Release -DCMAKE_POLICY_VERSION_MINIMUM=3.5
    cmake --build "$CACHE/build/$name" -j"$JOBS"
done
cp "$CACHE/build/armips/armips" "$CACHE/build/mkpsxiso/mkpsxiso" "$CACHE/build/mkpsxiso/dumpsxiso" "$PREFIX/bin/"
printf '\nNative tools ready. For an external cache, run:\nexport DW1_TOOLCHAIN_PREFIX=%q\n' "$PREFIX"
