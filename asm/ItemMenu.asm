.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x800fc6ac
  jal shopFillBuyItemList

.org 0x800fc6f0
  jal shopFillSellItemList
.org 0x800fcba0
  jal shopFillSellItemList

.close
