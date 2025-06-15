.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x800f1298
  jal addInventoryUI

;.org 0x800ab43c
;  jal closeInventoryBoxes
;.org 0x800b6a64
;  jal closeInventoryBoxes
;.org 0x800c3944
;  jal closeInventoryBoxes
;.org 0x800dbb9c
;  jal closeInventoryBoxes
.org 0x800de6f8
  jal closeInventoryBoxes
.org 0x800deaa0
  jal closeInventoryBoxes

.org 0x800f03e4
  jal initializeInventoryUI

.close

.open "work/DIGIMON/BTL_REL.BIN",0x80052ae0
.psx

; drawInventoryText not necessary anymore
.org 0x80056da0
  nop

.org 0x80058c98
  jal closeInventoryBoxes

.close