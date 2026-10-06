.open "work/DIGIMON/KAR_REL.BIN",0x80053800
.psx

.org 0x80053840
.area 0x80053878-.
  .importobj "compiled/KAROrdering.lib"

  .notice "KAR ordering space left: " + (0x80053878-.) + " bytes"
  .fill 0x80053878-.
.endarea

; The replaced stone tick and scene regions are contiguous; both callbacks remain symbolic.
.org 0x80053CD0
.area 0x80054490-.
  .importobj "compiled/KARMovement.lib"
  .importobj "compiled/KARRender.lib"

  .notice "KAR stone tick/scene space left: " + (0x80054490-.) + " bytes"
  .fill 0x80054490-.
.endarea

; All replaced gameplay/rendering allocations are contiguous, with every surviving caller retargeted below.
.org 0x80055A94
.area 0x8005AB10-.
  .importobj "compiled/KARHud.lib"
  .importobj "compiled/KARGeometry.lib"
  .importobj "compiled/KARRingMarkers.lib"
  .importobj "compiled/KARCollisionFlow.lib"
  .importobj "compiled/KARBounce.lib"
  .importobj "compiled/KARPhysics.lib"
  .importobj "compiled/KARCollision.lib"
  .importobj "compiled/KARWalls.lib"
  .importobj "compiled/KARAiming.lib"
  .importobj "compiled/KARHints.lib"
  .importobj "compiled/KARScoring.lib"
  .importobj "compiled/KAROpponent.lib"
  .importobj "compiled/KARHintPages.lib"
  .importobj "compiled/KAROpponentShot.lib"
  .importobj "compiled/KAR.lib"
  .importobj "compiled/KARAimScroll.lib"
  .importobj "compiled/KARSprite.lib"

  .notice "KAR gameplay/rendering space left: " + (0x8005AB10-.) + " bytes"
  .fill 0x8005AB10-.
.endarea

; The hint helper owns the shared immutable page offsets.
.org 0x8005B04C
.area 0x8005B058-.
  .notice "KAR penguin hint offset space left: " + (0x8005B058-.) + " bytes"
  .fill 0x8005B058-.
.endarea

; Private C++ data replaces the shot priorities, hint offsets, rings, sprites, glyphs and view templates.
.org 0x8005B40C
.area 0x8005B5A0-.
  .notice "KAR constant data space left: " + (0x8005B5A0-.) + " bytes"
  .fill 0x8005B5A0-.
.endarea

; The scoring import owns the thrown-stone order for this overlay lifetime.
.org 0x800639C0
.area 0x800639EC-.
  .notice "KAR thrown-stone table space left: " + (0x800639EC-.) + " bytes"
  .fill 0x800639EC-.
.endarea

.org 0x80053C64
  jal KAR_initializeOrderingTables

; KAR_start callback: preserve intervening instructions and original delay slots.

.org 0x80053C88
  lui a2, hi(KAR_tickStones)
.org 0x80053C98
  addiu a2, a2, lo(KAR_tickStones)

; KAR_start scene callback: the low half remains in the addObject delay slot.

.org 0x80053C8C
  lui a3, hi(KAR_renderScene)
.org 0x80053CA0
  addiu a3, a3, lo(KAR_renderScene)

; Retarget surviving vanilla callers; preserve the original delay slots.

.org 0x800545D8
  jal KAR_renderAimArrow

.org 0x800545E0
  jal KAR_renderStoneCursor

.org 0x800545E8
  jal KAR_renderScores

.org 0x800545F0
  jal KAR_renderPowerMeter

.org 0x800545F8
  jal KAR_renderReadyPrompt

.org 0x80054600
  jal KAR_renderNamePlates

.org 0x80054608
  jal KAR_handleAimScroll

.org 0x80054610
  jal KAR_updateRingMarkers

.org 0x80054618
  jal KAR_checkStonesStopped

.org 0x80054790
  jal KAR_tickYesNoPrompt

.org 0x800548E4
  jal KAR_tickHintBox
.org 0x80054AF8
  jal KAR_tickHintBox
.org 0x80054F48
  jal KAR_tickHintBox
.org 0x80055544
  jal KAR_tickHintBox
.org 0x80055A2C
  jal KAR_tickHintBox

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

.org 0x80055994
  jal KAR_chooseOpponentShot

.close
