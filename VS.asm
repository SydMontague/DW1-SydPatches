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

.close
