#!/bin/bash

./tools/linux/armips patches.asm
./tools/linux/mkpsxiso work/BUILD.XML -o DigimonWorldPatched.bin -y -q
rm DigimonWorldPatched.cue
mv mkpsxiso.cue DigimonWorldPatched.cue