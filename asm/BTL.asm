.open "work/DIGIMON/BTL_REL.BIN",0x80052ae0
.psx

; Shared EFE source is emitted only in this owning overlay.
.org 0x80064a80
.area 0x80064e0c-.
  .importobj "compiled/BTLEFEFrame.lib"
  .notice "BTL EFE tick/render space left: " + (0x80064e0c-.) + " bytes"
  .fill 0x80064e0c-.
.endarea

; Keep the vanilla sound clear function before the replacement stop operation.
.org 0x80064e48
.area 0x80064eac-.
  .importobj "compiled/BTLEFESound.lib"
  .notice "BTL EFE stop sounds space left: " + (0x80064eac-.) + " bytes"
  .fill 0x80064eac-.
.endarea

.org 0x80064eac
.area 0x80065800-.
  .importobj "compiled/BTLEFELifecycle.lib"
  .notice "BTL EFE lifecycle space left: " + (0x80065800-.) + " bytes"
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
  .notice "BTL EFE render space left: " + (0x80066198-.) + " bytes"
  .fill 0x80066198-.
.endarea

.org 0x800661d8
.area 0x80066260-.
  .importobj "compiled/BTLEFESlotScript.lib"
  .notice "BTL EFE slot script space left: " + (0x80066260-.) + " bytes"
  .fill 0x80066260-.
.endarea

.org 0x80068d60
.area 0x80068e98-.
  .notice "BTL retired EFE scalar vector space left: " + (0x80068e98-.) + " bytes"
  .fill 0x80068e98-.
.endarea

.org 0x8006a688
.area 0x8006a96c-.
  .importobj "compiled/BTLEFEVectorArithmetic.lib"
  .notice "BTL EFE vector arithmetic space left: " + (0x8006a96c-.) + " bytes"
  .fill 0x8006a96c-.
.endarea

.org 0x8006cb08
.area 0x8006cc50-.
  .notice "BTL EFE slot release space left: " + (0x8006cc50-.) + " bytes"
  .fill 0x8006cc50-.
.endarea

.org 0x8006db10
.area 0x8006e67c-.
  .importobj "compiled/BTLEFEAddressOperand.lib"
  .importobj "compiled/BTLEFEDispatch.lib"
  .importobj "compiled/BTLEFEIndexedOperand.lib"
  .importobj "compiled/BTLEFEControl.lib"
  .importobj "compiled/BTLEFEOperands.lib"
  .importobj "compiled/BTLEFEArithmetic.lib"
  .notice "BTL EFE operands, control and arithmetic space left: " + (0x8006e67c-.) + " bytes"
  .fill 0x8006e67c-.
.endarea

.org 0x8006e67c
.area 0x8006e74c-.
  .importobj "compiled/BTLEFEInitialize.lib"
  .notice "BTL EFE initialize space left: " + (0x8006e74c-.) + " bytes"
  .fill 0x8006e74c-.
.endarea

.org 0x8006eba8
.area 0x8006ed24-.
  .importobj "compiled/BTLEFERun.lib"
  .notice "BTL EFERun space left: " + (0x8006ed24-.) + " bytes"
  .fill 0x8006ed24-.
.endarea

.org 0x8007364c
.area 0x800736ec-.
  .notice "BTL retired EFE operator table space left: " + (0x800736ec-.) + " bytes"
  .fill 0x800736ec-.
.endarea

; BTL_addParticleEmitter
.org 0x80073380
  .word BTL_addParticleEmitter

; BTL_applyEFEVariableOperator
.org 0x80073614
  .word BTL_applyEFEVariableOperator

; EFE vector arithmetic subopcodes
.org 0x80073488
  .word BTL_addVectors
.org 0x80073490
  .word BTL_subtractVectors
.org 0x80073498
  .word BTL_multiplyVectors
.org 0x800734a0
  .word BTL_divideVectors
.org 0x800734b0
  .word BTL_shiftVectorsRight
.org 0x80073538
  .word BTL_multiplyVectorByScalar
.org 0x80073540
  .word BTL_divideVectorByScalar
.org 0x80073548
  .word BTL_maskVectorByScalar

; BTL_loadNextEFEFile
.org 0x800661d0
  j BTL_loadNextEFEFile

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

; Engine removal now owns slot unloading; the existing delay slot is nop.
.org 0x80059048
  nop

.close
