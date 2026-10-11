.open "work/DIGIMON/KAR_REL.BIN",0x80053800
.psx

; Preserve the overlay header and retained assets outside this allocation.
.org 0x80053840
.area 0x8005AB10-.
  .importobj "compiled/KAR.lib"

  .notice "KAR code/data space left: " + (0x8005AB10-.) + " bytes"
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
