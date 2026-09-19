.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x800fc6ac
  jal shopFillBuyItemList

.org 0x800fc6f0
  jal shopFillSellItemList
.org 0x800fcba0
  jal shopFillSellItemList

.org 0x800fcad0
  li v0, tickItemMenu

.org 0x800fcadc
  li v0, renderItemMenu

.org 0x800fd700
  li v0, tickItemMenuDescriptionBox

.org 0x800fd70c
  li v0, renderItemMenuDescriptionBox

.org 0x800fd1c8
  li v0, tickItemMenuAmountBox
  
.org 0x800fd1d4
  li v0, renderItemMenuAmountBox
  
.close
