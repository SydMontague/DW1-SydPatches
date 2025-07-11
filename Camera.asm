.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x800de3d0
  jal createCameraMovement
.org 0x800de4c8
  jal createCameraMovement
.org 0x800ef8d8
  jal createCameraMovement

.org 0x801057c0
  jal tickCameraMoveTo

.org 0x801057d8
  jal tickCameraMoveToEntity

;.org 0x800a9e80
;  jal unsetCameraFollowPlayer
;.org 0x800aa364
;  jal unsetCameraFollowPlayer
;.org 0x800aa6cc
;  jal unsetCameraFollowPlayer
;.org 0x800aaf48
;  jal unsetCameraFollowPlayer
;.org 0x800ab588
;  jal unsetCameraFollowPlayer
;.org 0x800aba94
;  jal unsetCameraFollowPlayer
;.org 0x800d82fc
;  jal unsetCameraFollowPlayer
;.org 0x800dda44
;  jal unsetCameraFollowPlayer
;.org 0x800ddd40
;  jal unsetCameraFollowPlayer
.org 0x800de33c
  jal unsetCameraFollowPlayer
.org 0x800de5a4
  jal unsetCameraFollowPlayer
.org 0x800de730
  jal unsetCameraFollowPlayer
.org 0x800de7d4
  jal unsetCameraFollowPlayer
.org 0x800de944
  jal unsetCameraFollowPlayer
.org 0x800deab8
  jal unsetCameraFollowPlayer
.org 0x800df5c0
  jal unsetCameraFollowPlayer
.org 0x80106330
  jal unsetCameraFollowPlayer

;.org 0x800aa620
;  jal setCameraFollowPlayer
;.org 0x800aaa30
;  jal setCameraFollowPlayer
;.org 0x800ab1b4
;  jal setCameraFollowPlayer
;.org 0x800b8e2c
;  jal setCameraFollowPlayer
;.org 0x800ba3e0
;  jal setCameraFollowPlayer
;.org 0x800d86f8
;  jal setCameraFollowPlayer
;.org 0x800d8f7c
;  jal setCameraFollowPlayer
;.org 0x800ddcd4
;  jal setCameraFollowPlayer
;.org 0x800dde98
;  jal setCameraFollowPlayer
.org 0x800de1bc
  jal setCameraFollowPlayer
.org 0x800de2a8
  jal setCameraFollowPlayer
.org 0x800de69c
  jal setCameraFollowPlayer
.org 0x800dea10
  jal setCameraFollowPlayer
.org 0x800debf8
  jal setCameraFollowPlayer
.org 0x801061f0
  jal setCameraFollowPlayer

.close

.open "work/DIGIMON/FISH_REL.BIN",0x80070000
.psx

.org 0x80079a60
  jal unsetCameraFollowPlayer

.org 0x800799cc
  jal setCameraFollowPlayer

.close

.open "work/DIGIMON/KAR_REL.BIN",0x80053800
.psx

.org 0x80054db4
  jal tickCameraMoveTo
.org 0x800554c4
  jal tickCameraMoveTo
.org 0x80055684
  jal tickCameraMoveTo
.org 0x80055860
  jal tickCameraMoveTo
.org 0x800594c4
  jal tickCameraMoveTo

.org 0x80055378
  jal moveCameraByDiff

.org 0x8005666c
  jal moveCameraByOffset

.close

.open "work/DIGIMON/TRN_REL.BIN",0x80088800
.psx

.org 0x800888ec
  jal createCameraMovement
.org 0x8008ac48
  jal createCameraMovement
.org 0x8008af70
  jal createCameraMovement
.org 0x8008b26c
  jal createCameraMovement
.org 0x8008b4c4
  jal createCameraMovement
.org 0x8008b8a8
  jal createCameraMovement
.org 0x8008bd1c
  jal createCameraMovement
.org 0x8008c038
  jal createCameraMovement
.org 0x8008c264
  jal createCameraMovement
.org 0x8008c8b8
  jal createCameraMovement
.org 0x8008cb88
  jal createCameraMovement
.org 0x8008cd98
  jal createCameraMovement
.org 0x8008d068
  jal createCameraMovement
.org 0x8008d39c
  jal createCameraMovement

.org 0x8008b880
  jal unsetCameraFollowPlayer
.org 0x8008c23c
  jal unsetCameraFollowPlayer
.org 0x8008cb60
  jal unsetCameraFollowPlayer

.org 0x8008bdd8
  jal setCameraFollowPlayer
.org 0x8008c9b8
  jal setCameraFollowPlayer
.org 0x8008ce50
  jal setCameraFollowPlayer
  
.close 

.open "work/DIGIMON/TRN2_REL.BIN",0x80088800
.psx

.org 0x800888c0
  jal createCameraMovement
.org 0x80088b6c
  jal createCameraMovement
.org 0x80088d40
  jal createCameraMovement
.org 0x80088fe4
  jal createCameraMovement
.org 0x8008929c
  jal createCameraMovement
.org 0x80089538
  jal createCameraMovement
.org 0x800897ac
  jal createCameraMovement
.org 0x8008995c
  jal createCameraMovement
.org 0x80089bec
  jal createCameraMovement
.org 0x80089f1c
  jal createCameraMovement
.org 0x8008a2a4
  jal createCameraMovement
.org 0x8008a520
  jal createCameraMovement
.org 0x8008a768
  jal createCameraMovement
.org 0x8008a954
  jal createCameraMovement
.org 0x8008aad0
  jal createCameraMovement

.org 0x80089bc4
  jal unsetCameraFollowPlayer
.org 0x8008a740
  jal unsetCameraFollowPlayer

.org 0x800890f8
  jal setCameraFollowPlayer
.org 0x80089fd8
  jal setCameraFollowPlayer
.org 0x8008a614
  jal setCameraFollowPlayer
.org 0x8008aa10
  jal setCameraFollowPlayer

.close

.open "work/DIGIMON/DOOA_REL.BIN",0x80080000
.psx

.org 0x80080f58
  jal tickCameraMovement

.close