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

;.org 0x800b5384
;  jal renderNinjamonEffect

.org 0x800b5bc4
  jal loadMapObjectPosition

.org 0x800b5a98
  jal getDrawPosition

.org 0x800e68a4
  jal setMapObjectsFlag
.org 0x800e68bc
  jal setMapObjectsFlag
.org 0x800e68e4
  jal setMapObjectsFlag
.org 0x800e68fc
  jal setMapObjectsFlag
.org 0x80103cd8
  jal setMapObjectsFlag

.org 0x801058e4
  jal tickMoveObjectTo
.close


.open "work/DIGIMON/KAR_REL.BIN",0x80053800
.psx

.org 0x80056890
  jal setMapObjectsFlag
.org 0x80056950
  jal setMapObjectsFlag
.org 0x800569a8
  jal setMapObjectsFlag

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
  
.org 0x8008b380
  jal getDrawPosition


.org 0x8008b308
  jal setMapObjectsFlag
.org 0x8008b318
  jal setMapObjectsFlag
.org 0x8008b484
  jal setMapObjectsFlag
.org 0x8008b494
  jal setMapObjectsFlag
.org 0x8008bebc
  jal setMapObjectsFlag
.org 0x8008becc
  jal setMapObjectsFlag
.org 0x8008bf28
  jal setMapObjectsFlag
.org 0x8008bf38
  jal setMapObjectsFlag
.org 0x8008bff0
  jal setMapObjectsFlag
.org 0x8008c000
  jal setMapObjectsFlag


.org 0x8008b398
  jal moveMapObjects

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

.org 0x80088940
  jal setMapObjectsFlag
.org 0x80088b58
  jal setMapObjectsFlag
.org 0x80088dcc
  jal setMapObjectsFlag
.org 0x80088e58
  jal setMapObjectsFlag
.org 0x80088e68
  jal setMapObjectsFlag
.org 0x80088f24
  jal setMapObjectsFlag
.org 0x80088f34
  jal setMapObjectsFlag
.org 0x80089020
  jal setMapObjectsFlag
.org 0x80089030
  jal setMapObjectsFlag
.org 0x80089d94
  jal setMapObjectsFlag
.org 0x80089da4
  jal setMapObjectsFlag
.org 0x80089e00
  jal setMapObjectsFlag
.org 0x80089e10
  jal setMapObjectsFlag
.org 0x80089ed4
  jal setMapObjectsFlag
.org 0x80089ee4
  jal setMapObjectsFlag
.org 0x8008a30c
  jal setMapObjectsFlag
.org 0x8008a31c
  jal setMapObjectsFlag
.org 0x8008a4d8
  jal setMapObjectsFlag
.org 0x8008a4e8
  jal setMapObjectsFlag
.org 0x8008a7e4
  jal setMapObjectsFlag
.org 0x8008a904
  jal setMapObjectsFlag

.close