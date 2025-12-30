.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x800e611c
  jal setEntityTextDigit
.org 0x800ee350
  jal setEntityTextDigit

.org 0x800e5bac
  jal initStringFT4
.org 0x800e5c78
  jal initStringFT4
.org 0x800ece90
  jal initStringFT4

.org 0x800edc1c
  jal renderNumber
.org 0x800edd6c
  jal renderNumber
.org 0x800ee3fc
  jal renderNumber
.org 0x800ee464
  jal renderNumber

.org 0x800f299c
  jal convertValueToDigits
.org 0x800f997c
  jal convertValueToDigits

.close 


.open "work/DIGIMON/BTL_REL.BIN",0x80052ae0
.psx

.org 0x800635b4
  jal setEntityTextDigit
.org 0x80063c54
  jal setEntityTextDigit
.org 0x80063f74
  jal setEntityTextDigit
.org 0x800641d0
  jal setEntityTextDigit
.org 0x800643c0
  jal setEntityTextDigit

.org 0x80063f50
  jal convertValueToDigits

.org 0x8005d320
  jal drawEntityText

.close


.open "work/DIGIMON/STD_REL.BIN",0x80052ae0
.psx

.org 0x80069bd8
  jal setEntityTextDigit
.org 0x8006a278
  jal setEntityTextDigit
.org 0x8006a598
  jal setEntityTextDigit
.org 0x8006a84c
  jal setEntityTextDigit
.org 0x8006aac4
  jal setEntityTextDigit

.org 0x80059424
  jal convertValueToDigits
.org 0x8005bedc
  jal convertValueToDigits
.org 0x8006a574
  jal convertValueToDigits

.close


.open "work/DIGIMON/VS_REL.BIN",0x80052ae0
.psx

.org 0x80060ed8
  jal setEntityTextDigit
.org 0x800615bc
  jal setEntityTextDigit
.org 0x800618f8
  jal setEntityTextDigit
.org 0x80061bac
  jal setEntityTextDigit
.org 0x80061e30
  jal setEntityTextDigit

.org 0x80057c48
  jal initStringFT4

.org 0x800579b0
  jal renderNumber
.org 0x800579dc
  jal renderNumber

.org 0x8005b15c
  jal convertValueToDigits
.org 0x800618d4
  jal convertValueToDigits

.close

.open "work/DIGIMON/TRN_REL.BIN",0x80088800
.psx

.org 0x8008da38
  jal setEntityTextDigit

.org 0x8008dae4
  jal renderNumber
.org 0x8008db4c
  jal renderNumber

.close


.open "work/DIGIMON/TRN2_REL.BIN",0x80088800
.psx

.org 0x8008c410
  jal setEntityTextDigit

.org 0x8008c4bc
  jal renderNumber
.org 0x8008c524
  jal renderNumber

.close


.open "work/DIGIMON/FISH_REL.BIN",0x80070000
.psx

.org 0x80070f6c
  jal renderItemAmount

.close