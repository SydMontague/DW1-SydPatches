.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x800de41c
  jal tickEntityWalkTo
.org 0x800de464
  jal tickEntityWalkTo
.org 0x800de514
  jal tickEntityWalkTo
.org 0x800de998
  jal tickEntityWalkTo
.org 0x8010573c
  jal tickEntityWalkTo
.org 0x8010575c
  jal tickEntityWalkTo
.org 0x80105780
  jal tickEntityWalkTo
.org 0x801057a4
  jal tickEntityWalkTo

.org 0x80105704
  jal tickLookAtEntity

.org 0x8010571c
  jal tickEntitySetRotation

.org 0x80105804
  jal tickEntityMoveTo
.org 0x8010582c
  jal tickEntityMoveTo
.org 0x8010585c
  jal tickEntityMoveTo
.org 0x80105888
  jal tickEntityMoveTo

.org 0x80105908
  jal tickEntityMoveToAxis
.org 0x80105930
  jal tickEntityMoveToAxis

.org 0x800d503c
  jal checkMapCollisionX
.org 0x800d5060
  jal checkMapCollisionX

.org 0x800d5084
  jal checkMapCollisionZ
.org 0x800d50a8
  jal checkMapCollisionZ

;.org 0x800a66a8
;  jal getModelTile
;.org 0x800ab284
;  jal getModelTile
;.org 0x800ac1c4
;  jal getModelTile
;.org 0x800ac1d4
;  jal getModelTile
;.org 0x800acb1c
;  jal getModelTile
;.org 0x800b71b8
;  jal getModelTile
;.org 0x800b71c8
;  jal getModelTile
;.org 0x800b7318
;  jal getModelTile
;.org 0x800b7328
;  jal getModelTile
;.org 0x800b753c
;  jal getModelTile
;.org 0x800b754c
;  jal getModelTile
;.org 0x800b77d4
;  jal getModelTile
;.org 0x800b77e4
;  jal getModelTile
;.org 0x800c4c48
;  jal getModelTile
;.org 0x800d3a1c
;  jal getModelTile
;.org 0x800d3aa8
;  jal getModelTile
;.org 0x800d3b10
;  jal getModelTile
;.org 0x800d439c
;  jal getModelTile
.org 0x800d4d20
  jal getModelTile
.org 0x800d4e2c
  jal getModelTile
.org 0x800de62c
  jal getModelTile
.org 0x800e1a38
  jal getModelTile
.org 0x800f0e68
  jal getModelTile
.org 0x800f1050
  jal getModelTile

;.org 0x800d4508
;  jal entityIsInEntity
.org 0x800d5328
  jal entityIsInEntity

.org 0x800d46dc
  jal entityCheckCombatArea

.org 0x800d5298
  jal isRectInRect
.org 0x800d52c0
  jal isRectInRect
.org 0x800d52e8
  jal isRectInRect
.org 0x800d5310
  jal isRectInRect
.org 0x800d5358
  jal isRectInRect
.org 0x800d5388
  jal isRectInRect
.org 0x800d53b8
  jal isRectInRect
.org 0x800d53e8
  jal isRectInRect

;.org 0x800d4054
;  jal getEntityTile
;.org 0x800d4090
;  jal getEntityTile
.org 0x800d4af0
  jal getEntityTile
.org 0x800e86e0
  jal getEntityTile
.org 0x800e86fc
  jal getEntityTile
.org 0x800e8748
  jal getEntityTile

.close 

.open "work/DIGIMON/BTL_REL.BIN",0x80052ae0
.psx

.org 0x80057564
  jal getModelTile

.org 0x80058ab8
  jal getEntityTile
.org 0x80058af4
  jal getEntityTile
.org 0x8005b1b8
  j getEntityTile

.close

.open "work/DIGIMON/TRN_REL.BIN",0x80088800
.psx

.org 0x8008ac98
  jal tickEntityWalkTo
.org 0x8008b030
  jal tickEntityWalkTo
.org 0x8008b2b4
  jal tickEntityWalkTo
.org 0x8008b2ec
  jal tickEntityWalkTo
.org 0x8008b518
  jal tickEntityWalkTo
.org 0x8008b8f8
  jal tickEntityWalkTo
.org 0x8008bda4
  jal tickEntityWalkTo
.org 0x8008c2ac
  jal tickEntityWalkTo
.org 0x8008c808
  jal tickEntityWalkTo
.org 0x8008c970
  jal tickEntityWalkTo
.org 0x8008cbd8
  jal tickEntityWalkTo
.org 0x8008ce24
  jal tickEntityWalkTo
.org 0x8008d0a4
  jal tickEntityWalkTo
.org 0x8008d114
  jal tickEntityWalkTo

.close


.open "work/DIGIMON/TRN2_REL.BIN",0x80088800
.psx

.org 0x80088904
  jal tickEntityWalkTo
.org 0x80088c2c
  jal tickEntityWalkTo
.org 0x80088d88
  jal tickEntityWalkTo
.org 0x800890b0
  jal tickEntityWalkTo
.org 0x800892ec
  jal tickEntityWalkTo
.org 0x800895fc
  jal tickEntityWalkTo
.org 0x800897f4
  jal tickEntityWalkTo
.org 0x8008982c
  jal tickEntityWalkTo
.org 0x8008999c
  jal tickEntityWalkTo
.org 0x80089c3c
  jal tickEntityWalkTo
.org 0x80089fa4
  jal tickEntityWalkTo
.org 0x8008a2e0
  jal tickEntityWalkTo
.org 0x8008a5cc
  jal tickEntityWalkTo
.org 0x8008a7b8
  jal tickEntityWalkTo
.org 0x8008a810
  jal tickEntityWalkTo
.org 0x8008a9e4
  jal tickEntityWalkTo

.close