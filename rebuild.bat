DEL work\
COPY extract\ work\
COPY BUILD.xml work\BUILD.XML
tools\windows\armips.exe patches.asm
tools\windows\mkpsxiso.exe work\BUILD.XML -o -y -q
DEL DigimonWorldPatched.cue
REN mkpsxiso.cue DigimonWorldPatched.cue