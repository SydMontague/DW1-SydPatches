.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x800a568c
.area 0x800a6230-.
  .importobj "compiled/Partner.lib"

  .notice "Empty space left: " + (0x800a6230-.) + " bytes"
  .fill 0x800a6230-.
.endarea


.org 0x800dd99c
  jal tickConditionBoundaries
.org 0x800abc28
  jal tickConditionBoundaries
.org 0x800a8fc8
  jal tickConditionBoundaries

.org 0x800a8fa0
  jal tickSleepMechanics

.org 0x800a8fc0
  jal tickUnhappinessMechanics

.close 

.open "work/DIGIMON/TRN_REL.BIN",0x80088800
.psx

.org 0x8008a6e0
  jal tickConditionBoundaries

.close


.open "work/DIGIMON/TRN2_REL.BIN",0x80088800
.psx

.org 0x8008be8c
  jal tickConditionBoundaries

.close