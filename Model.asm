.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

;.org 0x800a446c
;  jal loadMMD
;.org 0x800a983c
;  jal loadMMD
.org 0x800f1534
  jal loadMMD
;.org 0x800b6110
;  j loadMMD


;.org 0x800b63fc
;  j unloadModel
;.org 0x800b6634
;  jal unloadModel
;.org 0x800d9764
;  jal unloadModel
;.org 0x800d9770
;  jal unloadModel
;.org 0x800d97a0
;  jal unloadModel
.org 0x800e3624
  jal unloadModel
.org 0x800f172c
  jal unloadModel


;.org 0x800a4490
;  jal initializeDigimonObject
;.org 0x800a4ff4
;  jal initializeDigimonObject
;.org 0x800a5218
;  jal initializeDigimonObject
;.org 0x800a9860
;  jal initializeDigimonObject
;.org 0x800b61f0
;  jal initializeDigimonObject
.org 0x800f1558
  jal initializeDigimonObject


;.org 0x800b61a8		
;  jal removeEntity
;.org 0x800b63e4		
;  jal removeEntity
;.org 0x800b65e0		
;  jal removeEntity
;.org 0x800d9744		
;  jal removeEntity
.org 0x800e3610		
  jal removeEntity
.org 0x800f1720		
  jal removeEntity


;.org 0x800a9890
;  jal setupEntityMatrix
;.org 0x800a9c84
;  jal setupEntityMatrix
;.org 0x800ac700
;  jal setupEntityMatrix
;.org 0x800ac96c
;  jal setupEntityMatrix
;.org 0x800b6240
;  jal setupEntityMatrix
;.org 0x800bf078
;  jal setupEntityMatrix
;.org 0x800bf1c8
;  jal setupEntityMatrix
.org 0x800f14e4
  jal setupEntityMatrix
.org 0x800f15a8
  jal setupEntityMatrix


;.org 0x800a9874
;  jal setEntityPosition
;.org 0x800a9c60
;  jal setEntityPosition
;.org 0x800ac6f4
;  jal setEntityPosition
;.org 0x800ac960
;  jal setEntityPosition
;.org 0x800b6214
;  jal setEntityPosition
;.org 0x800bf05c
;  jal setEntityPosition
;.org 0x800bf1ac
;  jal setEntityPosition
.org 0x800f158c
  jal setEntityPosition
.org 0x800f1af8
  jal setEntityPosition
.org 0x800f1b68
  jal setEntityPosition


;.org 0x800a9888
;  jal setEntityRotation
;.org 0x800a9c7c
;  jal setEntityRotation
;.org 0x800b6238
;  jal setEntityRotation
;.org 0x800bf070
;  jal setEntityRotation
;.org 0x800bf1c0
;  jal setEntityRotation
.org 0x800f14dc
  jal setEntityRotation
.org 0x800f15a0
  jal setEntityRotation
.org 0x800f1b0c
  jal setEntityRotation
.org 0x800f1b7c
  jal setEntityRotation

.org 0x800f03d4
  jal resetFlattenGlobal
.org 0x800f2e94
  jal resetFlattenGlobal

.org 0x800ee85c
  jal initializeModelComponents
  
;.org 0x800c1a8c
;  jal getEntityModelComponent

;.org 0x800c1a80
;  jal getEntityType

;.org 0x800c2248
;  jal calculatePosMatrix
;.org 0x800c2284
;  jal calculatePosMatrix

;.org 0x800c1b40
;  jal resetMomentumData
;.org 0x800c1c10
;  jal resetMomentumData

;.org 0x800c1f94
;  jal animateEntityTexture

;.org 0x800c1b4c
;  jal setupModelMatrix
;.org 0x800c1c18
;  jal setupModelMatrix
;.org 0x800c2264
;  jal setupModelMatrix

;.org 0x800a1150
;  jal startAnimation
;.org 0x800a44d0
;  jal startAnimation
;.org 0x800a5034
;  jal startAnimation
;.org 0x800a5258
;  jal startAnimation
;.org 0x800a5660
;  jal startAnimation
;.org 0x800a7ce8
;  jal startAnimation
;.org 0x800a98b8
;  jal startAnimation
;.org 0x800a9c94
;  jal startAnimation
;.org 0x800a9e78
;  jal startAnimation
;.org 0x800a9f48
;  jal startAnimation
;.org 0x800a9f74
;  jal startAnimation
;.org 0x800a9fa0
;  jal startAnimation
;.org 0x800aa35c
;  jal startAnimation
;.org 0x800aa6bc
;  jal startAnimation
;.org 0x800aaa80
;  jal startAnimation
;.org 0x800aaac4
;  jal startAnimation
;.org 0x800aab98
;  jal startAnimation
;.org 0x800aabb0
;  jal startAnimation
;.org 0x800aabc0
;  jal startAnimation
;.org 0x800aabe4
;  jal startAnimation
;.org 0x800aac04
;  jal startAnimation
;.org 0x800aac1c
;  jal startAnimation
;.org 0x800aaf38
;  jal startAnimation
;.org 0x800ac064
;  j startAnimation
;.org 0x800b6268
;  jal startAnimation
;.org 0x800b6b24
;  jal startAnimation
;.org 0x800b6cc4
;  jal startAnimation
;.org 0x800b6dd0
;  jal startAnimation
;.org 0x800b6eb0
;  jal startAnimation
;.org 0x800b712c
;  jal startAnimation
;.org 0x800b721c
;  jal startAnimation
;.org 0x800b7264
;  jal startAnimation
;.org 0x800b7360
;  jal startAnimation
;.org 0x800b7584
;  jal startAnimation
;.org 0x800b7630
;  jal startAnimation
;.org 0x800b7748
;  jal startAnimation
;.org 0x800b7918
;  jal startAnimation
;.org 0x800b7c20
;  j startAnimation
;.org 0x800bf084
;  jal startAnimation
;.org 0x800bf1d4
;  jal startAnimation

;.org 0x800c4878
;  jal startAnimation
;.org 0x800db02c
;  jal startAnimation
;.org 0x800db080
;  jal startAnimation
;.org 0x800dda68
;  jal startAnimation
;.org 0x800ddae4
;  jal startAnimation
;.org 0x800ddaf4
;  jal startAnimation
;.org 0x800ddb30
;  jal startAnimation
;.org 0x800ddb6c
;  jal startAnimation
;.org 0x800ddc94
;  jal startAnimation
;.org 0x800ddd6c
;  jal startAnimation
;.org 0x800ddddc
;  jal startAnimation
;.org 0x800ddf48
;  jal startAnimation
;.org 0x800de074
;  jal startAnimation
;.org 0x800de0d0
;  jal startAnimation
;.org 0x800de0ec
;  jal startAnimation
;.org 0x800de150
;  jal startAnimation
;.org 0x800de254
;  jal startAnimation
;.org 0x800de3e0
;  jal startAnimation
;.org 0x800de480
;  jal startAnimation
;.org 0x800de4d8
;  jal startAnimation
;.org 0x800de57c
;  jal startAnimation
;.org 0x800de5e8
;  jal startAnimation
;.org 0x800de658
;  jal startAnimation
;.org 0x800de710
;  jal startAnimation
;.org 0x800de954
;  jal startAnimation
;.org 0x800de9b4
;  jal startAnimation
;.org 0x800dea00
;  jal startAnimation
;.org 0x800dea48
;  jal startAnimation
;.org 0x800deae0
;  jal startAnimation
;.org 0x800dedf0
;  jal startAnimation
;.org 0x800df08c
;  jal startAnimation
;.org 0x800df2c8
;  j startAnimation
;.org 0x800df4b8
;  jal startAnimation
;.org 0x800df5ec
;  j startAnimation
;.org 0x800df6f8
;  jal startAnimation
;.org 0x800df72c
;  jal startAnimation
;.org 0x800df78c
;  jal startAnimation
;.org 0x800df7c0
;  jal startAnimation
.org 0x800e7288
  jal startAnimation
.org 0x800e73f8
  jal startAnimation
.org 0x800e764c
  jal startAnimation
.org 0x800e7684
  jal startAnimation
.org 0x800e76c0
  jal startAnimation
.org 0x800e89c0
  j startAnimation
.org 0x800ef4a0
  jal startAnimation
.org 0x800ef4b0
  jal startAnimation
.org 0x800ef76c
  jal startAnimation
.org 0x800ef77c
  jal startAnimation
.org 0x800f04a0
  jal startAnimation
.org 0x800f0d70
  jal startAnimation
.org 0x800f0f64
  jal startAnimation
.org 0x800f1320
  jal startAnimation
.org 0x800f1340
  jal startAnimation
.org 0x800f1364
  jal startAnimation
.org 0x800f13ec
  jal startAnimation
.org 0x800f14f4
  jal startAnimation
.org 0x800f15b8
  jal startAnimation
.org 0x800f1b1c
  jal startAnimation
.org 0x800f1b8c
  jal startAnimation
.org 0x800f1e40
  jal startAnimation
.org 0x800f2b74
  jal startAnimation
.org 0x800f3510
  jal startAnimation
.org 0x800f3540
  jal startAnimation
.org 0x800f47e0
  jal startAnimation
.org 0x800f4888
  jal startAnimation
.org 0x800f5154
  jal startAnimation
.org 0x800f5428
  jal startAnimation
.org 0x800f5e68
  jal startAnimation
.org 0x800f5ebc
  jal startAnimation
.org 0x800f5f6c
  jal startAnimation
.org 0x800f6d90
  j startAnimation

;.org 0x800a9de0
;  jal tickAnimation
;.org 0x800b6b44
;  jal tickAnimation
;.org 0x800bf08c
;  jal tickAnimation
;.org 0x800bf1dc
;  jal tickAnimation
;.org 0x800dd880
;  jal tickAnimation
;.org 0x800dd9a8
;  jal tickAnimation
.org 0x800f13f8
  jal tickAnimation
.org 0x800f1428
  j tickAnimation
.org 0x800f1450
  jal tickAnimation
.org 0x800f1480
  jal tickAnimation
.org 0x800f1510
  jal tickAnimation
.org 0x800f7a9c
  j tickAnimation
.org 0x800f7ab8
  j tickAnimation

.org 0x800e23e0
  jal projectPosition

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