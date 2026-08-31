
.open "work/DIGIMON/EVL_REL.BIN",0x80060000
.psx

.org 0x8006310c
  jal downloadCLUT1
  
.org 0x8006032c
  jal fadeoutCLUT1
.org 0x80060f74
  jal fadeoutCLUT1

.org 0x80063118
  jal downloadCLUT2

.org 0x80060340
  jal fadeoutCLUT2
.org 0x80060f88
  jal fadeoutCLUT2

.close

.open "work/DIGIMON/MURD_REL.BIN",0x8007C000
.psx

.org 0x8007c134
  jal downloadCLUT1

.org 0x8007c148
  jal fadeoutCLUT1

.close

.open "work/DIGIMON/DOOA_REL.BIN",0x80080000
.psx

.org 0x80080484
  jal downloadCLUT1

.org 0x80080498
  jal fadeoutCLUT1
.org 0x80082640
  jal fadeoutCLUT1
.org 0x800826d4
  jal fadeoutCLUT1

.close