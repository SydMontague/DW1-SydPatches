.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

;; entityLookAt Softlock fix, happens at exactly 180° rotations
.org 0x800b7008
  nop
  nop

.org 0x800b707c
  nop
  nop

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