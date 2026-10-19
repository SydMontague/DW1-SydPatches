.open "work/DIGIMON/STD_REL.BIN",0x80052ae0
.psx

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

; Shared operand semantics are emitted in this owning overlay.
.org 0x800760ec
.area 0x8007616c-.
  .importobj "compiled/STDEFEAddressOperand.lib"
  .notice "STD EFE address operand space left: " + (0x8007616c-.) + " bytes"
  .fill 0x8007616c-.
.endarea

.org 0x80076290
.area 0x800763fc-.
  .importobj "compiled/STDEFEIndexedOperand.lib"
  .notice "STD EFE indexed operand space left: " + (0x800763fc-.) + " bytes"
  .fill 0x800763fc-.
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

.close
