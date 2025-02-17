.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x800d5d18
  jal loadMapEntities

.org 0x800d8f84
  jal checkMapInteraction

.org 0x800d5cf4
  jal loadMapObjects

.org 0x800d5bd8
  jal clearMapObjects

.org 0x800d6738
  jal calcMapObjectOrder

.org 0x800d5cb0
  jal loadMapImage2

.org 0x800d5c44
  jal loadMapImage1

.org 0x800d5b00
  jal renderMapOverlays

.org 0x8010435c
  jal createNinjamonEffect

.org 0x800b5384
  jal renderNinjamonEffect

.org 0x800b5bc4
  jal loadMapObjectPosition

.close


.open "work/DIGIMON/TRN_REL.BIN",0x80088800
.psx

.org 0x8008ac18
  jal storeMapObjectPosition
.org 0x8008b23c
  jal storeMapObjectPosition
.org 0x8008b870
  jal storeMapObjectPosition
.org 0x8008c22c
  jal storeMapObjectPosition


.org 0x8008b080
  jal loadMapObjectPosition
.org 0x8008b4b0
  jal loadMapObjectPosition
.org 0x8008bcfc
  jal loadMapObjectPosition
.org 0x8008c7b0
  jal loadMapObjectPosition
.org 0x8008c9a8
  jal loadMapObjectPosition
  
.close


.open "work/DIGIMON/TRN2_REL.BIN",0x80088800
.psx

.org 0x80088890
  jal storeMapObjectPosition
.org 0x80088d10
  jal storeMapObjectPosition
.org 0x8008926c
  jal storeMapObjectPosition
.org 0x80089bb4
  jal storeMapObjectPosition


.org 0x80088c7c
  jal loadMapObjectPosition
.org 0x80088f14
  jal loadMapObjectPosition
.org 0x800890e8
  jal loadMapObjectPosition
.org 0x8008964c
  jal loadMapObjectPosition
.org 0x8008a604
  jal loadMapObjectPosition

.close