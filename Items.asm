.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

; override setTrainingBoost to fix only last food applying training boost
.org 0x800c59b0
.area 48
  .importobj "compiled/setTrainingBoost.lib"
.endarea
.close