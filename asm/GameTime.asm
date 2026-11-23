.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

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
