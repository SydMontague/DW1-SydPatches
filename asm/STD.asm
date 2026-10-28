.open "work/DIGIMON/STD_REL.BIN",0x80052ae0
.psx

; Shared EFE particle source is emitted in this owning overlay.
.org 0x8006d168
.area 0x8006d1a0-.
  .notice "STD reclaimed EFE particles space left: " + (0x8006d1a0-.) + " bytes"
  .fill 0x8006d1a0-.
.endarea

; Shared EFE particle source is emitted in this owning overlay.
.org 0x8006dff8
.area 0x8006e558-.
  .importobj "compiled/STDEFEParticles.lib"
  .notice "STD reclaimed EFE particles space left: " + (0x8006e558-.) + " bytes"
  .fill 0x8006e558-.
.endarea

; Shared EFE commands replace these complete vanilla functions.
.org 0x80072d00
.area 0x80072d6c-.
  .notice "STD EFE mask space left: " + (0x80072d6c-.) + " bytes"
  .fill 0x80072d6c-.
.endarea

.org 0x80072f2c
.area 0x80072f78-.
  .notice "STD EFE vector space left: " + (0x80072f78-.) + " bytes"
  .fill 0x80072f78-.
.endarea

; Shared EFE particle source is emitted in this owning overlay.
.org 0x800750e4
.area 0x8007522c-.
  .notice "STD reclaimed EFE particles space left: " + (0x8007522c-.) + " bytes"
  .fill 0x8007522c-.
.endarea

; Shared operand and control semantics are emitted in this owning overlay.
.org 0x800760ec
.area 0x80076634-.
  .importobj "compiled/STDEFEAddressOperand.lib"
  .importobj "compiled/STDEFEDispatch.lib"
  .importobj "compiled/STDEFEIndexedOperand.lib"
  .importobj "compiled/STDEFEControl.lib"
  .notice "STD EFE operands and control space left: " + (0x80076634-.) + " bytes"
  .fill 0x80076634-.
.endarea

.org 0x80076700
.area 0x800769ec-.
  .importobj "compiled/STDEFEOperands.lib"
  .notice "STD EFE basic operands space left: " + (0x800769ec-.) + " bytes"
  .fill 0x800769ec-.
.endarea

.org 0x80076c58
.area 0x80076d28-.
  .notice "STD EFE comparison space left: " + (0x80076d28-.) + " bytes"
  .fill 0x80076d28-.
.endarea

.org 0x80077148
.area 0x800772c4-.
  .importobj "compiled/STDEFERun.lib"
  .notice "STD EFERun space left: " + (0x800772c4-.) + " bytes"
  .fill 0x800772c4-.
.endarea

; STD_dispatchEFEOpcode
.org 0x8006dde4
  jal STD_dispatchEFEOpcode

; STD_initializeEFESubOpcodeTable
.org 0x8006e820
  jal STD_initializeEFESubOpcodeTable

; STD_returnFromEFESubroutine
.org 0x80075fd4
  jal STD_returnFromEFESubroutine
.org 0x80076048
  jal STD_returnFromEFESubroutine

; STD_runEFEScript
.org 0x8006d2b0
  jal STD_runEFEScript
.org 0x8006d498
  jal STD_runEFEScript
.org 0x8006d7cc
  j STD_runEFEScript

; STD_addParticleEmitter
.org 0x8007ab9c
  .word STD_addParticleEmitter

; STD_initializeParticleEmitters
.org 0x8006e7b8
  jal STD_initializeParticleEmitters

; STD_renderParticleEmitters
.org 0x8006d4d0
  jal STD_renderParticleEmitters

; STD_tickParticleEmitters
.org 0x8006d33c
  jal STD_tickParticleEmitters

.close
