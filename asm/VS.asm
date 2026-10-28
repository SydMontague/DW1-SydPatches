.open "work/DIGIMON/VS_REL.BIN",0x80052ae0
.psx

; Shared EFE particle source is emitted in this owning overlay.
.org 0x80063238
.area 0x80063270-.
  .notice "VS reclaimed EFE particles space left: " + (0x80063270-.) + " bytes"
  .fill 0x80063270-.
.endarea

; Shared EFE particle source is emitted in this owning overlay.
.org 0x800640c8
.area 0x80064628-.
  .importobj "compiled/VSEFEParticles.lib"
  .notice "VS reclaimed EFE particles space left: " + (0x80064628-.) + " bytes"
  .fill 0x80064628-.
.endarea

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

; Shared EFE particle source is emitted in this owning overlay.
.org 0x8006b1b4
.area 0x8006b2fc-.
  .notice "VS reclaimed EFE particles space left: " + (0x8006b2fc-.) + " bytes"
  .fill 0x8006b2fc-.
.endarea

; Shared operand and control semantics are emitted in this owning overlay.
.org 0x8006c1bc
.area 0x8006c704-.
  .importobj "compiled/VSEFEAddressOperand.lib"
  .importobj "compiled/VSEFEDispatch.lib"
  .importobj "compiled/VSEFEIndexedOperand.lib"
  .importobj "compiled/VSEFEControl.lib"
  .notice "VS EFE operands and control space left: " + (0x8006c704-.) + " bytes"
  .fill 0x8006c704-.
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

.org 0x8006d218
.area 0x8006d394-.
  .importobj "compiled/VSEFERun.lib"
  .notice "VS EFERun space left: " + (0x8006d394-.) + " bytes"
  .fill 0x8006d394-.
.endarea

; VS_dispatchEFEOpcode
.org 0x80063eb4
  jal VS_dispatchEFEOpcode

; VS_initializeEFESubOpcodeTable
.org 0x800648f0
  jal VS_initializeEFESubOpcodeTable

; VS_returnFromEFESubroutine
.org 0x8006c0a4
  jal VS_returnFromEFESubroutine
.org 0x8006c118
  jal VS_returnFromEFESubroutine

; VS_runEFEScript
.org 0x80063380
  jal VS_runEFEScript
.org 0x80063568
  jal VS_runEFEScript
.org 0x8006389c
  j VS_runEFEScript

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

; VS_addParticleEmitter
.org 0x80070bc8
  .word VS_addParticleEmitter

; VS_initializeParticleEmitters
.org 0x80064888
  jal VS_initializeParticleEmitters

; VS_renderParticleEmitters
.org 0x800635a0
  jal VS_renderParticleEmitters

; VS_tickParticleEmitters
.org 0x8006340c
  jal VS_tickParticleEmitters

.close


.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x80111b9c
  jal VS__initialize
  
.close
