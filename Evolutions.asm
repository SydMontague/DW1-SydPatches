.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

@evoFlagOffset equ -lo(global_pointer-0x134e52)

; reset item evolution flag on rebirth
.org 0x800a53f4
  sb r0,@evoFlagOffset(gp)

; reset item evolution flag after evolution
.org 0x800deb94
  sb r0,@evoFlagOffset(gp)

; fix evolution priority for in-training -> rookie
.org 0x800e2664
  nop
  nop

; fix evolution priority for rookie -> champion
.org 0x800e2e7c
  beq at,zero,@@elseLabel
  nop
  sll s3,s0,0x10
  move s4,v1
  sra s3,s3,0x10
@@elseLabel:
  clear s0
  clear s1
  nop

; fix evolution priority for champion -> ultimate
.org 0x800e31d0
  beq at,zero,@@elseLabel
  nop
  sll s3,s0,0x10
  move s5,v1
  sra s3,s3,0x10
@@elseLabel:
  clear s0
  clear s1
  nop

.close