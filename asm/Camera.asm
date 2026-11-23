.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.close

.open "work/DIGIMON/FISH_REL.BIN",0x80070000
.psx

.org 0x80079a60
  jal unsetCameraFollowPlayer

.org 0x800799cc
  jal setCameraFollowPlayer

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
