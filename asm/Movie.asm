.open "work/DIGIMON/MOV_REL.BIN",0x80010000
.psx

; MOV owns its code, constants and playback workspace; Main imports its entry symbolically.
.org 0x80010040
.area 0x80083428-.
  .importobj "compiled/MOV.lib"

  .notice "MOV playback space left: " + (0x80083428-.) + " bytes"
  .fill 0x80083428-.
.endarea

.close
