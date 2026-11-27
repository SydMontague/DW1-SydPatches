.open "work/DIGIMON/ETCHI/EFEDAT.EFE",0

; Mark faces that keep their authored RGB when the effect changes the object's color.
; The fourth face-color byte is spare on these 32-byte textured quads.
.macro keepEFEFaceColors,count
  .byte 1
  .if count > 1
    .skip 31
    keepEFEFaceColors (count - 1)
  .endif
.endmacro

; Deadly Bomb (file 330), object 4: keep the round body while the studs flash.
.org 0x1748a8 + 23
  keepEFEFaceColors 32

; Aerial Attack (file 340), object 0: keep the original tint of the trailing surface.
.org 0x1a5c0c + 23
  keepEFEFaceColors 16

.close
