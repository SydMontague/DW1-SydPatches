.open "work/DIGIMON/VS_REL.BIN",0x80052ae0
.psx

; Shared EFE source is emitted only in this owning overlay.
.org 0x80063238
.area 0x80063fb8-.
  .importobj "compiled/VSEFELifecycle.lib"
  .importobj "compiled/VSEFESound.lib"
  .notice "VS EFE main space left: " + (0x80063fb8-.) + " bytes"
  .fill 0x80063fb8-.
.endarea

.org 0x800640c8
.area 0x80064628-.
  .importobj "compiled/VSEFEParticles.lib"
  .notice "VS reclaimed EFE particles space left: " + (0x80064628-.) + " bytes"
  .fill 0x80064628-.
.endarea

.org 0x80064814
.area 0x80064950-.
  .importobj "compiled/VSEFERender.lib"
  .notice "VS EFE render space left: " + (0x80064950-.) + " bytes"
  .fill 0x80064950-.
.endarea

.org 0x800649cc
.area 0x80064a54-.
  .notice "VS retired EFE slot space left: " + (0x80064a54-.) + " bytes"
  .fill 0x80064a54-.
.endarea

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

.org 0x8006a59c
.area 0x8006a660-.
  .importobj "compiled/VSEFEInitialize.lib"
  .notice "VS EFE initialize space left: " + (0x8006a660-.) + " bytes"
  .fill 0x8006a660-.
.endarea

.org 0x8006b1b4
.area 0x8006b2fc-.
  .importobj "compiled/VSEFESlots.lib"
  .notice "VS EFE slot release space left: " + (0x8006b2fc-.) + " bytes"
  .fill 0x8006b2fc-.
.endarea

.org 0x8006be5c
.area 0x8006bea8-.
  .notice "VS EFE wait space left: " + (0x8006bea8-.) + " bytes"
  .fill 0x8006bea8-.
.endarea

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
  .importobj "compiled/VSEFESlotScript.lib"
  .notice "VS EFE slot script space left: " + (0x8006cdf8-.) + " bytes"
  .fill 0x8006cdf8-.
.endarea

.org 0x8006d218
.area 0x8006d394-.
  .importobj "compiled/VSEFERun.lib"
  .notice "VS EFERun space left: " + (0x8006d394-.) + " bytes"
  .fill 0x8006d394-.
.endarea

; VS__combatMain
.org 0x80058238
  jal VS__combatMain

; VS__runIntro
.org 0x80058228
  jal VS__runIntro

; VS__tickDigimon
.org 0x80056f94
  li.u a2, VS__tickDigimon
.org 0x80056fb0
  li.l a2, VS__tickDigimon
.org 0x8005700c
  li.u a2, VS__tickDigimon
.org 0x80057028
  li.l a2, VS__tickDigimon

; VS__tickVSInput
.org 0x80056e88
  li.u a2, VS__tickVSInput
.org 0x80056e98
  li.l a2, VS__tickVSInput
.org 0x80056ea4
  li.u a2, VS__tickVSInput
.org 0x80056eb0
  li.l a2, VS__tickVSInput

; VS_addParticleEmitter
.org 0x80070bc8
  .word VS_addParticleEmitter

; VS_initializeSubEffectInstructions
.org 0x80070b58
  .word VS_initializeSubEffectInstructions

; VS_loadNextEFEFile
.org 0x800649ac
  jal VS_loadNextEFEFile

; VS_playEFESound
.org 0x80070c28
  .word VS_playEFESound

; VS_returnFromEFESubroutine
.org 0x8006c0a4
  jal VS_returnFromEFESubroutine
.org 0x8006c118
  jal VS_returnFromEFESubroutine

; VS_runEFESlotScript
.org 0x80064ab4
  jal VS_runEFESlotScript

.close

.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x80111b9c
  jal VS__initialize
  
.close
