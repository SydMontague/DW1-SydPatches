.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

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


.org 0x80073480
  .word EFE_copyVector

.org 0x800734a8
  .word EFE_maskVectors

.org 0x800736ec
  .word EFE_compareEqual
.org 0x800736f0
  .word EFE_compareNotEqual
.org 0x800736f4
  .word EFE_compareLess
.org 0x800736f8
  .word EFE_compareLessOrEqual
.org 0x800736fc
  .word EFE_compareGreater
.org 0x80073700
  .word EFE_compareGreaterOrEqual

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


.org 0x8007ac9c
  .word EFE_copyVector

.org 0x8007acc4
  .word EFE_maskVectors

.org 0x8007af08
  .word EFE_compareEqual
.org 0x8007af0c
  .word EFE_compareNotEqual
.org 0x8007af10
  .word EFE_compareLess
.org 0x8007af14
  .word EFE_compareLessOrEqual
.org 0x8007af18
  .word EFE_compareGreater
.org 0x8007af1c
  .word EFE_compareGreaterOrEqual

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


.org 0x80070cc8
  .word EFE_copyVector

.org 0x80070cf0
  .word EFE_maskVectors

.org 0x80070f34
  .word EFE_compareEqual
.org 0x80070f38
  .word EFE_compareNotEqual
.org 0x80070f3c
  .word EFE_compareLess
.org 0x80070f40
  .word EFE_compareLessOrEqual
.org 0x80070f44
  .word EFE_compareGreater
.org 0x80070f48
  .word EFE_compareGreaterOrEqual

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
