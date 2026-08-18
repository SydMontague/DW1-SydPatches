.open "work/DIGIMON/VS_REL.BIN",0x80052ae0
.psx

.org 0x80058228
  jal VS__runIntro

.org 0x80058238
  jal VS__combatMain

.org 0x80056e88
li.u a2, VS__tickVSInput
.org 0x80056e98
li.l a2, VS__tickVSInput

.org 0x80056ea4
li.u a2, VS__tickVSInput
.org 0x80056eb0
li.l a2, VS__tickVSInput

.org 0x80056f94
li.u a2, VS__tickDigimon
.org 0x80056fb0
li.l a2, VS__tickDigimon

.org 0x8005700c
li.u a2, VS__tickDigimon
.org 0x80057028
li.l a2, VS__tickDigimon

.close


.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.close
