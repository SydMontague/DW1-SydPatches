.open "work/DIGIMON/VS_REL.BIN",0x80052ae0
.psx

; Shared EFE commands replace these complete vanilla functions.
.org 0x80068dd0
.area 0x80068e3c-.
  .notice "VS EFE mask space left: " + (0x80068e3c-.) + " bytes"
  .fill 0x80068e3c-.
.endarea

.org 0x80068ffc
.area 0x80069048-.
  .notice "VS EFE vector space left: " + (0x80069048-.) + " bytes"
  .fill 0x80069048-.
.endarea

; Shared operand semantics are emitted in this owning overlay.
.org 0x8006c1bc
.area 0x8006c23c-.
  .importobj "compiled/VSEFEAddressOperand.lib"
  .notice "VS EFE address operand space left: " + (0x8006c23c-.) + " bytes"
  .fill 0x8006c23c-.
.endarea

.org 0x8006c360
.area 0x8006c4cc-.
  .importobj "compiled/VSEFEIndexedOperand.lib"
  .notice "VS EFE indexed operand space left: " + (0x8006c4cc-.) + " bytes"
  .fill 0x8006c4cc-.
.endarea

.org 0x8006c7d0
.area 0x8006cabc-.
  .importobj "compiled/VSEFEOperands.lib"
  .notice "VS EFE basic operands space left: " + (0x8006cabc-.) + " bytes"
  .fill 0x8006cabc-.
.endarea

.org 0x8006cd28
.area 0x8006cdf8-.
  .notice "VS EFE comparison space left: " + (0x8006cdf8-.) + " bytes"
  .fill 0x8006cdf8-.
.endarea

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

.org 0x80111b9c
  jal VS__initialize
  
.close
