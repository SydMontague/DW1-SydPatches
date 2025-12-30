.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x800e611c
  jal setEntityTextDigit
.org 0x800ee350
  jal setEntityTextDigit

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

.org 0x800e6598
  jal renderString
.org 0x800edcb0
  jal renderString
.org 0x800ede40
  jal renderString
.org 0x800ee548
  jal renderString
.org 0x800f27d8
  jal renderString
.org 0x800f73c4
  jal renderString
.org 0x800f73f8
  jal renderString
.org 0x800f7428
  jal renderString
.org 0x800f9e20
  jal renderString
.org 0x800fb13c
  jal renderString
.org 0x800fb6b0
  jal renderString
.org 0x800fbb88
  jal renderString
.org 0x800fbce4
  jal renderString
.org 0x800fbd1c
  jal renderString
.org 0x800fdf30
  jal renderString
.org 0x801001b4
  jal renderString
.org 0x80101388
  jal renderString
.org 0x8010911c
  jal renderString
.org 0x80109aec
  jal renderString
.org 0x80109b28
  jal renderString
.org 0x8010a50c
  jal renderString
.org 0x8010a59c
  jal renderString
.org 0x8010a62c
  jal renderString
.org 0x8010a704
  jal renderString
.org 0x8010b454
  jal renderString
.org 0x8010b4a8
  jal renderString

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

.org 0x80061f14
  jal renderString
.org 0x800630a8
  jal renderString

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

.org 0x80059260
  jal renderString
.org 0x8006338c
  jal renderString
.org 0x800633c0
  jal renderString
.org 0x800633f0
  jal renderString
.org 0x800684ec
  jal renderString

.close

.open "work/DIGIMON/DGET_REL.BIN",0x80080800
.psx

.org 0x80081324
  jal renderString
.org 0x80081744
  jal renderString
.org 0x8008177c
  jal renderString
.org 0x800817b8
  jal renderString
.org 0x800817f4
  jal renderString
.org 0x80081830
  jal renderString
.org 0x800818a0
  jal renderString
.org 0x80081a34
  jal renderString

.close


.open "work/DIGIMON/KAR_REL.BIN",0x80053800
.psx

.org 0x80058d50
  jal renderString
.org 0x80058d8c
  jal renderString
.org 0x8005901c
  jal renderString

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

.org 0x8005f990
  jal renderString

.close

.open "work/DIGIMON/TRN_REL.BIN",0x80088800
.psx

.org 0x8008da38
  jal setEntityTextDigit

.org 0x8008dae4
  jal renderNumber
.org 0x8008db4c
  jal renderNumber

.org 0x8008a72c
  jal renderString
.org 0x8008aa4c
  jal renderString
.org 0x8008aaa4
  jal renderString
.org 0x8008aad8
  jal renderString
.org 0x8008dc30
  jal renderString

.close


.open "work/DIGIMON/TRN2_REL.BIN",0x80088800
.psx

.org 0x8008c410
  jal setEntityTextDigit

.org 0x8008c4bc
  jal renderNumber
.org 0x8008c524
  jal renderNumber

.org 0x8008c608
  jal renderString

.close


.open "work/DIGIMON/FISH_REL.BIN",0x80070000
.psx

.org 0x80070f6c
  jal renderItemAmount

.org 0x80070dc0
  jal renderString
.org 0x80071608
  jal renderString

.close
