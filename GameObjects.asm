.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x800e505c
  jal tickObjects
.org 0x800e7ca4
  jal tickObjects
.org 0x800eee3c
  jal tickObjects
.org 0x800eef44
  jal tickObjects
.org 0x800ef0ec
  jal tickObjects
.org 0x800ef2b8
  jal tickObjects
.org 0x800efc94
  jal tickObjects

.org 0x800e5064
  jal renderObjects
.org 0x800e7cac
  jal renderObjects
.org 0x800eee44
  jal renderObjects
.org 0x800eef4c
  jal renderObjects
.org 0x800ef0f4
  jal renderObjects
.org 0x800ef2c0
  jal renderObjects
.org 0x800efc9c
  jal renderObjects

.org 0x800ee864
  jal initializeWorldObjects

;.org 0x800a1450
;  jal removeObject
;.org 0x800a4fbc
;  jal removeObject
;.org 0x800a51e0
;  jal removeObject
;.org 0x800aa260
;  jal removeObject
;.org 0x800acdb8
;  jal removeObject
;.org 0x800ace88
;  jal removeObject
;.org 0x800acf58
;  jal removeObject
;.org 0x800ad09c
;  jal removeObject
;.org 0x800ad0f4
;  jal removeObject
;.org 0x800adb70
;  jal removeObject
;.org 0x800ae330
;  j removeObject
;.org 0x800ae394
;  jal removeObject
;.org 0x800ae4b8
;  jal removeObject
;.org 0x800ae9ac
;  jal removeObject
;.org 0x800b54e0
;  jal removeObject
;.org 0x800b7e28
;  jal removeObject
;.org 0x800b8020
;  jal removeObject
;.org 0x800ba44c
;  jal removeObject
;.org 0x800c0524
;  jal removeObject
;.org 0x800c1118
;  jal removeObject
;.org 0x800c300c
;  j removeObject
;.org 0x800c4df8
;  jal removeObject
;.org 0x800c5618
;  jal removeObject
;.org 0x800d6794
;  jal removeObject
.org 0x800d74b8
  jal removeObject
.org 0x800d871c
  jal removeObject
.org 0x800d8f44
  jal removeObject
.org 0x800d9648
  jal removeObject
.org 0x800d96cc
  jal removeObject
.org 0x800d96d8
  jal removeObject
.org 0x800d96e4
  jal removeObject
.org 0x800d96f0
  jal removeObject
.org 0x800d96fc
  jal removeObject
.org 0x800d9708
  jal removeObject
.org 0x800db2a8
  jal removeObject
.org 0x800ddc7c
  jal removeObject
.org 0x800de12c
  jal removeObject
.org 0x800de244
  jal removeObject
.org 0x800dfdf8
  jal removeObject
.org 0x800e1bfc
  jal removeObject
.org 0x800e1c08
  jal removeObject
.org 0x800e1c14
  jal removeObject
.org 0x800e1c20
  jal removeObject
.org 0x800e1c2c
  jal removeObject
.org 0x800e1c38
  jal removeObject
.org 0x800e1c44
  jal removeObject
.org 0x800e1c50
  jal removeObject
.org 0x800e1c5c
  jal removeObject
.org 0x800e72c8
  jal removeObject
.org 0x800e8474
  j removeObject
.org 0x800e9fdc
  jal removeObject
.org 0x800ece6c
  jal removeObject
.org 0x800eefcc
  jal removeObject
.org 0x800ef198
  jal removeObject
.org 0x800ef1c8
  jal removeObject
.org 0x800f189c
  j removeObject
.org 0x800f1e64
  j removeObject
.org 0x800f1e84
  jal removeObject
.org 0x800f24f8
  jal removeObject
.org 0x800f25c8
  jal removeObject
.org 0x800f4b90
  jal removeObject
.org 0x800f4b9c
  jal removeObject
.org 0x800f50a0
  jal removeObject
.org 0x800f7da8
  jal removeObject
.org 0x800f7db4
  jal removeObject
.org 0x800ff8e4
  jal removeObject
.org 0x80104e08
  jal removeObject
.org 0x80111b64
  jal removeObject
.org 0x80111b70
  jal removeObject

;.org 0x800a1170
;  jal addObject
;.org 0x800a4714
;  jal addObject
;.org 0x800acde8
;  jal addObject
;.org 0x800acebc
;  jal addObject
;.org 0x800ad0c8
;  jal addObject
;.org 0x800ad128
;  jal addObject
;.org 0x800ad898
;  jal addObject
;.org 0x800adf98
;  jal addObject
;.org 0x800ae450
;  jal addObject
;.org 0x800ae834
;  jal addObject
;.org 0x800b5300
;  jal addObject
;.org 0x800b7cd8
;  jal addObject
;.org 0x800c171c
;  jal addObject
;.org 0x800c28f8
;  jal addObject
;.org 0x800c30e8
;  jal addObject
;.org 0x800c4c60
;  jal addObject
;.org 0x800c4db0
;  jal addObject
;.org 0x800d57cc
;  jal addObject
.org 0x800d7180
  jal addObject
.org 0x800d8080
  jal addObject
.org 0x800d888c
  jal addObject
.org 0x800d9008
  jal addObject
.org 0x800d9308
  jal addObject
.org 0x800d9320
  jal addObject
.org 0x800d9338
  jal addObject
.org 0x800d9438
  j addObject
.org 0x800dae40
  jal addObject
.org 0x800dcf54
  jal addObject
.org 0x800dcf6c
  jal addObject
.org 0x800ddc54
  jal addObject
.org 0x800de08c
  jal addObject
.org 0x800dfa08
  jal addObject
.org 0x800e0280
  jal addObject
.org 0x800e0564
  jal addObject
.org 0x800e07f8
  jal addObject
.org 0x800e0950
  jal addObject
.org 0x800e0be0
  jal addObject
.org 0x800e105c
  jal addObject
.org 0x800e1368
  jal addObject
.org 0x800e15a0
  jal addObject
.org 0x800e1698
  jal addObject
.org 0x800e1808
  jal addObject
.org 0x800e36ec
  jal addObject
.org 0x800e745c
  jal addObject
.org 0x800e7de4
  j addObject
.org 0x800e9dac
  jal addObject
.org 0x800ec628
  jal addObject
.org 0x800eed50
  jal addObject
.org 0x800ef024
  jal addObject
.org 0x800ef03c
  jal addObject
.org 0x800f0478
  jal addObject
.org 0x800f1890
  j addObject
.org 0x800f1d9c
  jal addObject
.org 0x800f1f08
  jal addObject
.org 0x800f4cf0
  jal addObject
.org 0x800f6db0
  jal addObject
.org 0x800f6dc8
  jal addObject
.org 0x800f7d0c
  jal addObject
.org 0x800f7d28
  jal addObject
.org 0x800ff8d4
  jal addObject
.org 0x80111bb4
  jal addObject
.org 0x80111bd0
  jal addObject

.close

.open "work/DIGIMON/BTL_REL.BIN",0x80052ae0
.psx

.org 0x8005df2c
  jal tickObjects

.org 0x8005df34
  jal renderObjects

.org 0x8005b24c
  j addObject
.org 0x80061200
  jal addObject
.org 0x800614e4
  j addObject
.org 0x800620e0
  jal addObject
.org 0x80062564
  jal addObject
.org 0x80063380
  jal addObject
.org 0x80063a40
  j addObject
.org 0x80064588
  j addObject
.org 0x80066130
  jal addObject
.org 0x8006ee08
  jal addObject
.org 0x8006f14c
  jal addObject
.org 0x8006f4bc
  jal addObject
.org 0x8006fb2c
  jal addObject
.org 0x8006fec4
  jal addObject
.org 0x80070be0
  jal addObject
.org 0x800712ac
  jal addObject
.org 0x8007181c
  jal addObject
.org 0x80071d74
  j addObject
.org 0x8007235c
  jal addObject
.org 0x80072ce4
  jal addObject


.org 0x80059070
  j removeObject
.org 0x80061234
  jal removeObject
.org 0x80061d88
  jal removeObject
.org 0x8006237c
  jal removeObject
.org 0x80062890
  jal removeObject
.org 0x80063a24
  j removeObject
.org 0x80063ee8
  j removeObject
.org 0x80064a78
  j removeObject
.org 0x80066180
  jal removeObject
.org 0x8006ee78
  jal removeObject
.org 0x8006f1b0
  jal removeObject
.org 0x8006f1dc
  jal removeObject
.org 0x8006f1fc
  jal removeObject
.org 0x8006f524
  jal removeObject
.org 0x8006fa74
  jal removeObject
.org 0x8006fb90
  jal removeObject
.org 0x8006ff30
  jal removeObject
.org 0x8006ff84
  jal removeObject
.org 0x800700ac
  jal removeObject
.org 0x80070c10
  jal removeObject
.org 0x80070cdc
  jal removeObject
.org 0x80070df4
  jal removeObject
.org 0x8007131c
  jal removeObject
.org 0x800714d0
  jal removeObject
.org 0x80071874
  jal removeObject
.org 0x80071d80
  j removeObject
.org 0x800723c8
  jal removeObject
.org 0x8007246c
  jal removeObject
.org 0x80072d34
  jal removeObject

.close


.open "work/DIGIMON/DOO2_REL.BIN",0x80070000
.psx

.org 0x80071570
  jal addObject

.org 0x80070264
  j removeObject

.close


.open "work/DIGIMON/DOOA_REL.BIN",0x80080000
.psx

.org 0x80081278
  jal addObject
.org 0x800846c4
  jal addObject
.org 0x80084880
  jal addObject


.org 0x80080f6c
  jal removeObject
.org 0x80082874
  jal removeObject
.org 0x80082a38
  j removeObject

.close


.open "work/DIGIMON/ENDI_REL.BIN",0x80060000
.psx

.org 0x80060548
  jal addObject
.org 0x80060d6c
  jal addObject


.org 0x800604e8
  jal removeObject
.org 0x80060a18
  j removeObject

.close


.open "work/DIGIMON/EVL_REL.BIN",0x80060000
.psx

.org 0x80061a34
  jal addObject
.org 0x80061f14
  jal addObject
.org 0x80062258
  jal addObject
.org 0x80063330
  jal addObject


.org 0x80061058
  jal removeObject
.org 0x800623a4
  jal removeObject
.org 0x80062414
  jal removeObject
.org 0x80062b0c
  jal removeObject
.org 0x80063070
  jal removeObject

.close

.open "work/DIGIMON/EAB_REL.BIN",0x80060000
.psx

.org 0x80060200
  jal addObject
.org 0x80060450
  jal addObject
.org 0x80060ee0
  jal addObject
.org 0x80061500
  jal addObject
.org 0x8006159c
  jal addObject


.org 0x80060260
  jal removeObject
.org 0x800604b0
  jal removeObject
.org 0x80060664
  jal removeObject
.org 0x800607e0
  jal removeObject
.org 0x80060c08
  jal removeObject
.org 0x8006115c
  jal removeObject
.org 0x800611c8
  jal removeObject

.close


.open "work/DIGIMON/FISH_REL.BIN",0x80070000
.psx

.org 0x80079bb8
  jal addObject

.org 0x80079be4
  jal removeObject

.close


.open "work/DIGIMON/KAR_REL.BIN",0x80053800
.psx

.org 0x80053c9c
  jal addObject

.org 0x8005454c
  jal removeObject

.close


.open "work/DIGIMON/MURD_REL.BIN",0x8007C000
.psx

.org 0x8007d668
  jal addObject


.org 0x8007c500
  jal removeObject
.org 0x8007c568
  jal removeObject

.close


.open "work/DIGIMON/STD_REL.BIN",0x80052ae0
.psx

.org 0x8006507c
  jal tickObjects

.org 0x80065084
  jal renderObjects

.org 0x80057ff8
  jal addObject
.org 0x80058018
  jal addObject
.org 0x800587d4
  jal addObject
.org 0x80058940
  jal addObject
.org 0x8005a42c
  jal addObject
.org 0x8005b2d0
  jal addObject
.org 0x8005b704
  jal addObject
.org 0x8005c0b0
  jal addObject
.org 0x8005d530
  j addObject
.org 0x8005d548
  j addObject
.org 0x8005df30
  jal addObject
.org 0x8005e648
  jal addObject
.org 0x80060c48
  jal addObject
.org 0x80062d2c
  jal addObject
.org 0x80062d44
  jal addObject
.org 0x800687f8
  j addObject
.org 0x800691b8
  j addObject
.org 0x800699a4
  jal addObject
.org 0x8006a064
  j addObject
.org 0x8006ad60
  j addObject
.org 0x8006b47c
  j addObject
.org 0x8006bfcc
  j addObject
.org 0x8006c674
  j addObject
.org 0x8006c7b8
  jal addObject
.org 0x8006ce44
  jal addObject
.org 0x8006e818
  jal addObject
.org 0x800773a8
  jal addObject
.org 0x800776ec
  jal addObject
.org 0x80077a5c
  jal addObject
.org 0x800780cc
  jal addObject
.org 0x80078464
  jal addObject
.org 0x80079180
  jal addObject
.org 0x8007984c
  jal addObject


.org 0x80057784
  jal removeObject
.org 0x80057790
  jal removeObject
.org 0x8005779c
  jal removeObject
.org 0x8005848c
  j removeObject
.org 0x8005889c
  j removeObject
.org 0x800588bc
  jal removeObject
.org 0x80058f64
  jal removeObject
.org 0x80059040
  jal removeObject
.org 0x8005a554
  j removeObject
.org 0x8005bc9c
  jal removeObject
.org 0x8005c1c8
  jal removeObject
.org 0x8005d7ac
  j removeObject
.org 0x8005d7b8
  j removeObject
.org 0x8005dfc4
  jal removeObject
.org 0x8005e89c
  j removeObject
.org 0x80060aac
  jal removeObject
.org 0x80060ab8
  jal removeObject
.org 0x80061010
  jal removeObject
.org 0x8006912c
  j removeObject
.org 0x80069444
  jal removeObject
.org 0x8006a048
  j removeObject
.org 0x8006a50c
  j removeObject
.org 0x8006b1e8
  j removeObject
.org 0x8006b6ec
  j removeObject
.org 0x8006c63c
  jal removeObject
.org 0x8006c6d4
  j removeObject
.org 0x8006ccd8
  j removeObject
.org 0x8006d160
  j removeObject
.org 0x8006e868
  jal removeObject
.org 0x80077418
  jal removeObject
.org 0x80077750
  jal removeObject
.org 0x8007777c
  jal removeObject
.org 0x8007779c
  jal removeObject
.org 0x80077ac4
  jal removeObject
.org 0x80078014
  jal removeObject
.org 0x80078130
  jal removeObject
.org 0x800784d0
  jal removeObject
.org 0x80078524
  jal removeObject
.org 0x8007864c
  jal removeObject
.org 0x800791b0
  jal removeObject
.org 0x8007927c
  jal removeObject
.org 0x80079394
  jal removeObject
.org 0x800798bc
  jal removeObject

.close

.open "work/DIGIMON/TRN_REL.BIN",0x80088800
.psx

.org 0x8008ab40
  jal addObject
.org 0x8008b160
  jal addObject
.org 0x8008b71c
  jal addObject
.org 0x8008b7ec
  jal addObject
.org 0x8008c13c
  jal addObject
.org 0x8008caa8
  jal addObject
.org 0x8008cae8
  jal addObject
.org 0x8008cefc
  jal addObject
.org 0x8008cfb4
  jal addObject
.org 0x8008ed90
  jal addObject


.org 0x8008b0b0
  jal removeObject
.org 0x8008b5c8
  jal removeObject
.org 0x8008bdf8
  jal removeObject
.org 0x8008c9e0
  jal removeObject
.org 0x8008ce70
  jal removeObject
.org 0x8008d4ac
  jal removeObject
.org 0x8008df7c
  jal removeObject

.close

.open "work/DIGIMON/TRN2_REL.BIN",0x80088800
.psx

.org 0x80089190
  jal addObject
.org 0x800891e8
  jal addObject
.org 0x80089714
  jal addObject
.org 0x80089b30
  jal addObject
.org 0x8008a154
  jal addObject
.org 0x8008a208
  jal addObject
.org 0x8008a6c4
  jal addObject
.org 0x8008d768
  jal addObject


.org 0x80088ca0
  jal removeObject
.org 0x80089114
  jal removeObject
.org 0x80089670
  jal removeObject
.org 0x80089a4c
  jal removeObject
.org 0x80089fec
  jal removeObject
.org 0x8008a630
  jal removeObject
.org 0x8008aa24
  jal removeObject
.org 0x8008c954
  jal removeObject

.close


.open "work/DIGIMON/VS_REL.BIN",0x80052ae0
.psx

.org 0x8005d74c
  jal tickObjects

.org 0x8005d754
  jal renderObjects

.org 0x80056e9c
  jal addObject
.org 0x80056eb4
  jal addObject
.org 0x80056ed0
  jal addObject
.org 0x800584f8
  jal addObject
.org 0x80058a54
  jal addObject
.org 0x80058a74
  jal addObject
.org 0x80058a94
  jal addObject
.org 0x800597ec
  jal addObject
.org 0x8005a54c
  jal addObject
.org 0x8005a984
  jal addObject
.org 0x8005b330
  jal addObject
.org 0x8005fc9c
  j addObject
.org 0x800606ac
  j addObject
.org 0x80060c7c
  jal addObject
.org 0x80061380
  j addObject
.org 0x800620ec
  j addObject
.org 0x8006273c
  j addObject
.org 0x80062e50
  jal addObject
.org 0x800648e8
  jal addObject
.org 0x8006d478
  jal addObject
.org 0x8006d7bc
  jal addObject
.org 0x8006db2c
  jal addObject
.org 0x8006e19c
  jal addObject
.org 0x8006e534
  jal addObject
.org 0x8006f250
  jal addObject
.org 0x8006f91c
  jal addObject

.org 0x800571f8
  jal removeObject
.org 0x80057204
  jal removeObject
.org 0x80057210
  jal removeObject
.org 0x8005852c
  jal removeObject
.org 0x80059044
  j removeObject
.org 0x800598dc
  j removeObject
.org 0x8005af1c
  jal removeObject
.org 0x8005b448
  jal removeObject
.org 0x800605d0
  j removeObject
.org 0x800609cc
  jal removeObject
.org 0x80061364
  j removeObject
.org 0x8006186c
  j removeObject
.org 0x80062574
  j removeObject
.org 0x80062988
  j removeObject
.org 0x80063228
  j removeObject
.org 0x80064938
  jal removeObject
.org 0x8006d4e8
  jal removeObject
.org 0x8006d820
  jal removeObject
.org 0x8006d84c
  jal removeObject
.org 0x8006d86c
  jal removeObject
.org 0x8006db94
  jal removeObject
.org 0x8006e0e4
  jal removeObject
.org 0x8006e200
  jal removeObject
.org 0x8006e5a0
  jal removeObject
.org 0x8006e5f4
  jal removeObject
.org 0x8006e71c
  jal removeObject
.org 0x8006f280
  jal removeObject
.org 0x8006f34c
  jal removeObject
.org 0x8006f464
  jal removeObject
.org 0x8006f98c
  jal removeObject

.close