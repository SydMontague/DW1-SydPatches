.open "work/DIGIMON/KAR_REL.BIN",0x80053800
.psx

; Replace KAR_getWallZone within its original overlay extent.
.org 0x80058430
.area 0x800584e8-.
  .importobj "compiled/KAR.lib"

  .notice "KAR Cave1 Empty space left: " + (0x800584e8-.) + " bytes"
  .fill 0x800584e8-.
.endarea

.org 0x80057054
  jal KAR_getWallZone

; Vanilla debug leaves at 0x800581D4 and 0x80058428 remain unchanged: jr ra; nop.
.org 0x800581DC
.area 0x80058428-.
  .importobj "compiled/KARCollision.lib"

  .notice "KAR collision space left: " + (0x80058428-.) + " bytes"
  .fill 0x80058428-.
.endarea

.org 0x800584E8
.area 0x8005873C-.
  .importobj "compiled/KARWalls.lib"

  .notice "KAR wall space left: " + (0x8005873C-.) + " bytes"
  .fill 0x8005873C-.
.endarea

.org 0x8005A868
.area 0x8005A97C-.
  .importobj "compiled/KARGeometry.lib"

  .notice "KAR geometry space left: " + (0x8005A97C-.) + " bytes"
  .fill 0x8005A97C-.
.endarea

; All distance callers now use its symbol; clear the obsolete vanilla function.
.org 0x80057294
.area 0x800572B4-.
  .fill 0x800572B4-.
.endarea

; Retarget every surviving vanilla caller; the original delay slots stay intact.
.org 0x800567CC
  jal KAR_distance
.org 0x80056DEC
  jal KAR_distance
.org 0x80057668
  jal KAR_distance
.org 0x80057704
  jal KAR_distance
.org 0x80057978
  jal KAR_distance
.org 0x80057998
  jal KAR_distance
.org 0x80057AC0
  jal KAR_distance
.org 0x80057B78
  jal KAR_distance
.org 0x80057E18
  jal KAR_distance
.org 0x80057E8C
  jal KAR_distance
.org 0x80057F98
  jal KAR_distance
.org 0x8005802C
  jal KAR_distance
.org 0x800580DC
  jal KAR_distance
.org 0x800590D0
  jal KAR_distance
.org 0x80059128
  jal KAR_distance
.org 0x8005A250
  jal KAR_distance
.org 0x8005A738
  jal KAR_distance

.org 0x800578B4
  jal KAR_computeImpactShare
.org 0x80057CEC
  jal KAR_computeImpactShare
.org 0x80057D44
  jal KAR_computeImpactShare

.org 0x8005790C
  jal KAR_computeSeparation
.org 0x80057D9C
  jal KAR_computeSeparation
.org 0x80057DF0
  jal KAR_computeSeparation

.org 0x80057120
  jal KAR_findWallContact
.org 0x80057178
  jal KAR_findWallContact

.org 0x8005712C
  jal KAR_reflectOffDiagonal
.org 0x80057184
  jal KAR_reflectOffDiagonal

.org 0x80057160
  jal KAR_placeAtContact
.org 0x800571B8
  jal KAR_placeAtContact

.org 0x80055EE4
  jal KAR_rotatePoint

.close
