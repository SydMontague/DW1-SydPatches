.open "work/DIGIMON/STD_REL.BIN",0x80052ae0
.psx

; Shared EFE source is emitted only in this owning overlay.
.org 0x8006d168
.area 0x8006dee8-.
  .importobj "compiled/STDEFELifecycle.lib"
  .importobj "compiled/STDEFESound.lib"
  .notice "STD EFE main space left: " + (0x8006dee8-.) + " bytes"
  .fill 0x8006dee8-.
.endarea

.org 0x8006dff8
.area 0x8006e558-.
  .importobj "compiled/STDEFEParticles.lib"
  .notice "STD reclaimed EFE particles space left: " + (0x8006e558-.) + " bytes"
  .fill 0x8006e558-.
.endarea

.org 0x8006e744
.area 0x8006e880-.
  .importobj "compiled/STDEFERender.lib"
  .notice "STD EFE render space left: " + (0x8006e880-.) + " bytes"
  .fill 0x8006e880-.
.endarea

.org 0x8006e8fc
.area 0x8006e984-.
  .notice "STD retired EFE slot space left: " + (0x8006e984-.) + " bytes"
  .fill 0x8006e984-.
.endarea

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

.org 0x800744cc
.area 0x80074590-.
  .importobj "compiled/STDEFEInitialize.lib"
  .notice "STD EFE initialize space left: " + (0x80074590-.) + " bytes"
  .fill 0x80074590-.
.endarea

.org 0x800750e4
.area 0x8007522c-.
  .importobj "compiled/STDEFESlots.lib"
  .notice "STD EFE slot release space left: " + (0x8007522c-.) + " bytes"
  .fill 0x8007522c-.
.endarea

.org 0x80075d8c
.area 0x80075dd8-.
  .notice "STD EFE wait space left: " + (0x80075dd8-.) + " bytes"
  .fill 0x80075dd8-.
.endarea

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
  .importobj "compiled/STDEFESlotScript.lib"
  .notice "STD EFE slot script space left: " + (0x80076d28-.) + " bytes"
  .fill 0x80076d28-.
.endarea

.org 0x80077148
.area 0x800772c4-.
  .importobj "compiled/STDEFERun.lib"
  .notice "STD EFERun space left: " + (0x800772c4-.) + " bytes"
  .fill 0x800772c4-.
.endarea

; STD_addParticleEmitter
.org 0x8007ab9c
  .word STD_addParticleEmitter

; STD_initializeEFEEngine
.org 0x8005f074
  jal STD_initializeEFEEngine

; STD_initializeSubEffectInstructions
.org 0x8007ab2c
  .word STD_initializeSubEffectInstructions

; STD_loadNextEFEFile
.org 0x8006e8dc
  jal STD_loadNextEFEFile

; STD_playEFESound
.org 0x8007abfc
  .word STD_playEFESound

; STD_removeEFEEngine
.org 0x80060a84
  jal STD_removeEFEEngine

; STD_returnFromEFESubroutine
.org 0x80075fd4
  jal STD_returnFromEFESubroutine
.org 0x80076048
  jal STD_returnFromEFESubroutine

; STD_runEFESlotScript
.org 0x8006e9e4
  jal STD_runEFESlotScript

; STD_unloadAllEFESlots
.org 0x80060a7c
  jal STD_unloadAllEFESlots

.close
