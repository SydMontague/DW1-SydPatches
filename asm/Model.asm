.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.close

.open "work/DIGIMON/VS_REL.BIN",0x80052ae0
.psx
  
.org 0x80056f88
  jal loadMMD
.org 0x80057000
  jal loadMMD

.org 0x80056f7c
  jal unloadModel
.org 0x80056ff4
  jal unloadModel
.org 0x80057234
  jal unloadModel
.org 0x80057258
  jal unloadModel

.org 0x80056fac
  jal initializeDigimonObject
.org 0x80057024
  jal initializeDigimonObject

.org 0x80056f70
  jal removeEntity
.org 0x80056fe8
  jal removeEntity
.org 0x80057228
  jal removeEntity
.org 0x8005724c
  jal removeEntity

.org 0x80063830
  jal unloadModel

.org 0x800626e8
  jal setupModelMatrix
.org 0x800627f0
  jal setupModelMatrix
.org 0x80062e20
  jal setupModelMatrix
.org 0x80063064
  jal setupModelMatrix

.org 0x8005a140
  jal startAnimation
.org 0x8005a434
  jal startAnimation
.org 0x8005a520
  jal startAnimation
.org 0x8005be94
  jal startAnimation
.org 0x8005c3a0
  jal startAnimation
.org 0x8005caac
  jal startAnimation
.org 0x8005cfe8
  jal startAnimation
.org 0x8005d0b8
  jal startAnimation
.org 0x8005e098
  jal startAnimation
.org 0x8005e18c
  jal startAnimation

.org 0x80059c7c
  jal calculateBoneMatrix
.org 0x80059cfc
  jal calculateBoneMatrix
.org 0x80067810
  jal calculateBoneMatrix
.org 0x80068540
  jal calculateBoneMatrix
.org 0x8006a498
  jal calculateBoneMatrix

.close

.open "work/DIGIMON/BTL_REL.BIN",0x80052ae0
.psx

.org 0x80056cb8
  jal resetFlattenGlobal

.org 0x80065078
  jal unloadModel

.org 0x80057460
  jal startAnimation
.org 0x80057520
  jal startAnimation
.org 0x800576ec
  jal startAnimation
.org 0x8005782c
  jal startAnimation
.org 0x80058670
  jal startAnimation
.org 0x80059168
  jal startAnimation
.org 0x80059178
  jal startAnimation
.org 0x800591f8
  jal startAnimation
.org 0x80059208
  jal startAnimation
.org 0x8005927c
  jal startAnimation
.org 0x800592d4
  jal startAnimation
.org 0x800593ac
  jal startAnimation
.org 0x80059438
  jal startAnimation
.org 0x80059734
  jal startAnimation
.org 0x800599c4
  jal startAnimation
.org 0x80059f1c
  jal startAnimation
.org 0x8005a1f0
  jal startAnimation
.org 0x8005a244
  jal startAnimation
.org 0x8005a2f4
  jal startAnimation
.org 0x8005b00c
  jal startAnimation
.org 0x8005b1a4
  j startAnimation
.org 0x8005c2a4
  jal startAnimation
.org 0x8005c744
  jal startAnimation
.org 0x8005cec8
  jal startAnimation
.org 0x8005d878
  jal startAnimation
.org 0x8005d928
  jal startAnimation
.org 0x8005e93c
  jal startAnimation
.org 0x8005ea60
  jal startAnimation

.org 0x800691a8
  jal calculateBoneMatrix
.org 0x80069e64
  jal calculateBoneMatrix
.org 0x8006bdbc
  jal calculateBoneMatrix

.close

.open "work/DIGIMON/STD_REL.BIN",0x80052ae0
.psx

.org 0x80056ec4
  jal initializeDigimonObject

.org 0x800584c8
  jal setEntityPosition
.org 0x80058554
  jal setEntityPosition

.org 0x800584dc
  jal setEntityRotation
.org 0x80058568
  jal setEntityRotation


.org 0x8005e8f0
  jal resetFlattenGlobal

.org 0x8006d760
  jal unloadModel

.org 0x8006b42c
  jal setupModelMatrix
.org 0x8006b530
  jal setupModelMatrix
.org 0x8006b9d8
  jal setupModelMatrix
.org 0x8006bd40
  jal setupModelMatrix
.org 0x8006be3c
  jal setupModelMatrix
.org 0x8006c158
  jal setupModelMatrix
.org 0x8006c218
  jal setupModelMatrix
.org 0x8006c28c
  jal setupModelMatrix
.org 0x8006cae0
  jal setupModelMatrix
.org 0x8006ce0c
  jal setupModelMatrix
.org 0x8006cfd0
  jal setupModelMatrix

.org 0x800584ec
  jal startAnimation
.org 0x80058574
  jal startAnimation
.org 0x80058878
  jal startAnimation
.org 0x800595fc
  jal startAnimation
.org 0x8005aec4
  jal startAnimation
.org 0x8005b1b8
  jal startAnimation
.org 0x8005b2a4
  jal startAnimation
.org 0x8005f01c
  jal startAnimation
.org 0x80060108
  jal startAnimation
.org 0x80060784
  jal startAnimation
.org 0x8006080c
  jal startAnimation
.org 0x800610c0
  jal startAnimation
.org 0x80061394
  jal startAnimation
.org 0x80061db8
  jal startAnimation
.org 0x80061e0c
  jal startAnimation
.org 0x80061ebc
  jal startAnimation
.org 0x80062d0c
  j startAnimation
.org 0x80063500
  j startAnimation
.org 0x8006428c
  jal startAnimation
.org 0x800647a4
  jal startAnimation
.org 0x80064f58
  jal startAnimation
.org 0x8006561c
  jal startAnimation
.org 0x800656ec
  jal startAnimation
.org 0x800666b4
  jal startAnimation
.org 0x800667a8
  jal startAnimation
.org 0x8006be8c
  jal startAnimation
.org 0x8006c1bc
  jal startAnimation
.org 0x8006c3cc
  jal startAnimation


.org 0x800696f8
  j tickAnimation
.org 0x80069714
  j tickAnimation

.org 0x8005aa00
  jal calculateBoneMatrix
.org 0x8005aa80
  jal calculateBoneMatrix
.org 0x80071740
  jal calculateBoneMatrix
.org 0x80072470
  jal calculateBoneMatrix
.org 0x800743c8
  jal calculateBoneMatrix

.close

.open "work/DIGIMON/TRN2_REL.BIN",0x80088800
.psx

.org 0x800888b0
  jal startAnimation
.org 0x80088920
  jal startAnimation
.org 0x80088b34
  jal startAnimation
.org 0x80088bcc
  jal startAnimation
.org 0x80088d30
  jal startAnimation
.org 0x80088dbc
  jal startAnimation
.org 0x80088ef8
  jal startAnimation
.org 0x80088fd0
  jal startAnimation
.org 0x8008908c
  jal startAnimation
.org 0x8008928c
  jal startAnimation
.org 0x80089308
  jal startAnimation
.org 0x80089510
  jal startAnimation
.org 0x8008959c
  jal startAnimation
.org 0x8008979c
  jal startAnimation
.org 0x80089858
  jal startAnimation
.org 0x800898bc
  jal startAnimation
.org 0x8008996c
  jal startAnimation
.org 0x800899b8
  jal startAnimation
.org 0x80089bdc
  jal startAnimation
.org 0x80089c78
  jal startAnimation
.org 0x80089e40
  jal startAnimation
.org 0x80089f08
  jal startAnimation
.org 0x80089f80
  jal startAnimation
.org 0x8008a294
  jal startAnimation
.org 0x8008a394
  jal startAnimation
.org 0x8008a50c
  jal startAnimation
.org 0x8008a5a8
  jal startAnimation
.org 0x8008a758
  jal startAnimation
.org 0x8008a83c
  jal startAnimation
.org 0x8008a928
  jal startAnimation
.org 0x8008a9b8
  jal startAnimation
.org 0x8008aabc
  jal startAnimation

.close

.open "work/DIGIMON/TRN_REL.BIN",0x80088800
.psx

.org 0x8008ccdc
  jal setupEntityMatrix

.org 0x800888d8
  jal startAnimation
.org 0x8008ac38
  jal startAnimation
.org 0x8008acb4
  jal startAnimation
.org 0x8008af48
  jal startAnimation
.org 0x8008afd0
  jal startAnimation
.org 0x8008b25c
  jal startAnimation
.org 0x8008b3b4
  jal startAnimation
.org 0x8008b4d4
  jal startAnimation
.org 0x8008b534
  jal startAnimation
.org 0x8008b898
  jal startAnimation
.org 0x8008b934
  jal startAnimation
.org 0x8008bc48
  jal startAnimation
.org 0x8008bce0
  jal startAnimation
.org 0x8008bd80
  jal startAnimation
.org 0x8008bf68
  jal startAnimation
.org 0x8008c024
  jal startAnimation
.org 0x8008c254
  jal startAnimation
.org 0x8008c2d8
  jal startAnimation
.org 0x8008c430
  jal startAnimation
.org 0x8008c4f8
  jal startAnimation
.org 0x8008c7c0
  jal startAnimation
.org 0x8008c88c
  jal startAnimation
.org 0x8008c8e4
  jal startAnimation
.org 0x8008c94c
  jal startAnimation
.org 0x8008cb78
  jal startAnimation
.org 0x8008cc2c
  jal startAnimation
.org 0x8008ccd4
  jal startAnimation
.org 0x8008ccec
  jal startAnimation
.org 0x8008cd78
  jal startAnimation
.org 0x8008ce00
  jal startAnimation
.org 0x8008d034
  jal startAnimation
.org 0x8008d04c
  jal startAnimation
.org 0x8008d0c0
  jal startAnimation
.org 0x8008d0e8
  jal startAnimation
.org 0x8008d168
  jal startAnimation
.org 0x8008d178
  jal startAnimation
.org 0x8008d1dc
  jal startAnimation
.org 0x8008d1ec
  jal startAnimation
.org 0x8008d25c
  jal startAnimation
.org 0x8008d2b0
  jal startAnimation
.org 0x8008d2fc
  jal startAnimation
.org 0x8008d388
  jal startAnimation
.org 0x8008d3ac
  jal startAnimation

.close

.open "work/DIGIMON/EVL_REL.BIN",0x80060000
.psx

.org 0x80063aa0
  jal unloadModel

.org 0x80063a8c
  jal removeEntity

.org 0x80060064
  jal getEntityModelComponent
.org 0x80061a9c
  jal getEntityModelComponent
.org 0x80061dc0
  jal getEntityModelComponent
.org 0x80062478
  jal getEntityModelComponent

.org 0x80060058
  jal getEntityType
.org 0x80061a90
  jal getEntityType

.org 0x800604a0
  jal loadMMDAsync

.org 0x800602b8
  jal startAnimation
.org 0x800604f8
  jal startAnimation
.org 0x80060e30
  jal startAnimation
.org 0x80061000
  jal startAnimation

.org 0x80061e30
  jal calculateBoneMatrix
.org 0x80061e48
  jal calculateBoneMatrix
.org 0x800622ac
  jal calculateBoneMatrix
.org 0x80062df4
  jal calculateBoneMatrix
.org 0x80062e24
  jal calculateBoneMatrix

.close


.open "work/DIGIMON/DOOA_REL.BIN",0x80080000
.psx

.org 0x80081a8c
  jal renderDropShadow

.org 0x8008060c
  jal setupEntityMatrix
.org 0x800806d0
  jal setupEntityMatrix
.org 0x80080a80
  jal setupEntityMatrix
.org 0x80080b20
  jal setupEntityMatrix

.org 0x80080604
  jal setEntityPosition
.org 0x800806c8
  jal setEntityPosition
.org 0x80080a78
  jal setEntityPosition
.org 0x80080b18
  jal setEntityPosition
.org 0x8008255c
  jal setEntityPosition

.org 0x800812bc
  jal getEntityModelComponent
.org 0x80083348
  jal getEntityModelComponent
.org 0x80083c10
  jal getEntityModelComponent

.org 0x800812b0
  jal getEntityType

.org 0x800822a8
  jal loadMMDAsync

.org 0x800800b0
  jal startAnimation
.org 0x80080424
  jal startAnimation
.org 0x8008043c
  jal startAnimation
.org 0x800806f0
  jal startAnimation
.org 0x80080ac0
  jal startAnimation
.org 0x8008262c
  jal startAnimation
.org 0x80082844
  jal startAnimation
.org 0x80084794
  jal startAnimation

.org 0x80083c7c
  jal calculateBoneMatrix

.close

.open "work/DIGIMON/MURD_REL.BIN",0x8007C000
.psx

.org 0x8007cdcc
  jal renderDropShadow

.org 0x8007c684
  jal getEntityModelComponent

.org 0x8007c678
  jal getEntityType

.close


.open "work/DIGIMON/ENDI_REL.BIN",0x80060000
.psx

.org 0x800601ac
  jal getEntityModelComponent


.org 0x80060800
  jal calculateBoneMatrix
.org 0x80060858
  jal calculateBoneMatrix

.close

.open "work/DIGIMON/FISH_REL.BIN",0x80070000
.psx

.org 0x80077ed8
  jal startAnimation
.org 0x80077fb0
  jal startAnimation
.org 0x80078088
  jal startAnimation
.org 0x8007820c
  jal startAnimation
.org 0x80078314
  jal startAnimation
.org 0x80078630
  jal startAnimation
.org 0x80078824
  jal startAnimation
.org 0x80078b3c
  jal startAnimation
.org 0x80078d3c
  jal startAnimation
.org 0x8007919c
  jal startAnimation
.org 0x800799dc
  jal startAnimation

.close

.open "work/DIGIMON/KAR_REL.BIN",0x80053800
.psx

.org 0x80054504
  jal startAnimation
.org 0x80054514
  jal startAnimation
.org 0x80054538
  jal startAnimation
.org 0x8005474c
  jal startAnimation
.org 0x80054770
  jal startAnimation
.org 0x80054efc
  jal startAnimation
.org 0x80054f20
  jal startAnimation
.org 0x800550f0
  jal startAnimation
.org 0x80055124
  jal startAnimation
.org 0x80055148
  jal startAnimation
.org 0x800555c4
  jal startAnimation
.org 0x800555e8
  jal startAnimation
.org 0x80058780
  jal startAnimation
.org 0x800587b4
  jal startAnimation
.org 0x80058a90
  jal startAnimation
.org 0x80058ac4
  jal startAnimation
.org 0x80058ae8
  jal startAnimation

.close

.open "work/DIGIMON/EAB_REL.BIN",0x80060000
.psx

.org 0x800606fc
  jal startAnimation
.org 0x80060798
  jal startAnimation
.org 0x80060820
  jal startAnimation

.org 0x80061078
  jal calculateBoneMatrix

.close
