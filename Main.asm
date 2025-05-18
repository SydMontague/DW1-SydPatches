.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x800ee9bc
  jal initializeTamer
.org 0x800eea58
  jal initializeTamer
.org 0x800eeb14
  jal initializeTamer

.org 0x80090010
  .word EntryPoint

.org 0x800ee86c
  jal initializeGsTMDMap

.org 0x800ee87c
  jal initializeAttackObjects

.org 0x800eebb8
  jal cleanupGame

.close

.open "work/DIGIMON/BTL_REL.BIN",0x80052ae0
.psx

.org 0x800653d4
  jal updateTMDTextureData


.org 0x80056d60
  jal initializeAttackObjects

.org 0x80066714
  jal addAttackObject
.org 0x8006696c
  jal addAttackObject
.org 0x80066c90
  jal addAttackObject
.org 0x8006d320
  jal addAttackObject
.org 0x80070ddc
  jal addAttackObject

.org 0x8005c804
  jal popAttackObject
.close

.open "work/DIGIMON/VS_REL.BIN",0x80052ae0
.psx

.org 0x80063b8c
  jal updateTMDTextureData

.org 0x800f2e9c
  jal initializeAttackObjects

.org 0x80064ddc
  jal addAttackObject
.org 0x80065034
  jal addAttackObject
.org 0x80065358
  jal addAttackObject
.org 0x8006b9cc
  jal addAttackObject
.org 0x8006f44c
  jal addAttackObject

.org 0x8005c448
  jal popAttackObject

.close

.open "work/DIGIMON/STD_REL.BIN",0x80052ae0
.psx

.org 0x8006dabc
  jal updateTMDTextureData

.org 0x8005e97c
  jal initializeAttackObjects

.org 0x8006ed0c
  jal addAttackObject
.org 0x8006ef64
  jal addAttackObject
.org 0x8006f288
  jal addAttackObject
.org 0x800758fc
  jal addAttackObject
.org 0x8007937c
  jal addAttackObject

.org 0x80064864
  jal popAttackObject

.close