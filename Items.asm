.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x80102d04
  jal setInventorySize

.org 0x800db190
  jal removeItem
.org 0x800f04d8
  jal removeItem
.org 0x800fb378
  jal removeItem
.org 0x800fc28c
  jal removeItem
.org 0x80102eb0
  jal removeItem
.org 0x8010407c
  jal removeItem
.org 0x801040a8
  jal removeItem
.org 0x80108840
  jal removeItem
.org 0x80108a44
  jal removeItem
.org 0x80108c30
  jal removeItem
.org 0x8010c664
  jal removeItem

.org 0x800fb410
  jal giveItem
.org 0x80102e6c
  jal giveItem
.org 0x80108798
  jal giveItem
.org 0x8010896c
  jal giveItem
.org 0x80108b68
  jal giveItem
.org 0x8010bf3c
  jal giveItem
.org 0x8010c648
  jal giveItem

;.org 0x800a6db4
;  jal getItemCount
.org 0x800df0ac
  jal getItemCount
.org 0x800fd0ac
  jal getItemCount
.org 0x80102608
  jal getItemCount
.org 0x80107cbc
  jal getItemCount
.org 0x80108760
  jal getItemCount
.org 0x80108828
  jal getItemCount
.org 0x80108930
  jal getItemCount
.org 0x80108b28
  jal getItemCount

.org 0x800f03ec
  jal initializeInventory

;.org 0x800c393c
;  jal removeTamerItem
;.org 0x800c455c
;  jal removeTamerItem
.org 0x800de208
  jal removeTamerItem
.org 0x800de2a0
  jal removeTamerItem
.org 0x800f06cc
  jal removeTamerItem

.close


.open "work/DIGIMON/TRN_REL.BIN",0x80088800
.psx

.org 0x8008a220
  jal getItemCount

.close


.open "work/DIGIMON/TRN2_REL.BIN",0x80088800
.psx

.org 0x8008ba24
  jal getItemCount

.close

.open "work/DIGIMON/FISH_REL.BIN",0x80070000
.psx

.org 0x80076338
  jal removeItem

.org 0x80079110
  jal giveItem
.org 0x800793c4
  jal giveItem

.close