.open "work/DIGIMON/FISH_REL.BIN",0x80070000
.psx

; All retained entries and callbacks moved into the message owner.
.org 0x800713b0
.area 0x80071854-., 0
  .notice "FISH retired message space: " + (0x80071854-.) + " bytes"
.endarea

; Complete owner fits the existing 28-sector FISH load; the original logical tail ends here.
.org 0x8007d96c
.area 0x8007e000-., 0
  .importobj "compiled/FISHMessage.lib"
  .notice "FISH message space left: " + (0x8007e000-.) + " bytes"
.endarea

; FISH_initMessage
.org 0x80079b54
  jal FISH_initMessage

; FISH_isMessageVisible
.org 0x80076080
  jal FISH_isMessageVisible
.org 0x80077dd4
  jal FISH_isMessageVisible
.org 0x80079524
  jal FISH_isMessageVisible

; FISH_showMessage
.org 0x800789b8
  jal FISH_showMessage
.org 0x80078a24
  jal FISH_showMessage
.org 0x80078a54
  jal FISH_showMessage
.org 0x80078a7c
  jal FISH_showMessage
.org 0x80079150
  jal FISH_showMessage
.org 0x8007918c
  jal FISH_showMessage
.org 0x800792c0
  jal FISH_showMessage
.org 0x80079344
  jal FISH_showMessage
.org 0x80079380
  jal FISH_showMessage
.org 0x80079404
  jal FISH_showMessage
.org 0x80079440
  jal FISH_showMessage

; FISH_shutdownMessage
.org 0x80079a04
  jal FISH_shutdownMessage

.close
