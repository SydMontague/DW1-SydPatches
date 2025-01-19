.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x800a568c
.area 0x800a7d68-.
  .importobj "compiled/Partner.lib"

  .notice "Empty space left: " + (0x800a7d68-.) + " bytes"
  .fill 0x800a7d68-.
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

.org 0x800dde34
  jal handlePraiseScold

.org 0x800ddbd4
  jal handleSleeping

.org 0x800de63c
  jal createPoopPile

.org 0x800ddbcc
  jal sleepRegen

.org 0x800a8fb0
  jal tickTirednessMechanics

.org 0x800de0b8
  jal partnerWillRefuseItem

.org 0x800a8fb8
  jal tickHungerMechanics

.org 0x800dd944
  jal handleConditionBubble
.org 0x800dd9f0
  jal handleConditionBubble

.org 0x800de210
  jal partnerHandleFoodFeed

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