.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x800d5d18
  jal loadMapEntities

.org 0x800d8f84
  jal checkMapInteraction

.org 0x800d5cf4
  jal loadMapObjects

.org 0x800d5bd8
  jal clearMapObjects

.org 0x800d6738
  jal calcMapObjectOrder

.org 0x800d5cb0
  jal loadMapImage2

.org 0x800d5c44
  jal loadMapImage1

.close