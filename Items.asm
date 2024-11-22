.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

; fix only last food applying training boost
.org 0x800c59b0
.area 48
  beq a1,zero,@@setTrainingBoost_end
  lui at,0x8014
  sll v0,a2,0x04
  sub v1,v0,a2
  sll v0,v1,0x02
  add v0,v1,v0
  sll v0,v0,0x04
  sh v0,-0x7b50(at)
  sh a1,-0x7b52(at)
  sh a0,-0x7b54(at)
@@setTrainingBoost_end:
  jr ra
  nop
.endarea
.close