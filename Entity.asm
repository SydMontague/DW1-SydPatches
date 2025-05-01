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

;.org 0x800aa6f4
;  jal entityLookAtLocation
;.org 0x800aaae8
;  jal entityLookAtLocation
;.org 0x800ac1e0
;  jal entityLookAtLocation
;.org 0x800d443c
;  jal entityLookAtLocation
.org 0x800dda9c
  jal entityLookAtLocation
.org 0x800ddab4
  jal entityLookAtLocation
.org 0x800ddda0
  jal entityLookAtLocation
.org 0x800dddb8
  jal entityLookAtLocation
.org 0x800ddf8c
  jal entityLookAtLocation
.org 0x800ddfe0
  jal entityLookAtLocation
.org 0x800ddff8
  jal entityLookAtLocation
.org 0x800de050
  jal entityLookAtLocation
.org 0x800de3fc
  jal entityLookAtLocation
.org 0x800de444
  jal entityLookAtLocation
.org 0x800de4f4
  jal entityLookAtLocation
.org 0x800de59c
  jal entityLookAtLocation
.org 0x800de5c8
  jal entityLookAtLocation
.org 0x800de728
  jal entityLookAtLocation
.org 0x800de780
  jal entityLookAtLocation
.org 0x800de980
  jal entityLookAtLocation
.org 0x800dead0
  jal entityLookAtLocation
.org 0x800deb04
  jal entityLookAtLocation
.org 0x800e60c8
  jal entityLookAtLocation
.org 0x800e7714
  jal entityLookAtLocation
.org 0x800e7814
  jal entityLookAtLocation
.org 0x800e7af0
  jal entityLookAtLocation
.org 0x800f0f54
  jal entityLookAtLocation
.org 0x800f11e0
  jal entityLookAtLocation
.org 0x800f12a8
  jal entityLookAtLocation
.org 0x800f3530
  jal entityLookAtLocation
.org 0x800f3560
  jal entityLookAtLocation
.org 0x800f4e60
  jal entityLookAtLocation
.org 0x800f4f4c
  jal entityLookAtLocation
.org 0x800f5784
  jal entityLookAtLocation
.org 0x800f5824
  jal entityLookAtLocation
.org 0x800f5c14
  jal entityLookAtLocation
.org 0x800f5cc4
  jal entityLookAtLocation
.org 0x800f5e04
  jal entityLookAtLocation
.org 0x800f5f10
  jal entityLookAtLocation
.org 0x800f5f58
  jal entityLookAtLocation
.org 0x800f6230
  jal entityLookAtLocation
.org 0x800f6378
  jal entityLookAtLocation
.org 0x800f663c
  jal entityLookAtLocation
.org 0x800f6b34
  jal entityLookAtLocation

;.org 0x800ab394
;  jal entityCheckCollision
;.org 0x800ac250
;  jal entityCheckCollision
;.org 0x800b69d0
;  jal entityCheckCollision
;.org 0x800d4454
;  jal entityCheckCollision
;.org 0x800d48cc
;  jal entityCheckCollision
;.org 0x800d4944
;  jal entityCheckCollision
.org 0x800def24
  jal entityCheckCollision
.org 0x800ef4cc
  jal entityCheckCollision
.org 0x800f5fb0
  jal entityCheckCollision
.org 0x800f63b4
  jal entityCheckCollision
.org 0x800f6428
  jal entityCheckCollision
.org 0x800f645c
  jal entityCheckCollision
.org 0x800f6480
  jal entityCheckCollision
.org 0x800f659c
  jal entityCheckCollision
.org 0x800f65c0
  jal entityCheckCollision
.org 0x800f6658
  jal entityCheckCollision
.org 0x800f670c
  jal entityCheckCollision
.org 0x800f6ef0
  jal entityCheckCollision
.org 0x800f6f70
  jal entityCheckCollision
.org 0x800f6fe8
  jal entityCheckCollision
.org 0x800f705c
  jal entityCheckCollision
.org 0x800f70ac
  jal entityCheckCollision
.org 0x800f710c
  jal entityCheckCollision

;.org 0x800ab4a8
;  jal collisionGrace
;.org 0x800b6afc
;  jal collisionGrace
;.org 0x800d44b0
;  jal collisionGrace
.org 0x800f54e4
  jal collisionGrace
.org 0x800f5554
  jal collisionGrace
.org 0x800f560c
  jal collisionGrace
.org 0x800f5ab4
  jal collisionGrace
.org 0x800f6680
  jal collisionGrace
.org 0x800f70dc
  jal collisionGrace
.org 0x800f7130
  jal collisionGrace
.org 0x800f7150
  jal collisionGrace

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

.org 0x800573d8
  jal entityLookAtLocation
.org 0x800574bc
  jal entityLookAtLocation
.org 0x8005770c
  jal entityLookAtLocation
.org 0x800577b4
  jal entityLookAtLocation
.org 0x8005784c
  jal entityLookAtLocation
.org 0x80059494
  jal entityLookAtLocation
.org 0x80059a8c
  jal entityLookAtLocation
.org 0x80059b2c
  jal entityLookAtLocation
.org 0x80059f8c
  jal entityLookAtLocation
.org 0x8005a03c
  jal entityLookAtLocation
.org 0x8005a17c
  jal entityLookAtLocation
.org 0x8005a298
  jal entityLookAtLocation
.org 0x8005a2e0
  jal entityLookAtLocation
.org 0x8005a57c
  jal entityLookAtLocation
.org 0x8005a624
  jal entityLookAtLocation
.org 0x8005a764
  jal entityLookAtLocation
.org 0x8005aeb0
  jal entityLookAtLocation
.org 0x8005ceb0
  jal entityLookAtLocation
.org 0x8005d7f4
  jal entityLookAtLocation
.org 0x8005d86c
  jal entityLookAtLocation
.org 0x8005d8f0
  jal entityLookAtLocation
.org 0x80066d0c
  jal entityLookAtLocation

.org 0x80057614
  jal entityCheckCollision
.org 0x8005768c
  jal entityCheckCollision
.org 0x8005a338
  jal entityCheckCollision
.org 0x8005a640
  jal entityCheckCollision
.org 0x8005a7a0
  jal entityCheckCollision
.org 0x8005a814
  jal entityCheckCollision
.org 0x8005a848
  jal entityCheckCollision
.org 0x8005a86c
  jal entityCheckCollision
.org 0x8005a988
  jal entityCheckCollision
.org 0x8005a9ac
  jal entityCheckCollision
.org 0x8005aa58
  jal entityCheckCollision
.org 0x8005b364
  jal entityCheckCollision
.org 0x8005b3e4
  jal entityCheckCollision
.org 0x8005b45c
  jal entityCheckCollision
.org 0x8005b4d0
  jal entityCheckCollision
.org 0x8005b520
  jal entityCheckCollision
.org 0x8005b580
  jal entityCheckCollision

.org 0x800597e8
  jal collisionGrace
.org 0x80059858
  jal collisionGrace
.org 0x80059904
  jal collisionGrace
.org 0x80059dbc
  jal collisionGrace
.org 0x8005a668
  jal collisionGrace
.org 0x8005b550
  jal collisionGrace
.org 0x8005b5a4
  jal collisionGrace
.org 0x8005b5c4
  jal collisionGrace

.close

.open "work/DIGIMON/STD_REL.BIN",0x80052ae0
.psx

.org 0x8005efe4
  jal entityLookAtLocation
.org 0x8005f050
  jal entityLookAtLocation
.org 0x80060e8c
  jal entityLookAtLocation
.org 0x800616e0
  jal entityLookAtLocation
.org 0x80061780
  jal entityLookAtLocation
.org 0x80061b54
  jal entityLookAtLocation
.org 0x80061c04
  jal entityLookAtLocation
.org 0x80061d44
  jal entityLookAtLocation
.org 0x80061e60
  jal entityLookAtLocation
.org 0x80061ea8
  jal entityLookAtLocation
.org 0x8006217c
  jal entityLookAtLocation
.org 0x800622c4
  jal entityLookAtLocation
.org 0x80062588
  jal entityLookAtLocation
.org 0x80062ab0
  jal entityLookAtLocation
.org 0x80064f40
  jal entityLookAtLocation
.org 0x80065598
  jal entityLookAtLocation
.org 0x80065610
  jal entityLookAtLocation
.org 0x80065694
  jal entityLookAtLocation
.org 0x8006f304
  jal entityLookAtLocation

.org 0x80061f00
  jal entityCheckCollision
.org 0x80062300
  jal entityCheckCollision
.org 0x80062374
  jal entityCheckCollision
.org 0x800623a8
  jal entityCheckCollision
.org 0x800623cc
  jal entityCheckCollision
.org 0x800624e8
  jal entityCheckCollision
.org 0x8006250c
  jal entityCheckCollision
.org 0x800625a4
  jal entityCheckCollision
.org 0x80062658
  jal entityCheckCollision
.org 0x80062ebc
  jal entityCheckCollision
.org 0x80062f3c
  jal entityCheckCollision
.org 0x80062fb4
  jal entityCheckCollision
.org 0x80063028
  jal entityCheckCollision
.org 0x80063078
  jal entityCheckCollision
.org 0x800630d8
  jal entityCheckCollision

.org 0x80061448
  jal collisionGrace
.org 0x800614b8
  jal collisionGrace
.org 0x80061564
  jal collisionGrace
.org 0x80061a10
  jal collisionGrace
.org 0x800625cc
  jal collisionGrace
.org 0x800630a8
  jal collisionGrace
.org 0x800630fc
  jal collisionGrace
.org 0x8006311c
  jal collisionGrace

.close

.open "work/DIGIMON/STD_REL.BIN",0x80052ae0
.psx

.org 0x8005ca94
  jal entityLookAtLocation
.org 0x8005cf50
  jal entityLookAtLocation
.org 0x8005cfdc
  jal entityLookAtLocation
.org 0x8005d060
  jal entityLookAtLocation
.org 0x800653d4
  jal entityLookAtLocation

.close


.open "work/DIGIMON/DOOA_REL.BIN",0x80080000
.psx

.org 0x800805bc
  jal entityLookAtLocation

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

.org 0x8008d0d8
  jal entityLookAtLocation
.org 0x8008d140
  jal entityLookAtLocation
.org 0x8008d158
  jal entityLookAtLocation
.org 0x8008d19c
  jal entityLookAtLocation
.org 0x8008d1b4
  jal entityLookAtLocation
.org 0x8008d1cc
  jal entityLookAtLocation

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

.org 0x80088dac
  jal entityLookAtLocation
.org 0x80088ee8
  jal entityLookAtLocation
.org 0x8008907c
  jal entityLookAtLocation
.org 0x8008a598
  jal entityLookAtLocation

.close