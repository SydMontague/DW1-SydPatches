.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

@evoFlagOffset equ -lo(global_pointer-0x134e52)

;; reset item evolution flag on rebirth
;.org 0x800a53f4
;  sb r0,@evoFlagOffset(gp)

; reset item evolution flag after evolution
.org 0x800deb94
  sb r0,@evoFlagOffset(gp)

.close