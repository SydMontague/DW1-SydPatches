.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x800f0388
  jal initializeEFE

.close

.open "work/DIGIMON/BTL_REL.BIN",0x80052ae0
.psx

.org 0x800660ec
  jal initializeFlashData

.org 0x80073328
  .word EFE_rotateVector

.org 0x80073390
  .word EFE_createFlash

.org 0x80064f90
  jal getEFEDATEntry

.close

.open "work/DIGIMON/STD_REL.BIN",0x80052ae0
.psx

.org 0x8006e7d4
  jal initializeFlashData

.org 0x8007ab44
  .word EFE_rotateVector

.org 0x8007abac
  .word EFE_createFlash

.org 0x8006d678
  jal getEFEDATEntry

.org 0x8006e89c
  jal EFE_downloadSomeImage

.org 0x8006eb94
  j EFE_modifySomeImage

.close

.open "work/DIGIMON/VS_REL.BIN",0x80052ae0
.psx

.org 0x800648a4
  jal initializeFlashData

.org 0x80070b70
  .word EFE_rotateVector

.org 0x80070bd8
  .word EFE_createFlash

.org 0x80063748
  jal getEFEDATEntry

.org 0x8006496c
  jal EFE_downloadSomeImage

.org 0x80064c64
  j EFE_modifySomeImage

.close

.open "work/DIGIMON/EVL_REL.BIN",0x80060000
.psx

.org 0x80063164
  jal initializeFlashData

.org 0x80060d90
  jal EFE_createFlash
.org 0x800607d0
  jal EFE_createFlash

.close


.open "work/DIGIMON/EAB_REL.BIN",0x80060000
.psx

.org 0x800615b0
  jal initializeFlashData

.org 0x8006169c
  jal EFE_createFlash

.close


.open "work/DIGIMON/DOOA_REL.BIN",0x80080000
.psx

.org 0x8008467c
  jal initializeFlashData

.org 0x800824d0
  jal EFE_createFlash
.org 0x80080c64
  jal EFE_createFlash

.org 0x80080cb8
  jal EFE_setFlashOffset

.close