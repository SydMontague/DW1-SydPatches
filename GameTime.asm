.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x800efa00
  jal updateMinuteHand

;.org 0x800a9ec0
;  jal stopGameTime
;.org 0x800aaf28
;  jal stopGameTime
;.org 0x800ab590
;  jal stopGameTime
;.org 0x800ab910
;  jal stopGameTime
;.org 0x800abaa4
;  jal stopGameTime
;.org 0x800acdf0
;  jal stopGameTime
;.org 0x800dda3c
;  jal stopGameTime
;.org 0x800dea90
;  jal stopGameTime
.org 0x800eea24
  jal stopGameTime
.org 0x8010634c
  jal stopGameTime

;.org 0x800aa2dc
;  jal startGameTime
;.org 0x800ab1bc
;  jal startGameTime
;.org 0x800ab1cc
;  jal startGameTime
;.org 0x800b8e38
;  jal startGameTime
;.org 0x800ba2e4
;  jal startGameTime
;.org 0x800ba334
;  jal startGameTime
;.org 0x800ba39c
;  jal startGameTime
;.org 0x800ba458
;  jal startGameTime
;.org 0x800ddcec
;  jal startGameTime
;.org 0x800deb7c
;  jal startGameTime
;.org 0x800e4fe4
;  jal startGameTime
.org 0x801061f8
  jal startGameTime

.close


.open "work/DIGIMON/TRN_REL.BIN",0x80088800
.psx

.org 0x8008a3dc
  jal updateMinuteHand

.close


.open "work/DIGIMON/TRN2_REL.BIN",0x80088800
.psx

.org 0x8008bbd8
  jal updateMinuteHand

.close


.open "work/DIGIMON/FISH_REL.BIN",0x80070000
.psx

.org 0x80079c60
  jal startGameTime

.close