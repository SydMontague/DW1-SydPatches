.open "work/DIGIMON/VS_REL.BIN",0x80052ae0
.psx

.org 0x80058228
  jal VS__runIntro

.org 0x80058238
  jal VS__combatMain

.close


.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.close
