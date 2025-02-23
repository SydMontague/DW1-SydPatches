.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x801069fc
  jal loadNPCModel

.org 0x80103620
  jal scriptSetDigimon
  
.close