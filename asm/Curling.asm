.open "work/DIGIMON/KAR_REL.BIN",0x80053800
.psx

; KAR owns all code, strings, model storage, rows and ordering data after the loader header.
.org 0x80053840
.area 0x800639EC-.
  .importobj "compiled/KAR.lib"

  .notice "KAR code/data space left: " + (0x800639EC-.) + " bytes"
  .fill 0x800639EC-.
.endarea

.close
