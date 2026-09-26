.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

;.org 0x800e6958
;  jal allocateArray
;.org 0x800fc100
;  jal allocateArray
.org 0x800fc88c
  jal allocateArray
.org 0x800fc898
  jal allocateArray
.org 0x800fe53c
  jal allocateArray
.org 0x80106e78
  jal allocateArray

;.org 0x800e6bec
;  jal freeArray
;.org 0x800fc190
;  jal freeArray
;.org 0x800fc2a8
;  jal freeArray
.org 0x800fc940
  jal freeArray
.org 0x800fc94c
  jal freeArray
.org 0x800fe5c0
  jal freeArray
.org 0x80106ef4
  jal freeArray

.close


.open "work/DIGIMON/DGET_REL.BIN",0x80080800
.psx

.org 0x80081ad4
  jal allocateArray

.org 0x80081b34
  jal freeArray

.close
