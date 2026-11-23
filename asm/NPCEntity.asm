.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.close


.open "work/DIGIMON/STD_REL.BIN",0x80052ae0
.psx

.org 0x80056d5c
  jal removeMapEntities
.org 0x80057bc8
  jal removeMapEntities

.close
