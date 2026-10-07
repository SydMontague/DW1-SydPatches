.open "work/DIGIMON/KAR_REL.BIN",0x80053800
.psx

; Keep the stone tick in its original allocation and retarget its only callback below.
.org 0x80053CD0
.area 0x80054048-.
  .importobj "compiled/KARMovement.lib"

  .notice "KAR tick space left: " + (0x80054048-.) + " bytes"
  .fill 0x80054048-.
.endarea

; Reclaim adjacent physics/helpers and both empty debug leaves with their callers replaced.
.org 0x800569E4
.area 0x8005873C-.
  .importobj "compiled/KAR.lib"
  .importobj "compiled/KARCollisionFlow.lib"
  .importobj "compiled/KARBounce.lib"
  .importobj "compiled/KARPhysics.lib"
  .importobj "compiled/KARCollision.lib"
  .importobj "compiled/KARWalls.lib"

  .notice "KAR physics space left: " + (0x8005873C-.) + " bytes"
  .fill 0x8005873C-.
.endarea

.org 0x8005A868
.area 0x8005A97C-.
  .importobj "compiled/KARGeometry.lib"

  .notice "KAR geometry space left: " + (0x8005A97C-.) + " bytes"
  .fill 0x8005A97C-.
.endarea

; KAR_start registers this callback with addObject; preserve the intervening instructions.
.org 0x80053C88
  lui a2, hi(KAR_tickStones)
.org 0x80053C98
  addiu a2, a2, lo(KAR_tickStones)

; Retarget surviving vanilla callers; preserve the original delay slots.
.org 0x80054618
  jal KAR_checkStonesStopped

.org 0x80055EE4
  jal KAR_rotatePoint

.org 0x800567CC
  jal KAR_distance
.org 0x800590D0
  jal KAR_distance
.org 0x80059128
  jal KAR_distance
.org 0x8005A250
  jal KAR_distance
.org 0x8005A738
  jal KAR_distance

.close
