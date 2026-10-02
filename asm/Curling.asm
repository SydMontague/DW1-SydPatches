.open "work/DIGIMON/KAR_REL.BIN",0x80053800
.psx

; Stopped scan, collision orchestration, bounce and resolver; includes the old distance slot.
.org 0x800569E4
.area 0x8005757C-.
  .importobj "compiled/KAR.lib"
  .importobj "compiled/KARCollisionFlow.lib"
  .importobj "compiled/KARBounce.lib"

  .notice "KAR collision flow space left: " + (0x8005757C-.) + " bytes"
  .fill 0x8005757C-.
.endarea

; The debug-stone leaf at 0x800581D4 remains unchanged: jr ra; nop.
; Reclaim the adjacent helpers and empty debug-pair leaf after removing its only call.
.org 0x800581DC
.area 0x8005873C-.
  .importobj "compiled/KARCollision.lib"
  .importobj "compiled/KARWalls.lib"

  .notice "KAR collision and wall space left: " + (0x8005873C-.) + " bytes"
  .fill 0x8005873C-.
.endarea

.org 0x8005A868
.area 0x8005A97C-.
  .importobj "compiled/KARGeometry.lib"

  .notice "KAR geometry space left: " + (0x8005A97C-.) + " bytes"
  .fill 0x8005A97C-.
.endarea

; Retarget surviving vanilla callers; preserve the original delay slots.
.org 0x80054014
  jal KAR_updateCollisions

.org 0x80054618
  jal KAR_checkStonesStopped

.org 0x80055EE4
  jal KAR_rotatePoint

.org 0x800567CC
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

.org 0x80057C98
  nop ; Remove the empty KAR_debugCollisionPair call; retain its delay slot.

.close
