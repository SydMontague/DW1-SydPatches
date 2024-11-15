COPY ALLTIM.TIM work\DIGIMON\CHDAT\ALLTIM.TIM
COPY BAKE.MMD work\DIGIMON\CHDAT\MMD1\BAKE.MMD
tools\windows\armips.exe patches.asm
tools\windows\mkpsxiso.exe work\BUILD.XML -o DigimonWorldPatched.bin -y -q
DEL DigimonWorldPatched.cue
REN mkpsxiso.cue DigimonWorldPatched.cue