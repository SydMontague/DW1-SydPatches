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

.org 0x800f8324
  jal VS__isAlreadySelected
.org 0x800f837c
  jal VS__isAlreadySelected
.org 0x800f83e0
  jal VS__isAlreadySelected
.org 0x800f8444
  jal VS__isAlreadySelected

.org 0x800f834c
  jal VS__handleDigimonSelected
.org 0x800f83b0
  jal VS__handleDigimonSelected
.org 0x800f8414
  jal VS__handleDigimonSelected
.org 0x800f8478
  jal VS__handleDigimonSelected
  
.org 0x800f864c
  jal VS__createPressStartToBeginBox

.org 0x800f86ac
  jal VS__removePressStartToBeginBox
.org 0x800f8738
  jal VS__removePressStartToBeginBox

.org 0x80111b9c
  jal VS__initialize
  
.close
