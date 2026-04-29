.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x800fbf84
.area 24
  j inputInit
  nop
.endarea

.org 0x800fbf9c
.area 188
  j processInput
  nop
.endarea

.org 0x800fc054
.area 56
  j isKeyDown
  nop
.endarea

.close
