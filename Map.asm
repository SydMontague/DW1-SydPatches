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

;.org 0x800b5bc4
;  jal loadMapObjectPosition

;.org 0x800b5a98
;  jal getDrawPosition

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

.org 0x80104f28
  jal spawnSpriteAtLocation

.org 0x80104f58
  jal spawnSpriteAtEntity

.org 0x80104140
  jal resetMapObjectAnimation

.org 0x80105e64
  jal tickRemoveMist

.org 0x800d5d04
  jal clearMapDigimon

.org 0x800d5d90
  jal loadMapCollisionData

.org 0x800e1a48
  jal setImpassableSquare

.org 0x80104edc
  jal setImpassableRect

;.org 0x800ba4cc
;  j renderOverworldItem
;.org 0x800c4ca0
;  jal renderOverworldItem
.org 0x800f06e8
  j renderOverworldItem

.org 0x800d9044
  jal clearDroppedItems

.org 0x80104d40
  jal spawnItem

.org 0x800eda74
  jal spawnDroppedItem

.org 0x800f03dc
  jal initializeDroppedItems

;.org 0x800d3498
;  jal isTileOffScreen
;.org 0x800d35d8
;  jal isTileOffScreen
.org 0x800d4284
  jal isTileOffScreen
.org 0x800d4d9c
  jal isTileOffScreen

.org 0x800d4e50
  jal getClosestTileOffScreen

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


.org 0x8008ad5c
  jal moveMapObjectsWithLimit
.org 0x8008ad9c
  jal moveMapObjectsWithLimit
.org 0x8008adc0
  jal moveMapObjectsWithLimit
.org 0x8008b018
  jal moveMapObjectsWithLimit
.org 0x8008bad0
  jal moveMapObjectsWithLimit
.org 0x8008bb00
  jal moveMapObjectsWithLimit
.org 0x8008bb94
  jal moveMapObjectsWithLimit
.org 0x8008bc04
  jal moveMapObjectsWithLimit
.org 0x8008c35c
  jal moveMapObjectsWithLimit
.org 0x8008c488
  jal moveMapObjectsWithLimit
.org 0x8008c550
  jal moveMapObjectsWithLimit
.org 0x8008c5c4
  jal moveMapObjectsWithLimit
.org 0x8008c638
  jal moveMapObjectsWithLimit
.org 0x8008c6ac
  jal moveMapObjectsWithLimit
.org 0x8008c720
  jal moveMapObjectsWithLimit
.org 0x8008c788
  jal moveMapObjectsWithLimit

.org 0x8008b398
  jal moveMapObjects

.org 0x8008bed8
  jal resetMapObjectAnimation

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


.org 0x80088e78
  jal resetMapObjectAnimation
.org 0x80089db0
  jal resetMapObjectAnimation
.org 0x8008a32c
  jal resetMapObjectAnimation
.org 0x8008a438
  jal resetMapObjectAnimation


.org 0x800889cc
  jal moveMapObjectsWithLimit
.org 0x80088a0c
  jal moveMapObjectsWithLimit
.org 0x80088a30
  jal moveMapObjectsWithLimit
.org 0x80088c14
  jal moveMapObjectsWithLimit
.org 0x80088e20
  jal moveMapObjectsWithLimit
.org 0x800893b0
  jal moveMapObjectsWithLimit
.org 0x800893f0
  jal moveMapObjectsWithLimit
.org 0x80089414
  jal moveMapObjectsWithLimit
.org 0x800895e4
  jal moveMapObjectsWithLimit

.close