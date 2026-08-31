

.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.close 

.open "work/DIGIMON/BTL_REL.BIN",0x80052ae0
.psx

.org 0x8006d92c
  jal getOriginalType
.org 0x800710bc
  jal getOriginalType

.close

.open "work/DIGIMON/STD_REL.BIN",0x80052ae0
.psx

.org 0x80075f08
  jal getOriginalType
.org 0x8007965c
  jal getOriginalType

.close

.open "work/DIGIMON/VS_REL.BIN",0x80052ae0
.psx

.org 0x8006bfd8
  jal getOriginalType
.org 0x8006f72c
  jal getOriginalType

.close