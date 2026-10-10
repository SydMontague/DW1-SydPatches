.open "work/DIGIMON/KAR_REL.BIN",0x80053800
.psx

.org 0x80053840
.area 0x80053878-.
  .importobj "compiled/KAROrdering.lib"

  .notice "KAR ordering space left: " + (0x80053878-.) + " bytes"
  .fill 0x80053878-.
.endarea

.org 0x80053878
.area 0x80053CD0-.
  .importobj "compiled/KARMatch.lib"

  .notice "KAR match setup space left: " + (0x80053CD0-.) + " bytes"
  .fill 0x80053CD0-.
.endarea

; The replaced stone tick and scene regions are contiguous; both callbacks remain symbolic.
.org 0x80053CD0
.area 0x80054490-.
  .importobj "compiled/KARMovement.lib"
  .importobj "compiled/KARRender.lib"

  .notice "KAR stone tick/scene space left: " + (0x80054490-.) + " bytes"
  .fill 0x80054490-.
.endarea

.org 0x80054490
.area 0x80055A94-.
  .importobj "compiled/KARMatchState.lib"

  .notice "KAR match state space left: " + (0x80055A94-.) + " bytes"
  .fill 0x80055A94-.
.endarea

; All replaced gameplay/rendering allocations are contiguous; their callers use imported symbols.
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

; Match setup owns these immutable numeric tables; the following model-path asset stays live.
.org 0x8005AB80
.area 0x8005ABA4-.
  .notice "KAR setup table space left: " + (0x8005ABA4-.) + " bytes"
  .fill 0x8005ABA4-.
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

; Match state owns the current camera position and keeps its previous-position copy on the stack.
.org 0x800638F4
.area 0x80063914-.
  .notice "KAR camera position space left: " + (0x80063914-.) + " bytes"
  .fill 0x80063914-.
.endarea

; The scoring import owns the thrown-stone order for this overlay lifetime.
.org 0x800639C0
.area 0x800639EC-.
  .notice "KAR thrown-stone table space left: " + (0x800639EC-.) + " bytes"
  .fill 0x800639EC-.
.endarea

.close
