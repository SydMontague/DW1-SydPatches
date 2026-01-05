.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x800ee90c
  jal initializeMusic
.org 0x800ee940
  jal initializeMusic
.org 0x800ee990
  jal initializeMusic
.org 0x800eeae8
  jal initializeMusic

.org 0x800ee91c
  jal finalizeMusic
.org 0x800ee97c
  jal finalizeMusic
.org 0x800eead4
  jal finalizeMusic
.org 0x800eebc0
  jal finalizeMusic

;.org 0x800e6bf4
;  jal stopBGM
;.org 0x800e72d0
;  jal stopBGM
.org 0x800f2438
  jal stopBGM
.org 0x800f2524
  jal stopBGM
.org 0x800f25d0
  jal stopBGM
.org 0x800f4754
  jal stopBGM
.org 0x800f4824
  jal stopBGM
.org 0x800f4924
  jal stopBGM
.org 0x800f4cf8
  jal stopBGM
.org 0x800f7198
  jal stopBGM
.org 0x800f7260
  jal stopBGM
.org 0x8010460c
  jal stopBGM
.org 0x801064f8
  j stopBGM
.org 0x80106ac8
  jal stopBGM
.org 0x80106b68
  jal stopBGM
.org 0x80106be0
  jal stopBGM
.org 0x801078dc
  jal stopBGM
.org 0x80109254
  jal stopBGM

.org 0x800e7524
  jal playMusic
.org 0x800e7560
  jal playMusic
.org 0x800e75b4
  jal playMusic
.org 0x800e75c8
  jal playMusic
.org 0x800e75ec
  jal playMusic
.org 0x800e75fc
  jal playMusic
.org 0x800f244c
  jal playMusic
.org 0x800f2538
  jal playMusic
.org 0x800f4838
  jal playMusic
.org 0x800f71a4
  jal playMusic
.org 0x80106ad8
  jal playMusic
.org 0x80106b78
  jal playMusic
.org 0x80106bf0
  jal playMusic
.org 0x8010927c
  jal playMusic

;.org 0x800d5db0
;  jal loadMapSounds
;.org 0x800deb98
;  jal loadMapSounds
;.org 0x800dec68
;  jal loadMapSounds

;.org 0x800e7400
;  jal stopSound
.org 0x800f2440
  jal stopSound
.org 0x800f252c
  jal stopSound
.org 0x800f25d8
  jal stopSound
.org 0x800f475c
  jal stopSound
.org 0x800f482c
  jal stopSound
.org 0x800f492c
  jal stopSound

;.org 0x800daf8c
;  jal playSound
;.org 0x800db524
;  jal playSound
;.org 0x800db56c
;  jal playSound
;.org 0x800db5b8
;  jal playSound
;.org 0x800db604
;  jal playSound
;.org 0x800dc418
;  jal playSound
;.org 0x800dc454
;  jal playSound
;.org 0x800dc998
;  jal playSound
;.org 0x800dcd98
;  jal playSound
;.org 0x800dcdd8
;  jal playSound
;.org 0x800ddad4
;  jal playSound
;.org 0x800dddf4
;  jal playSound
;.org 0x800de064
;  jal playSound
;.org 0x800e6450
;  jal playSound
;.org 0x800e64dc
;  jal playSound
.org 0x800edb6c
  jal playSound
.org 0x800edbcc
  jal playSound
.org 0x800ee298
  jal playSound
.org 0x800eedac
  jal playSound
.org 0x800f0bdc
  jal playSound
.org 0x800f0c90
  jal playSound
.org 0x800f0d28
  jal playSound
.org 0x800f0fdc
  jal playSound
.org 0x800f28c4
  jal playSound
.org 0x800f36a0
  jal playSound
.org 0x800f3704
  jal playSound
.org 0x800f77e8
  jal playSound
.org 0x800f7864
  jal playSound
.org 0x800f78d8
  jal playSound
.org 0x800f7a10
  jal playSound
.org 0x800f7e14
  jal playSound
.org 0x800f7e5c
  jal playSound
.org 0x800f84a0
  jal playSound
.org 0x800f84f4
  jal playSound
.org 0x800f868c
  jal playSound
.org 0x800f8720
  jal playSound
.org 0x800fa0b8
  jal playSound
.org 0x800fa154
  jal playSound
.org 0x800fa194
  jal playSound
.org 0x800fa1dc
  jal playSound
.org 0x800fa218
  jal playSound
.org 0x800facbc
  jal playSound
.org 0x800fadc8
  jal playSound
.org 0x800fb040
  jal playSound
.org 0x800fb054
  jal playSound
.org 0x800fb0b4
  jal playSound
.org 0x800fb1fc
  jal playSound
.org 0x800fb294
  jal playSound
.org 0x800fb2c8
  jal playSound
.org 0x800fb2f4
  jal playSound
.org 0x800fb91c
  jal playSound
.org 0x800fb944
  jal playSound
.org 0x800fb970
  jal playSound
.org 0x800fba78
  jal playSound
.org 0x800fba88
  jal playSound
.org 0x800fd210
  jal playSound
.org 0x800fd224
  jal playSound
.org 0x800fd3a8
  jal playSound
.org 0x800fd3bc
  jal playSound
.org 0x800fd4bc
  jal playSound
.org 0x800fd4d0
  jal playSound
.org 0x800fd5e8
  jal playSound
.org 0x800fd604
  jal playSound
.org 0x800fe000
  jal playSound
.org 0x800ff308
  j playSound
.org 0x800ff330
  j playSound
.org 0x800ffc70
  jal playSound
.org 0x800ffcd0
  jal playSound
.org 0x800ffd00
  jal playSound
.org 0x800ffd18
  jal playSound
.org 0x800ffd78
  jal playSound
.org 0x800ffdd8
  jal playSound
.org 0x800ffeac
  jal playSound
.org 0x800ffebc
  jal playSound
.org 0x80100058
  jal playSound
.org 0x801000cc
  jal playSound
.org 0x80103db8
  jal playSound
.org 0x80107fe0
  jal playSound
.org 0x80108304
  jal playSound
.org 0x80108318
  jal playSound
.org 0x80108408
  jal playSound
.org 0x80108430
  jal playSound
.org 0x8010845c
  jal playSound
.org 0x80108498
  jal playSound
.org 0x801084c4
  jal playSound
.org 0x801084f0
  jal playSound
.org 0x801085ec
  jal playSound
.org 0x80109304
  jal playSound
.org 0x8010932c
  jal playSound
.org 0x801095f4
  jal playSound
.org 0x80109604
  jal playSound
.org 0x80109630
  jal playSound
.org 0x801098ec
  jal playSound
.org 0x801098fc
  jal playSound
.org 0x80109928
  jal playSound
.org 0x8010a0e4
  jal playSound
;.org 0x8010a1d8
;  jal playSound
.org 0x8010a27c
  jal playSound
.org 0x8010a2c8
  jal playSound
.org 0x8010a2e4
  jal playSound
.org 0x8010a314
  jal playSound
.org 0x8010a360
  jal playSound
.org 0x8010ab98
  jal playSound
.org 0x8010ad1c
  j playSound
.org 0x8010adec
  j playSound
.org 0x8010aea4
  j playSound
.org 0x8010af58
  j playSound
.org 0x801103d8
  jal playSound
.org 0x801122c0
  jal playSound
.org 0x80112374
  jal playSound
.org 0x8011240c
  jal playSound
.org 0x80112440
  jal playSound
.org 0x80112480
  jal playSound
.org 0x801124b8
  jal playSound
.org 0x801124e0
  jal playSound
.org 0x80112500
  jal playSound
.org 0x801129dc
  jal playSound
.org 0x80112a10
  jal playSound
.org 0x80112a70
  jal playSound
.org 0x80112aa4
  jal playSound
.org 0x80112aec
  jal playSound
.org 0x80112b0c
  jal playSound

.close


.open "work/DIGIMON/FISH_REL.BIN",0x80070000
.psx

.org 0x80070868
  jal playSound
.org 0x800708f0
  jal playSound
.org 0x800709a8
  jal playSound
.org 0x80070a0c
  jal playSound
.org 0x80070a50
  jal playSound
.org 0x80070aac
  jal playSound
.org 0x80071504
  jal playSound
.org 0x80073cbc
  jal playSound
.org 0x80073d58
  jal playSound
.org 0x80073df4
  jal playSound
.org 0x800764d4
  jal playSound
.org 0x80076a24
  jal playSound
.org 0x80076f00
  jal playSound
.org 0x80078404
  jal playSound
.org 0x80078574
  jal playSound
.org 0x80078f08
  jal playSound
.org 0x80079100
  jal playSound
.org 0x80079250
  jal playSound
.org 0x80079294
  jal playSound
.org 0x800792d0
  jal playSound
.org 0x80079318
  jal playSound
.org 0x80079358
  jal playSound
.org 0x800793b4
  jal playSound

.close


.open "work/DIGIMON/DOO2_REL.BIN",0x80070000
.psx

.org 0x80071910
  jal stopSound
.org 0x8007191c
  jal playSound
.org 0x800719e4
  jal playSound
.org 0x80071a30
  jal playSound
.org 0x80071a64
  jal playSound

.close


.open "work/DIGIMON/DGET_REL.BIN",0x80080800
.psx

.org 0x8008139c
  jal playSound
.org 0x80081418
  jal playSound
.org 0x8008142c
  jal playSound
.org 0x80081474
  jal playSound
.org 0x800814b8
  jal playSound
.org 0x80081500
  jal playSound
.org 0x80081544
  jal playSound
.org 0x800815a4
  jal playSound
.org 0x800815b4
  jal playSound
.org 0x8008198c
  jal playSound

.close


.open "work/DIGIMON/DOOA_REL.BIN",0x80080000
.psx

.org 0x8008072c
  jal stopSound
.org 0x8008084c
  jal stopSound
.org 0x80080f7c
  jal stopSound
.org 0x8008219c
  jal stopSound
.org 0x80080858
  jal playSound
.org 0x80080b2c
  jal playSound
.org 0x800821a8
  jal playSound
.org 0x800822c0
  jal playSound
.org 0x80082804
  jal playSound
.org 0x80080204
  jal stopBGM
.org 0x80080714
  jal playSound2
.org 0x80080ce8
  jal playSound2

.close


.open "work/DIGIMON/ENDI_REL.BIN",0x80060000
.psx

.org 0x800604d4
  jal stopSound
.org 0x80060460
  jal playSound
.org 0x80060d80
  jal playSound

.close

.open "work/DIGIMON/EVL_REL.BIN",0x80060000
.psx

.org 0x800604dc
  jal playSound2
.org 0x800604e8
  jal playSound2
.org 0x800606ac
  jal playSound2
.org 0x80060be4
  jal stopSound
.org 0x80060bf0
  jal playSound

.close

.open "work/DIGIMON/EAB_REL.BIN",0x80060000
.psx

.org 0x800607f4
  jal playSound2
.org 0x80061518
  jal playSound2
.org 0x800607e8
  jal stopSound
.org 0x80060ba8
  jal stopSound
.org 0x80060bb4
  jal playSound
.org 0x800616bc
  jal playSound

.close

.open "work/DIGIMON/MURD_REL.BIN",0x8007C000
.psx

.org 0x8007c160
  jal stopBGM

.close

.open "work/DIGIMON/TRN_REL.BIN",0x80088800
.psx

.org 0x8008b3c0
  jal playSound2
.org 0x8008c3c8
  jal playSound2
.org 0x8008ac54
  jal playSound
.org 0x8008ace8
  jal playSound
.org 0x8008ade4
  jal playSound
.org 0x8008af08
  jal playSound
.org 0x8008b278
  jal playSound
.org 0x8008b4e0
  jal playSound
.org 0x8008b8b4
  jal playSound
.org 0x8008ba74
  jal playSound
.org 0x8008bd08
  jal playSound
.org 0x8008beac
  jal playSound
.org 0x8008c044
  jal playSound
.org 0x8008c270
  jal playSound
.org 0x8008c898
  jal playSound
.org 0x8008cb94
  jal playSound
.org 0x8008cd84
  jal playSound
.org 0x8008d058
  jal playSound
.org 0x8008d24c
  jal playSound
.org 0x8008d2d4
  jal playSound
.org 0x8008d3c0
  jal playSound
.org 0x8008d980
  jal playSound
.org 0x8008e118
  jal playSound
.org 0x8008e20c
  jal playSound
.org 0x8008e358
  jal playSound
.org 0x8008e42c
  jal playSound
.org 0x8008e454
  jal playSound
.org 0x8008e468
  jal playSound
.org 0x8008e480
  jal playSound
.org 0x8008b4ec
  jal stopSoundMask
.org 0x8008c8a4
  jal stopSoundMask

.close

.open "work/DIGIMON/TRN2_REL.BIN",0x80088800
.psx

.org 0x8008a940
  jal stopSoundMask
.org 0x8008a7d0
  jal playSound2
.org 0x80088a54
  jal playSound
.org 0x80088af4
  jal playSound
.org 0x80088d4c
  jal playSound
.org 0x80088e88
  jal playSound
.org 0x80088ff0
  jal playSound
.org 0x800892a8
  jal playSound
.org 0x8008933c
  jal playSound
.org 0x80089438
  jal playSound
.org 0x800894d8
  jal playSound
.org 0x800897b8
  jal playSound
.org 0x80089934
  jal playSound
.org 0x80089bf8
  jal playSound
.org 0x80089d84
  jal playSound
.org 0x80089ec4
  jal playSound
.org 0x8008a3f4
  jal playSound
.org 0x8008a52c
  jal playSound
.org 0x8008a774
  jal playSound
.org 0x8008a934
  jal playSound
.org 0x8008c358
  jal playSound
.org 0x8008caf0
  jal playSound
.org 0x8008cbe4
  jal playSound
.org 0x8008cd30
  jal playSound
.org 0x8008ce04
  jal playSound
.org 0x8008ce2c
  jal playSound
.org 0x8008ce40
  jal playSound
.org 0x8008ce58
  jal playSound

.close

.open "work/DIGIMON/KAR_REL.BIN",0x80053800
.psx

.org 0x8005538c
  jal stopSoundMask
.org 0x80058cc8
  jal playSound
.org 0x80058d18
  jal playSound
.org 0x80058f88
  jal playSound
.org 0x80053fe0
  jal playSound2
.org 0x800551a4
  jal playSound2
.org 0x80055940
  jal playSound2
.org 0x80055958
  jal playSound2
.org 0x8005707c
  jal playSound2
.org 0x80057214
  jal playSound2
.org 0x80057300
  jal playSound2
.org 0x80057310
  jal playSound2
.org 0x80059628
  jal playSound2
.org 0x80059638
  jal playSound2

.close 


.open "work/DIGIMON/BTL_REL.BIN",0x80052ae0
.psx

.org 0x80064c9c
  jal stopSoundMask
.org 0x80064e78
  jal stopSoundMask
.org 0x80058f70
  jal stopBGM
.org 0x80059180
  jal stopBGM
.org 0x8006bf48
  jal playSound2
.org 0x80058f78
  jal stopSound
.org 0x80059188
  jal stopSound
.org 0x8005752c
  jal playSound
.org 0x800578c0
  jal playSound
.org 0x80059194
  jal playSound
.org 0x800591e8
  jal playSound
.org 0x8006bf18
  jal playSound

.close

.open "work/DIGIMON/VS_REL.BIN",0x80052ae0
.psx

.org 0x80063454
  jal stopSoundMask
.org 0x80063630
  jal stopSoundMask
.org 0x8006a5f4
  jal playSound
.org 0x8006a624
  jal playSound2
.org 0x800581f0
  jal loadDigimonSounds
.org 0x80058208
  jal loadDigimonSounds
.org 0x800581b8
  jal VS_loadSounds

.close

.open "work/DIGIMON/STD_REL.BIN",0x80052ae0
.psx

.org 0x8006d384
  jal stopSoundMask
.org 0x8006d560
  jal stopSoundMask
.org 0x80074554
  jal playSound2
.org 0x8005934c
  jal playSound
.org 0x8005c3e0
  jal playSound
.org 0x8005c52c
  jal playSound
.org 0x8005c7b0
  jal playSound
.org 0x8005c898
  jal playSound
.org 0x8005f1a8
  jal playSound
.org 0x8005f20c
  jal playSound
.org 0x8006948c
  jal playSound
.org 0x800694f4
  jal playSound
.org 0x8006955c
  jal playSound
.org 0x80069658
  jal playSound
.org 0x80074524
  jal playSound
.org 0x8005763c
  jal stopSound
.org 0x80058ed8
  jal stopSound
.org 0x80058f74
  jal stopSound
.org 0x80059050
  jal stopSound
.org 0x8005df40
  jal stopSound
.org 0x8005dfe0
  jal stopSound
.org 0x8005e620
  jal stopSound
.org 0x80060794
  jal stopSound
.org 0x80060878
  jal stopSound
.org 0x8006c64c
  jal stopSound
.org 0x80057a78
  jal loadMapSounds
.org 0x80057b10
  jal loadMapSounds
.org 0x80057b9c
  jal loadMapSounds
.org 0x80058ee4
  jal playMusic
.org 0x80058fc8
  jal playMusic
.org 0x8005df4c
  jal playMusic
.org 0x8005e62c
  jal playMusic
.org 0x800607a8
  jal playMusic
.org 0x800607bc
  jal playMusic
.org 0x80063174
  jal playMusic
.org 0x80057634
  jal stopBGM
.org 0x80058ed0
  jal stopBGM
.org 0x80058f6c
  jal stopBGM
.org 0x80059048
  jal stopBGM
.org 0x8005df38
  jal stopBGM
.org 0x8005dfd8
  jal stopBGM
.org 0x8005e618
  jal stopBGM
.org 0x8006078c
  jal stopBGM
.org 0x80060870
  jal stopBGM
.org 0x80060c50
  jal stopBGM
.org 0x80063168
  jal stopBGM
.org 0x8006c644
  jal stopBGM

.close