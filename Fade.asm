.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x800ee894
  jal initializeFadeData

.org 0x801044fc
  jal fadeToWhite

.org 0x80104518
  jal fadeFromWhite

;.org 0x800aa254
;  jal fadeFromBlack
;.org 0x800d8f54
;  jal fadeFromBlack
;.org 0x800ddc84
;  jal fadeFromBlack
.org 0x800eeb88
  jal fadeFromBlack
.org 0x800eed5c
  jal fadeFromBlack
.org 0x800ef044
  jal fadeFromBlack
.org 0x800ef208
  jal fadeFromBlack
.org 0x800f2458
  jal fadeFromBlack
.org 0x800f7d30
  jal fadeFromBlack
.org 0x800f9fd0
  jal fadeFromBlack
.org 0x800fa520
  jal fadeFromBlack

;.org 0x800aa1bc
;  jal fadeToBlack
;.org 0x800d8e98
;  jal fadeToBlack
;.org 0x800ddba4
;  jal fadeToBlack
.org 0x800eeeb8
  jal fadeToBlack
.org 0x800ef18c
  jal fadeToBlack
.org 0x800ef350
  jal fadeToBlack
.org 0x800f0b98
  jal fadeToBlack
.org 0x800f7d7c
  jal fadeToBlack
.org 0x800fa018
  jal fadeToBlack
.org 0x800fa568
  jal fadeToBlack

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