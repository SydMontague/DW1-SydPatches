
.open "work/DIGIMON/TRN_REL.BIN",0x80088800
.psx

.org 0x80089ca8
  jal createMenuBox
.org 0x8008a9c8
  jal createMenuBox

.close