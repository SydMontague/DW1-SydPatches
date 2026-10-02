.open "work/DIGIMON/KAR_REL.BIN",0x80053800
.psx

; Replace KAR_getWallZone within its original overlay extent.
.org 0x80058430
.area 0x800584e8-.
  .importobj "compiled/KAR.lib"

  .notice "KAR Cave1 Empty space left: " + (0x800584e8-.) + " bytes"
  .fill 0x800584e8-.
.endarea

.org 0x80057054
  jal KAR_getWallZone

.close
