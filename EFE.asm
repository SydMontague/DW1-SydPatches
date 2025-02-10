.open "work/DIGIMON/BTL_REL.BIN",0x80052ae0
.psx

.org 0x800660ec
  jal initializeFlashData

.org 0x80073328
  .word EFE_rotateVector

.org 0x80073390
  .word EFE_createFlash

.close


.open "work/DIGIMON/STD_REL.BIN",0x80052ae0
.psx

.org 0x8006e7d4
  jal initializeFlashData

.org 0x8007ab44
  .word EFE_rotateVector

.org 0x8007abac
  .word EFE_createFlash

.close

.open "work/DIGIMON/VS_REL.BIN",0x80052ae0
.psx

.org 0x800648a4
  jal initializeFlashData

.org 0x80070b70
  .word EFE_rotateVector

.org 0x80070bd8
  .word EFE_createFlash

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

.close