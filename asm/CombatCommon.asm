.open "work/DIGIMON/BTL_REL.BIN",0x80052ae0
.psx

.org 0x80066b5c
  jal doSomethingWithSomePoints

.close

.open "work/DIGIMON/STD_REL.BIN",0x80052ae0
.psx

.org 0x8005bb44
  jal initializeSomeArenaArrays
.org 0x8005b5b4
  jal processSomeArenaArrays
.org 0x8005b654
  jal processSomeArenaArrays
.org 0x8006f154
  jal doSomethingWithSomePoints

.close

.open "work/DIGIMON/VS_REL.BIN",0x80052ae0
.psx

.org 0x8005adc4
  jal initializeSomeArenaArrays
.org 0x8005a830
  jal processSomeArenaArrays
.org 0x8005a8d0
  jal processSomeArenaArrays
.org 0x80065224
  jal doSomethingWithSomePoints

.close