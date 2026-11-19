.open "work/DIGIMON/VS_REL.BIN",0x80052ae0
.psx

; Shared EFE source is emitted only in this owning overlay.
.org 0x80063238
.area 0x800635c4-.
  .importobj "compiled/VSEFEFrame.lib"
  .notice "VS EFE tick/render space left: " + (0x800635c4-.) + " bytes"
  .fill 0x800635c4-.
.endarea

; Keep the vanilla sound clear function before the replacement stop operation.
.org 0x80063600
.area 0x80063664-.
  .importobj "compiled/VSEFESound.lib"
  .notice "VS EFE stop sounds space left: " + (0x80063664-.) + " bytes"
  .fill 0x80063664-.
.endarea

.org 0x80063664
.area 0x80063fb8-.
  .importobj "compiled/VSEFELifecycle.lib"
  .notice "VS EFE lifecycle space left: " + (0x80063fb8-.) + " bytes"
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
  .notice "VS EFE render space left: " + (0x80064950-.) + " bytes"
  .fill 0x80064950-.
.endarea

.org 0x800649cc
.area 0x80064a54-.
  .importobj "compiled/VSEFESlotScript.lib"
  .notice "VS EFE slot script space left: " + (0x80064a54-.) + " bytes"
  .fill 0x80064a54-.
.endarea

.org 0x800673c8
.area 0x80067500-.
  .notice "VS retired EFE scalar vector space left: " + (0x80067500-.) + " bytes"
  .fill 0x80067500-.
.endarea

.org 0x80068d64
.area 0x80069048-.
  .importobj "compiled/VSEFEVectorArithmetic.lib"
  .notice "VS EFE vector arithmetic space left: " + (0x80069048-.) + " bytes"
  .fill 0x80069048-.
.endarea

.org 0x8006b1b4
.area 0x8006b2fc-.
  .notice "VS EFE slot release space left: " + (0x8006b2fc-.) + " bytes"
  .fill 0x8006b2fc-.
.endarea

.org 0x8006c1bc
.area 0x8006cd28-.
  .importobj "compiled/VSEFEAddressOperand.lib"
  .importobj "compiled/VSEFEDispatch.lib"
  .importobj "compiled/VSEFEIndexedOperand.lib"
  .importobj "compiled/VSEFEControl.lib"
  .importobj "compiled/VSEFEOperands.lib"
  .importobj "compiled/VSEFEArithmetic.lib"
  .notice "VS EFE operands, control and arithmetic space left: " + (0x8006cd28-.) + " bytes"
  .fill 0x8006cd28-.
.endarea

.org 0x8006cd28
.area 0x8006cdf8-.
  .importobj "compiled/VSEFEInitialize.lib"
  .notice "VS EFE initialize space left: " + (0x8006cdf8-.) + " bytes"
  .fill 0x8006cdf8-.
.endarea

.org 0x8006d218
.area 0x8006d394-.
  .importobj "compiled/VSEFERun.lib"
  .notice "VS EFERun space left: " + (0x8006d394-.) + " bytes"
  .fill 0x8006d394-.
.endarea

.org 0x80070e94
.area 0x80070f34-.
  .notice "VS retired EFE operator table space left: " + (0x80070f34-.) + " bytes"
  .fill 0x80070f34-.
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

; VS_applyEFEVariableOperator
.org 0x80070e5c
  .word VS_applyEFEVariableOperator

; EFE vector arithmetic subopcodes
.org 0x80070cd0
  .word VS_addVectors
.org 0x80070cd8
  .word VS_subtractVectors
.org 0x80070ce0
  .word VS_multiplyVectors
.org 0x80070ce8
  .word VS_divideVectors
.org 0x80070cf8
  .word VS_shiftVectorsRight
.org 0x80070d80
  .word VS_multiplyVectorByScalar
.org 0x80070d88
  .word VS_divideVectorByScalar
.org 0x80070d90
  .word VS_maskVectorByScalar

; VS_loadNextEFEFile
.org 0x800649ac
  jal VS_loadNextEFEFile

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
