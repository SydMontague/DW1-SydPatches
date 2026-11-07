.open "work/DIGIMON/BTL_REL.BIN",0x80052ae0
.psx

; Shared EFE source is emitted only in this owning overlay.
.org 0x80064a80
.area 0x80065800-.
  .importobj "compiled/BTLEFELifecycle.lib"
  .importobj "compiled/BTLEFESound.lib"
  .notice "BTL EFE main space left: " + (0x80065800-.) + " bytes"
  .fill 0x80065800-.
.endarea

.org 0x80065910
.area 0x80065e70-.
  .importobj "compiled/BTLEFEParticles.lib"
  .notice "BTL reclaimed EFE particles space left: " + (0x80065e70-.) + " bytes"
  .fill 0x80065e70-.
.endarea

.org 0x8006605c
.area 0x80066198-.
  .importobj "compiled/BTLEFERender.lib"
  .notice "BTL EFE render space left: " + (0x80066198-.) + " bytes"
  .fill 0x80066198-.
.endarea

.org 0x800661d8
.area 0x80066260-.
  .notice "BTL retired EFE slot space left: " + (0x80066260-.) + " bytes"
  .fill 0x80066260-.
.endarea

.org 0x8006a6f4
.area 0x8006a760-.
  .notice "BTL EFE mask space left: " + (0x8006a760-.) + " bytes"
  .fill 0x8006a760-.
.endarea

.org 0x8006a920
.area 0x8006a96c-.
  .notice "BTL EFE vector space left: " + (0x8006a96c-.) + " bytes"
  .fill 0x8006a96c-.
.endarea

.org 0x8006bec0
.area 0x8006bf84-.
  .importobj "compiled/BTLEFEInitialize.lib"
  .notice "BTL EFE initialize space left: " + (0x8006bf84-.) + " bytes"
  .fill 0x8006bf84-.
.endarea

.org 0x8006cb08
.area 0x8006cc50-.
  .importobj "compiled/BTLEFESlots.lib"
  .notice "BTL EFE slot release space left: " + (0x8006cc50-.) + " bytes"
  .fill 0x8006cc50-.
.endarea

.org 0x8006d7b0
.area 0x8006d7fc-.
  .notice "BTL EFE wait space left: " + (0x8006d7fc-.) + " bytes"
  .fill 0x8006d7fc-.
.endarea

.org 0x8006db10
.area 0x8006e058-.
  .importobj "compiled/BTLEFEAddressOperand.lib"
  .importobj "compiled/BTLEFEDispatch.lib"
  .importobj "compiled/BTLEFEIndexedOperand.lib"
  .importobj "compiled/BTLEFEControl.lib"
  .notice "BTL EFE operands and control space left: " + (0x8006e058-.) + " bytes"
  .fill 0x8006e058-.
.endarea

.org 0x8006e124
.area 0x8006e410-.
  .importobj "compiled/BTLEFEOperands.lib"
  .notice "BTL EFE basic operands space left: " + (0x8006e410-.) + " bytes"
  .fill 0x8006e410-.
.endarea

.org 0x8006e67c
.area 0x8006e74c-.
  .importobj "compiled/BTLEFESlotScript.lib"
  .notice "BTL EFE slot script space left: " + (0x8006e74c-.) + " bytes"
  .fill 0x8006e74c-.
.endarea

.org 0x8006eba8
.area 0x8006ed24-.
  .importobj "compiled/BTLEFERun.lib"
  .notice "BTL EFERun space left: " + (0x8006ed24-.) + " bytes"
  .fill 0x8006ed24-.
.endarea

; BTL_addParticleEmitter
.org 0x80073380
  .word BTL_addParticleEmitter

; BTL_initializeSubEffectInstructions
.org 0x80073310
  .word BTL_initializeSubEffectInstructions

; BTL_loadNextEFEFile
.org 0x800661d0
  j BTL_loadNextEFEFile

; BTL_playEFESound
.org 0x800733e0
  .word BTL_playEFESound

; BTL_removeEFEEngine
.org 0x80059050
  jal BTL_removeEFEEngine

; BTL_returnFromEFESubroutine
.org 0x8006d9f8
  jal BTL_returnFromEFESubroutine
.org 0x8006da6c
  jal BTL_returnFromEFESubroutine

; BTL_runEFESlotScript
.org 0x800662c0
  jal BTL_runEFESlotScript

; BTL_unloadAllEFESlots
.org 0x80059048
  jal BTL_unloadAllEFESlots

.close
