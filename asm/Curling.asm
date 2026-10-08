.open "work/DIGIMON/KAR_REL.BIN",0x80053800
.psx

; Keep the stone tick in its original allocation and retarget its only callback below.
.org 0x80053CD0
.area 0x80054048-.
  .importobj "compiled/KARMovement.lib"

  .notice "KAR tick space left: " + (0x80054048-.) + " bytes"
  .fill 0x80054048-.
.endarea

; The replaced scroll/marker/physics/aiming region also holds the geometry helpers.
; Every surviving caller below remains symbolic; no live gap exists between these allocations.
.org 0x80056590
.area 0x80058B00-.
  .importobj "compiled/KARGeometry.lib"
  .importobj "compiled/KARRingMarkers.lib"
  .importobj "compiled/KARCollisionFlow.lib"
  .importobj "compiled/KARBounce.lib"
  .importobj "compiled/KARPhysics.lib"
  .importobj "compiled/KARCollision.lib"
  .importobj "compiled/KARWalls.lib"
  .importobj "compiled/KARAiming.lib"

  .notice "KAR movement/aiming space left: " + (0x80058B00-.) + " bytes"
  .fill 0x80058B00-.
.endarea

.org 0x80059040
.area 0x80059760-.
  .importobj "compiled/KARScoring.lib"

  .notice "KAR scoring space left: " + (0x80059760-.) + " bytes"
  .fill 0x80059760-.
.endarea

; The replaced registration and geometry regions are contiguous, with all callers retargeted.
.org 0x8005A7E8
.area 0x8005A97C-.
  .importobj "compiled/KAR.lib"
  .importobj "compiled/KARAimScroll.lib"

  .notice "KAR classifier/scroll space left: " + (0x8005A97C-.) + " bytes"
  .fill 0x8005A97C-.
.endarea

; C++ constants replace the scoring zones, ring objects and marker zones.
.org 0x8005B438
.area 0x8005B478-.
  .notice "KAR ring table space left: " + (0x8005B478-.) + " bytes"
  .fill 0x8005B478-.
.endarea

; The scoring import owns the thrown-stone order for this overlay lifetime.
.org 0x800639C0
.area 0x800639EC-.
  .notice "KAR thrown-stone table space left: " + (0x800639EC-.) + " bytes"
  .fill 0x800639EC-.
.endarea

; KAR_start registers this callback with addObject; preserve the intervening instructions.
.org 0x80053C88
  lui a2, hi(KAR_tickStones)
.org 0x80053C98
  addiu a2, a2, lo(KAR_tickStones)

; Retarget surviving vanilla callers; preserve the original delay slots.
.org 0x80054608
  jal KAR_handleAimScroll

.org 0x80054610
  jal KAR_updateRingMarkers

.org 0x80054618
  jal KAR_checkStonesStopped

.org 0x8005499C
  jal KAR_beginAiming
.org 0x80054A38
  jal KAR_beginAiming

.org 0x800549FC
  jal KAR_selectNextStone
.org 0x80054A98
  jal KAR_selectNextStone

.org 0x80054A68
  jal KAR_selectPreviousStone

.org 0x80054B5C
  jal KAR_turnAim
.org 0x80054BB4
  jal KAR_turnAim
.org 0x80054C38
  jal KAR_turnAim
.org 0x80054C48
  jal KAR_turnAim
.org 0x80054CD8
  jal KAR_turnAim
.org 0x80054D30
  jal KAR_turnAim

.org 0x80054C08
  jal KAR_beginThrow
.org 0x80054C80
  jal KAR_beginThrow

.org 0x80055394
  jal KAR_classifyStoneRings

.org 0x8005547C
  jal KAR_tickScoreTally

.org 0x80055EE4
  jal KAR_rotatePoint

.org 0x8005A250
  jal KAR_distance
.org 0x8005A738
  jal KAR_distance

.close
