.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

;; entityLookAt Softlock fix, happens at exactly 180° rotations
.org 0x800b7008
  nop
  nop

.org 0x800b707c
  nop
  nop

;; entityMoveTo Softlock fix
.org 0x800ac748
  beq v0,zero,0x800ac764

.org 0x800ac7a4
  beq v0,zero,0x800ac7c0

;; entityMoveToAxis
.org 0x800ac9ac
  beq v0,zero,0x800ac9c8

;; entityWalkTo
.org 0x800ac1bc
.area 32
  addiu a0,sp,0x28
  jal getDistanceSquared
  addiu a1,sp,0x38
  lw a0,0x00(s1)
  jal getDigimonRadius
  sw v0,0x48(sp)
  sw v0,0x4C(sp)
  nop
.endarea

.org 0x800ac260
.area 40
  lw v1,0x4C(sp)
  lw v0,0x48(sp)
  mult v1,v1
  nop
  mflo v1
  nop
  nop
  blt v0,v1,0x800ac2a0
  nop
.endarea

.close

.open "work/DIGIMON/BTL_REL.BIN",0x80052ae0
.psx

;; Fix MP Consumption Bonus glitch.
; string end check looked for '0' instead of \0
.org 0x80062ec8
  move at,r0
; It has only two lines instead of 4
.org 0x80062d58
  addi v0,v0,0x2

; shorten string to actually fit
.org 0x80072fbc
  .asciiz "#C1MP usage#R"
.org 0x80072fd8
  .asciiz "reduced by "

.close