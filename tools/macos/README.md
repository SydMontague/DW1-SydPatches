# Build on macOS

These tools run natively on macOS and compile the existing PlayStation game patches to MIPS R3000 code. The output is a PS1 BIN/CUE image to use with hardware or an emulator.

Install Xcode Command Line Tools (`xcode-select --install`) and Homebrew, then run from the repository:

```sh
brew install cmake gmp mpfr libmpc texinfo
bash tools/macos/setup.sh
bash extract.sh '/path/to/Digimon World (USA).bin'
bash rebuild.sh
```

The first setup builds GCC 14.2.0 with the same coprocessor-register patch and target configuration as the devcontainer, GNU binutils 2.43, armips, and mkpsxiso/dumpsxiso. Downloads are pinned in `setup.sh`. Allow several minutes and several GB for the compiler build. `JOBS=4 bash tools/macos/setup.sh` limits build concurrency. The default cache is ignored `.macos-toolchain/`; dependencies remain managed by Homebrew.

The build creates `DigimonWorldPatched.bin` and `DigimonWorldPatched.cue` in the repository root. Keep both files together. Extraction preserves the source image and refuses to overwrite an existing `extract/`; rebuilding replaces generated `work/`, `compiled/`, and the patched image. Scripts stop on the first failed build step.

To share installed tools between checkouts:

```sh
bash tools/macos/setup.sh /path/to/tool-cache
export DW1_TOOLCHAIN_PREFIX=/path/to/tool-cache/prefix
bash rebuild.sh
```

The tool prefix must be absolute. You can also override `MIPS_CXX`, `ARMIPS`, `MKPSXISO`, and `DUMPSXISO` individually. Linux continues to use the bundled disc tools and devcontainer compiler by default.

Validated host and build results are recorded by the consuming project. Successful image generation is a build check; gameplay and hardware compatibility need separate testing.
