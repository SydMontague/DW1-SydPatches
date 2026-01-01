.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x800f4648
  jal removePauseBox
.org 0x800f7204
  jal removePauseBox

.org 0x800f462c
  jal createPauseBox

.org 0x800efd18
  jal handlePause

.close


.open "work/DIGIMON/BTL_REL.BIN",0x80052ae0
.psx

.org 0x8005b664
  jal handlePause

.org 0x8005b680
  jal removePauseBox

.close


.open "work/DIGIMON/STD_REL.BIN",0x80052ae0
.psx

.org 0x800631c8
  jal handlePause

.org 0x800631d8
  jal removePauseBox

.close