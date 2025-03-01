.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

;.org 0x800a9e60
;  jal addGameMenu
.org 0x800db070
  jal addGameMenu

;.org 0x800aba9c
;  jal closeTriangleMenu
.org 0x800b8e1c
  jal closeTriangleMenu
.org 0x800ba2d0
  jal closeTriangleMenu
.org 0x800ba320
  jal closeTriangleMenu
.org 0x800ba388
  jal closeTriangleMenu
.org 0x800ba3d8
  jal closeTriangleMenu
.org 0x800db068
  jal closeTriangleMenu

.close