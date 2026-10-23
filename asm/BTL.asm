.open "work/DIGIMON/BTL_REL.BIN",0x80052ae0
.psx

; Shared EFE commands replace these complete vanilla functions.
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

; Shared operand semantics are emitted in this owning overlay.
.org 0x8006db10
.area 0x8006db90-.
  .importobj "compiled/BTLEFEAddressOperand.lib"
  .notice "BTL EFE address operand space left: " + (0x8006db90-.) + " bytes"
  .fill 0x8006db90-.
.endarea

.org 0x8006db90
.area 0x8006dcb4-.
  .importobj "compiled/BTLEFEDispatch.lib"
  .notice "BTL EFEDispatch space left: " + (0x8006dcb4-.) + " bytes"
  .fill 0x8006dcb4-.
.endarea

.org 0x8006dcb4
.area 0x8006de20-.
  .importobj "compiled/BTLEFEIndexedOperand.lib"
  .notice "BTL EFE indexed operand space left: " + (0x8006de20-.) + " bytes"
  .fill 0x8006de20-.
.endarea

.org 0x8006de20
.area 0x8006e058-.
  .importobj "compiled/BTLEFEControl.lib"
  .notice "BTL EFEControl space left: " + (0x8006e058-.) + " bytes"
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
  .notice "BTL EFE comparison space left: " + (0x8006e74c-.) + " bytes"
  .fill 0x8006e74c-.
.endarea

.org 0x8006eba8
.area 0x8006ed24-.
  .importobj "compiled/BTLEFERun.lib"
  .notice "BTL EFERun space left: " + (0x8006ed24-.) + " bytes"
  .fill 0x8006ed24-.
.endarea

; BTL_dispatchEFEOpcode
.org 0x800656fc
  jal BTL_dispatchEFEOpcode

; BTL_initializeEFESubOpcodeTable
.org 0x80066138
  jal BTL_initializeEFESubOpcodeTable

; BTL_returnFromEFESubroutine
.org 0x8006d9f8
  jal BTL_returnFromEFESubroutine
.org 0x8006da6c
  jal BTL_returnFromEFESubroutine

; BTL_runEFEScript
.org 0x80064bc8
  jal BTL_runEFEScript
.org 0x80064db0
  jal BTL_runEFEScript
.org 0x800650e4
  j BTL_runEFEScript

.close
