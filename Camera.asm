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

.close

.open "work/DIGIMON/DOOA_REL.BIN",0x80080000
.psx

.org 0x80080f58
  jal tickCameraMovement

.close