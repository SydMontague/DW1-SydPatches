.open "work/DIGIMON/VS_REL.BIN",0x80052ae0
.psx

.org 0x80058228
  jal VS__runIntro

.close


.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x800f71ac
  jal VS__combatInit

.org 0x800f71b4
  jal VS__combatSetup

; since things are implemented at the moment, we just omit the numbers for now, so that we can skip this useless code
.org 0x800f7478
  nop ; jal VS__renderIntroStatNumber
.org 0x800f74c4
  nop ; jal VS__renderIntroStatNumber
  
.org 0x800f71bc
  jal VS__checkEndCondition

.org 0x800f71d4
  jal VS__tickDigimonAi
.org 0x800f71dc
  jal VS__tickDigimonAi

.org 0x800f71e4
  jal VS__tickBattle

.org 0x800f71f4
  jal VS__handlePause
  
.org 0x800f721c
  jal VS__deinitializeCombat
.org 0x800f723c
  jal VS__deinitializeCombat
.org 0x800f7250
  jal VS__deinitializeCombat

.org 0x800f6ab8
  jal VS__setWalking
.org 0x800f6af0
  jal VS__setWalking

.close
