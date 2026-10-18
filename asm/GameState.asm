.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx


.close

.open "work/DIGIMON/EVL_REL.BIN",0x80060000
.psx

.org 0x80063acc
  jal setDigimonRaised

.close
