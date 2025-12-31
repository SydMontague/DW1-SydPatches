.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x800f1298
  jal addInventoryUI

.org 0x800f03e4
  jal initializeInventoryUI

.org 0x800fb60c
  jal renderItemSprite
.org 0x800fddbc
  jal renderItemSprite

.close

.open "work/DIGIMON/BTL_REL.BIN",0x80052ae0
.psx

; drawInventoryText not necessary anymore
.org 0x80056da0
  nop

.org 0x80058c98
  jal closeInventoryBoxes

.close

.open "work/DIGIMON/FISH_REL.BIN",0x80070000
.psx

.org 0x8007999c
  j renderItemSprite

.close