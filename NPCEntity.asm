.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x801069fc
  jal loadNPCModel

.org 0x80103620
  jal scriptSetDigimon

.org 0x80106c4c
  jal unloadDigimonModel

.org 0x80103644
  jal scriptUnloadEntity
  
.org 0x800d903c
  jal removeMapEntities

;.org 0x800b6658
;  jal clearMapAiTable
.org 0x800b7b88
  jal clearMapAiTable
.org 0x800b7bec
  jal clearMapAiTable

.close


.open "work/DIGIMON/STD_REL.BIN",0x80052ae0
.psx

.org 0x80056d5c
  jal removeMapEntities
.org 0x80057bc8
  jal removeMapEntities

.close