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

.org 0x8006dcb4
.area 0x8006de20-.
  .importobj "compiled/BTLEFEIndexedOperand.lib"
  .notice "BTL EFE indexed operand space left: " + (0x8006de20-.) + " bytes"
  .fill 0x8006de20-.
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

.close
