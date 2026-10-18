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

.close
