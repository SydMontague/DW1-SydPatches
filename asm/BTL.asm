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

.close
