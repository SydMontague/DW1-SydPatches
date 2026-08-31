.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x801044fc
  jal fadeToWhite

.org 0x80104518
  jal fadeFromWhite

.close


.open "work/DIGIMON/STD_REL.BIN",0x80052ae0
.psx

.org 0x80057708
  jal fadeToBlack
.org 0x80057a68
  jal fadeToBlack
.org 0x80057c3c
  jal fadeToBlack

.org 0x80057ad8
  jal fadeFromBlack
.org 0x80057c8c
  jal fadeFromBlack

.close

.open "work/DIGIMON/VS_REL.BIN",0x80052ae0
.psx

.org 0x8005859c
  jal fadeToBlack

.org 0x80058418
  jal fadeFromBlack

.close
