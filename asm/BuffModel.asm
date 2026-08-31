.open "work/DIGIMON/STD_REL.BIN",0x80052ae0
.psx

.org 0x80056e10
  jal initializeBuffModel

.close

.open "work/DIGIMON/VS_REL.BIN",0x80052ae0
.psx

.org 0x80056e50
  jal initializeBuffModel

.close

.open "work/DIGIMON/BTL_REL.BIN",0x80052ae0
.psx

.org 0x80058fd8
  jal removeBuffModelObject

.close