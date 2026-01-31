.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x801010b4
  jal worldPosToScreenPos2
  
;.org 0x800a40ac
;  jal setRotTransMatrix

;.org 0x800a47cc
;  jal random
;.org 0x800a5f10
;  jal random
;.org 0x800a6970
;  jal random
;.org 0x800a6e80
;  jal random
;.org 0x800a6f6c
;  jal random
;.org 0x800a72e4
;  jal random
;.org 0x800a74ec
;  jal random
;.org 0x800a74fc
;  jal random
;.org 0x800a8110
;  jal random
;.org 0x800a8484
;  jal random
;.org 0x800a8614
;  jal random
;.org 0x800acc0c
;  jal random
;.org 0x800b403c
;  jal random
;.org 0x800b4048
;  jal random
;.org 0x800b4d88
;  jal random
;.org 0x800b4f60
;  jal random
;.org 0x800b52a8
;  jal random
;.org 0x800b52c4
;  jal random
;.org 0x800b5438
;  jal random
;.org 0x800b5454
;  jal random
;.org 0x800c5694
;  jal random
;.org 0x800c56e8
;  jal random
;.org 0x800c59f0
;  jal random
;.org 0x800def60
;  jal random
;.org 0x800deff4
;  jal random
;.org 0x800df03c
;  jal random
;.org 0x800e32e0
;  jal random
;.org 0x800e6aec
;  jal random
;.org 0x800e6b78
;  jal random
;.org 0x800e8788
;  jal random
;.org 0x800e88f0
;  jal random
.org 0x800ed054
  jal random
.org 0x800ed1e0
  jal random
.org 0x800ed2b0
  jal random
.org 0x800ed370
  jal random
.org 0x800ed524
  jal random
.org 0x800ed560
  jal random
.org 0x800f3cd4
  jal random
.org 0x800f44a4
  jal random
.org 0x800f44d0
  jal random
.org 0x800f5570
  jal random
.org 0x800f6958
  jal random
.org 0x800f6974
  jal random
.org 0x800f70ec
  jal random
.org 0x800fc1ec
  jal random
.org 0x800fc3b4
  jal random
.org 0x800fe588
  jal random
.org 0x80102d24
  jal random
.org 0x80106e20
  jal random
.org 0x80106ee0
  jal random

;.org 0x800adae8
;  jal sin
;.org 0x800addd8
;  jal sin
;.org 0x800aebbc
;  jal sin
;.org 0x800aec6c
;  jal sin
;.org 0x800c12d8
;  jal sin
;.org 0x800e9e58
;  jal sin
;.org 0x800e9ef8
;  jal sin
;.org 0x800ecd80
;  jal sin

;.org 0x800adb04
;  jal cos
;.org 0x800c1320
;  jal cos
;.org 0x800e9e98
;  jal cos

;.org 0x800ab228
;  jal atan
;.org 0x800b6f28
;  jal atan
;.org 0x800d45d0
;  jal atan
;.org 0x800e81f8
;  jal atan

;.org 0x800add78
;  jal lerp
;.org 0x800add94
;  jal lerp
;.org 0x800addb0
;  jal lerp
;.org 0x800addcc
;  jal lerp
;.org 0x800aeb54
;  jal lerp
;.org 0x800aeb74
;  jal lerp
;.org 0x800aeb94
;  jal lerp
;.org 0x800aebb4
;  jal lerp
;.org 0x800aec20
;  jal lerp
;.org 0x800aec64
;  jal lerp
;.org 0x800c11bc
;  jal lerp
;.org 0x800c1284
;  jal lerp
;.org 0x800c1378
;  jal lerp
;.org 0x800e9e4c
;  jal lerp
;.org 0x800e9eec
;  jal lerp
;.org 0x800ecd04
;  jal lerp
;.org 0x800ecd30
;  jal lerp
;.org 0x800ecd78
;  jal lerp

;.org 0x800adc6c
;  jal worldPosToScreenPos
;.org 0x800ae164
;  jal worldPosToScreenPos
;.org 0x800ae530
;  jal worldPosToScreenPos
;.org 0x800aea1c
;  jal worldPosToScreenPos
;.org 0x800daa30
;  jal worldPosToScreenPos
;.org 0x800e7f40
;  jal worldPosToScreenPos
;.org 0x800e9f40
;  jal worldPosToScreenPos
;.org 0x800ec6ac
;  jal worldPosToScreenPos
.org 0x800fbee8
  jal worldPosToScreenPos

;.org 0x800ec530
;  j setInt16WithStride

.org 0x800ed5fc
  jal getEntityScreenPos
.org 0x800ee010
  jal getEntityScreenPos
.org 0x80101120
  jal getEntityScreenPos
.org 0x80109c6c
  jal getEntityScreenPos
.org 0x80109e14
  jal getEntityScreenPos
.org 0x80109f38
  jal getEntityScreenPos

.close

.open "work/DIGIMON/BTL_REL.BIN",0x80052ae0
.psx

.org 0x8005cf74
  jal addWithLimit

.org 0x80066678
  jal findAABBHitEntity
.org 0x8006ab58
  jal findAABBHitEntity
.org 0x80070d4c
  jal findAABBHitEntity

.org 0x80069384
  jal setRotTransMatrix
.org 0x80069484
  jal setRotTransMatrix

.org 0x80057cc4
  jal random
.org 0x80058178
  jal random
.org 0x800584e0
  jal random
.org 0x80058810
  jal random
.org 0x80059874
  jal random
.org 0x8005acd4
  jal random
.org 0x8005acf0
  jal random
.org 0x8005b560
  jal random
.org 0x8005be2c
  jal random
.org 0x8005c050
  jal random
.org 0x8005c174
  jal random
.org 0x8005c1a8
  jal random
.org 0x8005c4bc
  jal random
.org 0x8005c564
  jal random
.org 0x8005c5ac
  jal random
.org 0x8005c974
  jal random
.org 0x8005cbc0
  jal random
.org 0x8005cd0c
  jal random
.org 0x8005e738
  jal random
.org 0x8005ea9c
  jal random
.org 0x8005efac
  jal random
.org 0x8005fc38
  jal random
.org 0x8005fc68
  jal random
.org 0x8005ff48
  jal random
.org 0x80060038
  jal random
.org 0x80060560
  jal random
.org 0x80060d8c
  jal random
.org 0x8006311c
  jal random
.org 0x800631ac
  jal random
.org 0x800631e8
  jal random
.org 0x8006321c
  jal random
.org 0x80063250
  jal random
.org 0x80063294
  jal random
.org 0x800632c0
  jal random
.org 0x800632e0
  jal random

.org 0x8005e460
  jal sin
.org 0x8005e470
  jal sin
.org 0x8005e4c4
  jal sin
.org 0x8005e4e8
  jal sin
.org 0x80067138
  jal sin
.org 0x8006b4ec
  jal sin
.org 0x8006b664
  jal sin
.org 0x8006b680
  jal sin
.org 0x8006c0cc
  jal sin
.org 0x8006d414
  jal sin
.org 0x8006d430
  jal sin
.org 0x8006ef48
  jal sin
.org 0x8006f2b4
  jal sin
.org 0x80070254
  jal sin

.org 0x800670f8
  jal cos
.org 0x8006b504
  jal cos
.org 0x8006c0e4
  jal cos
.org 0x8006f2f8
  jal cos
.org 0x80070120
  jal cos

.org 0x8005c274
  jal atan
.org 0x8005c318
  jal atan
.org 0x8005c3ac
  jal atan
.org 0x8005c42c
  jal atan
.org 0x8006b038
  jal atan
.org 0x8006b0bc
  jal atan
.org 0x8006b494
  jal atan
.org 0x8006b570
  jal atan
.org 0x8006ce50
  jal atan
.org 0x8006e79c
  jal atan

.org 0x8006d024
  jal lerp
.org 0x8006d040
  jal lerp
.org 0x8006d05c
  jal lerp
.org 0x8006eef0
  jal lerp
.org 0x8006ef3c
  jal lerp
.org 0x8006f754
  jal lerp
.org 0x8006fc54
  jal lerp
.org 0x8006fd08
  jal lerp
.org 0x80070118
  jal lerp
.org 0x800701c0
  jal lerp
.org 0x80071434
  jal lerp
.org 0x80071b6c
  jal lerp
.org 0x80071ba0
  jal lerp
.org 0x80071c08
  jal lerp
.org 0x80071c4c
  jal lerp
.org 0x800724b8
  jal lerp
.org 0x8007251c
  jal lerp
.org 0x800726f8
  jal lerp

.org 0x80065d50
  jal worldPosToScreenPos
.org 0x8006715c
  jal worldPosToScreenPos
.org 0x8006c6e0
  jal worldPosToScreenPos
.org 0x8006e96c
  jal worldPosToScreenPos
.org 0x8006e998
  jal worldPosToScreenPos
.org 0x8006ef20
  jal worldPosToScreenPos
.org 0x8006f808
  jal worldPosToScreenPos

.org 0x8006ed34
  j setInt16WithStride
.org 0x8006f3e8
  jal setInt16WithStride
.org 0x80071690
  jal setInt16WithStride
.org 0x80072a5c
  jal setInt16WithStride

.org 0x8005d0d4
  jal getEntityScreenPos
.org 0x80061038
  jal getEntityScreenPos
.org 0x80062494
  jal getEntityScreenPos
  
.close


.open "work/DIGIMON/STD_REL.BIN",0x80052ae0
.psx

.org 0x8006472c
  jal addWithLimit

.org 0x8006ec70
  jal findAABBHitEntity
.org 0x80073164
  jal findAABBHitEntity
.org 0x800792ec
  jal findAABBHitEntity

.org 0x8007191c
  jal setRotTransMatrix
.org 0x80071a58
  jal setRotTransMatrix

.org 0x80056f54
  jal random
.org 0x80056fac
  jal random
.org 0x80057004
  jal random
.org 0x8005705c
  jal random
.org 0x800570c0
  jal random
.org 0x80057118
  jal random
.org 0x8005736c
  jal random
.org 0x80057410
  jal random
.org 0x8005743c
  jal random
.org 0x8005a584
  jal random
.org 0x8005a5c4
  jal random
.org 0x8005a6a4
  jal random
.org 0x8005da14
  jal random
.org 0x8005da48
  jal random
.org 0x8005dac0
  jal random
.org 0x8005dbcc
  jal random
.org 0x8005dc8c
  jal random
.org 0x8005f744
  jal random
.org 0x8005fc1c
  jal random
.org 0x8005ff44
  jal random
.org 0x80060228
  jal random
.org 0x80060244
  jal random
.org 0x80060538
  jal random
.org 0x80060564
  jal random
.org 0x800614d4
  jal random
.org 0x800628d4
  jal random
.org 0x800628f0
  jal random
.org 0x800630b8
  jal random
.org 0x80064038
  jal random
.org 0x8006415c
  jal random
.org 0x80064190
  jal random
.org 0x800644a4
  jal random
.org 0x8006454c
  jal random
.org 0x80064594
  jal random
.org 0x800649c0
  jal random
.org 0x80064a44
  jal random
.org 0x80064c50
  jal random
.org 0x80064d9c
  jal random
.org 0x80065d68
  jal random
.org 0x800664b0
  jal random
.org 0x800667e4
  jal random
.org 0x80066ba4
  jal random
.org 0x8006746c
  jal random
.org 0x8006749c
  jal random
.org 0x80067978
  jal random
.org 0x800681b8
  jal random
.org 0x80069740
  jal random
.org 0x800697d0
  jal random
.org 0x8006980c
  jal random
.org 0x80069840
  jal random
.org 0x80069874
  jal random
.org 0x800698b8
  jal random
.org 0x800698e4
  jal random
.org 0x80069904
  jal random
.org 0x8006c740
  jal random

.org 0x800662a4
  jal sin
.org 0x800662b4
  jal sin
.org 0x80066308
  jal sin
.org 0x8006632c
  jal sin
.org 0x8006f730
  jal sin
.org 0x80073af8
  jal sin
.org 0x80073c70
  jal sin
.org 0x80073c8c
  jal sin
.org 0x800746d8
  jal sin
.org 0x800759f0
  jal sin
.org 0x80075a0c
  jal sin
.org 0x800774e8
  jal sin
.org 0x80077854
  jal sin
.org 0x800787f4
  jal sin

.org 0x8006f6f0
  jal cos
.org 0x80073b10
  jal cos
.org 0x800746f0
  jal cos
.org 0x80077898
  jal cos
.org 0x800786c0
  jal cos

.org 0x80059c60
  jal atan
.org 0x80059cf8
  jal atan
.org 0x8005a0ac
  jal atan
.org 0x8005b78c
  jal atan
.org 0x8006425c
  jal atan
.org 0x80064300
  jal atan
.org 0x80064394
  jal atan
.org 0x80064414
  jal atan
.org 0x80073644
  jal atan
.org 0x800736c8
  jal atan
.org 0x80073aa0
  jal atan
.org 0x80073b7c
  jal atan
.org 0x8007542c
  jal atan
.org 0x80076d78
  jal atan

.org 0x8005ac64
  jal lerp
.org 0x8005ac80
  jal lerp
.org 0x8005acb0
  jal lerp
.org 0x8005acd0
  jal lerp
.org 0x8005afc0
  jal lerp
.org 0x8005afe0
  jal lerp
.org 0x8005b4ec
  jal lerp
.org 0x80075600
  jal lerp
.org 0x8007561c
  jal lerp
.org 0x80075638
  jal lerp
.org 0x80077490
  jal lerp
.org 0x800774dc
  jal lerp
.org 0x80077cf4
  jal lerp
.org 0x800781f4
  jal lerp
.org 0x800782a8
  jal lerp
.org 0x800786b8
  jal lerp
.org 0x80078760
  jal lerp

.org 0x8006e438
  jal worldPosToScreenPos
.org 0x8006f754
  jal worldPosToScreenPos
.org 0x80074cec
  jal worldPosToScreenPos
.org 0x80076f38
  jal worldPosToScreenPos
.org 0x80076f60
  jal worldPosToScreenPos
.org 0x800774c0
  jal worldPosToScreenPos
.org 0x80077da8
  jal worldPosToScreenPos

.org 0x800772d4
  j setInt16WithStride
.org 0x80077988
  jal setInt16WithStride

.close

.open "work/DIGIMON/VS_REL.BIN",0x80052ae0
.psx

.org 0x8005c334
  jal addWithLimit

.org 0x80064d40
  jal findAABBHitEntity
.org 0x80069234
  jal findAABBHitEntity
.org 0x8006f3bc
  jal findAABBHitEntity

.org 0x800679ec
  jal setRotTransMatrix
.org 0x80067b28
  jal setRotTransMatrix

.org 0x8005990c
  jal random
.org 0x8005994c
  jal random
.org 0x80059a20
  jal random
.org 0x8005bcc8
  jal random
.org 0x8005bd24
  jal random
.org 0x8005c0ac
  jal random
.org 0x8005c154
  jal random
.org 0x8005c19c
  jal random
.org 0x8005c5a0
  jal random
.org 0x8005c5e4
  jal random
.org 0x8005c7a4
  jal random
.org 0x8005c8f0
  jal random
.org 0x8005d648
  jal random
.org 0x8005de94
  jal random
.org 0x8005e1c8
  jal random
.org 0x8005e588
  jal random
.org 0x8005edf8
  jal random
.org 0x8005f704
  jal random
.org 0x80060a18
  jal random
.org 0x80060aa8
  jal random
.org 0x80060ae4
  jal random
.org 0x80060b18
  jal random
.org 0x80060b4c
  jal random
.org 0x80060b90
  jal random
.org 0x80060bbc
  jal random
.org 0x80060bdc
  jal random

.org 0x8005dc88
  jal sin
.org 0x8005dc98
  jal sin
.org 0x8005dcec
  jal sin
.org 0x8005dd10
  jal sin
.org 0x80065800
  jal sin
.org 0x80069bc8
  jal sin
.org 0x80069d40
  jal sin
.org 0x80069d5c
  jal sin
.org 0x8006a7a8
  jal sin
.org 0x8006bac0
  jal sin
.org 0x8006badc
  jal sin
.org 0x8006d5b8
  jal sin
.org 0x8006d924
  jal sin
.org 0x8006e8c4
  jal sin

.org 0x800657c0
  jal cos
.org 0x80069be0
  jal cos
.org 0x8006a7c0
  jal cos
.org 0x8006d968
  jal cos
.org 0x8006e790
  jal cos

.org 0x800591a4
  jal atan
.org 0x8005923c
  jal atan
.org 0x800595fc
  jal atan
.org 0x8005aa0c
  jal atan
.org 0x8005be64
  jal atan
.org 0x8005bf08
  jal atan
.org 0x8005bf9c
  jal atan
.org 0x8005c01c
  jal atan
.org 0x80069714
  jal atan
.org 0x80069798
  jal atan
.org 0x80069b70
  jal atan
.org 0x80069c4c
  jal atan
.org 0x8006b4fc
  jal atan
.org 0x8006ce48
  jal atan

.org 0x80059ee0
  jal lerp
.org 0x80059efc
  jal lerp
.org 0x80059f2c
  jal lerp
.org 0x80059f4c
  jal lerp
.org 0x8005a23c
  jal lerp
.org 0x8005a25c
  jal lerp
.org 0x8005a768
  jal lerp
.org 0x8006b6d0
  jal lerp
.org 0x8006b6ec
  jal lerp
.org 0x8006b708
  jal lerp
.org 0x8006d560
  jal lerp
.org 0x8006d5ac
  jal lerp
.org 0x8006ddc4
  jal lerp
.org 0x8006e2c4
  jal lerp
.org 0x8006e378
  jal lerp
.org 0x8006e788
  jal lerp
.org 0x8006e830
  jal lerp

.org 0x80064508
  jal worldPosToScreenPos
.org 0x80065824
  jal worldPosToScreenPos
.org 0x8006adbc
  jal worldPosToScreenPos
.org 0x8006d008
  jal worldPosToScreenPos
.org 0x8006d030
  jal worldPosToScreenPos
.org 0x8006d590
  jal worldPosToScreenPos
.org 0x8006de78
  jal worldPosToScreenPos

.org 0x8006d3a4
  j setInt16WithStride
.org 0x8006da58
  jal setInt16WithStride

.close


.open "work/DIGIMON/TRN_REL.BIN",0x80088800
.psx

.org 0x800889a0
  jal random
.org 0x8008a930
  jal random

.org 0x8008e02c
  jal worldPosToScreenPos

.org 0x8008d448
  jal getEntityScreenPos
.org 0x8008d6f8
  jal getEntityScreenPos

.close

.open "work/DIGIMON/TRN2_REL.BIN",0x80088800
.psx

.org 0x8008ca04
  jal worldPosToScreenPos

.org 0x8008c0d0
  jal getEntityScreenPos

.close

.open "work/DIGIMON/EAB_REL.BIN",0x80060000
.psx

.org 0x80060330
  jal sin
.org 0x80060580
  jal sin
.org 0x80060910
  jal sin
.org 0x800609c4
  jal sin

.org 0x80060304
  jal lerp
.org 0x80060328
  jal lerp
.org 0x80060554
  jal lerp
.org 0x80060578
  jal lerp
.org 0x80060858
  jal lerp
.org 0x800608e8
  jal lerp
.org 0x80060908
  jal lerp
.org 0x800609b4
  jal lerp
.org 0x80060a40
  jal lerp
.org 0x80060a80
  jal lerp
.org 0x80060aa0
  jal lerp
.org 0x80061238
  jal lerp
.org 0x800613b0
  jal lerp
.org 0x800613d0
  jal lerp
.org 0x800613f0
  jal lerp

.org 0x800612e8
  jal worldPosToScreenPos

.close

.open "work/DIGIMON/ENDI_REL.BIN",0x80060000
.psx

.org 0x800603fc
  jal lerp
.org 0x8006041c
  jal lerp

.close

.open "work/DIGIMON/EVL_REL.BIN",0x80060000
.psx

.org 0x80060310
  jal lerp
.org 0x8006055c
  jal lerp
.org 0x80060908
  jal lerp
.org 0x80060928
  jal lerp
.org 0x80060994
  jal lerp
.org 0x800609f4
  jal lerp
.org 0x80060a54
  jal lerp
.org 0x80060afc
  jal lerp
.org 0x80060c58
  jal lerp
.org 0x80060cd0
  jal lerp
.org 0x80060cfc
  jal lerp
.org 0x80060e60
  jal lerp
.org 0x800616bc
  jal lerp
.org 0x800616e0
  jal lerp
.org 0x80061704
  jal lerp
.org 0x8006172c
  jal lerp
.org 0x80061748
  jal lerp
.org 0x80061778
  jal lerp
.org 0x8006179c
  jal lerp
.org 0x800617c0
  jal lerp
.org 0x80061808
  jal lerp
.org 0x80061820
  jal lerp
.org 0x8006183c
  jal lerp
.org 0x80062b7c
  jal lerp
.org 0x80062cf4
  jal lerp
.org 0x80062d14
  jal lerp
.org 0x80062d34
  jal lerp

.org 0x800615bc
  jal worldPosToScreenPos
.org 0x80062c2c
  jal worldPosToScreenPos

.close

.open "work/DIGIMON/MURD_REL.BIN",0x8007C000
.psx

.org 0x8007c2fc
  jal lerp
.org 0x8007c4c4
  jal lerp
.org 0x8007c7cc
  jal lerp
.org 0x8007ceec
  jal lerp
.org 0x8007d074
  jal lerp

.org 0x8007c7f0
  jal worldPosToScreenPos
.org 0x8007c9cc
  jal worldPosToScreenPos
.org 0x8007cac4
  jal worldPosToScreenPos
.org 0x8007cf10
  jal worldPosToScreenPos

.close

.open "work/DIGIMON/DOOA_REL.BIN",0x80080000
.psx

.org 0x80080c88
  jal sin

.org 0x800808a4
  jal lerp
.org 0x8008098c
  jal lerp
.org 0x80080a50
  jal lerp
.org 0x80080b54
  jal lerp
.org 0x80080c80
  jal lerp
.org 0x80080eb8
  jal lerp
.org 0x80081148
  jal lerp
.org 0x80081638
  jal lerp
.org 0x8008165c
  jal lerp
.org 0x80081680
  jal lerp
.org 0x800816a8
  jal lerp
.org 0x800816c4
  jal lerp
.org 0x8008172c
  jal lerp
.org 0x80081750
  jal lerp
.org 0x80081774
  jal lerp
.org 0x80081794
  jal lerp
.org 0x800817ac
  jal lerp
.org 0x800817c8
  jal lerp
.org 0x80081bc4
  jal lerp
.org 0x80081d4c
  jal lerp
.org 0x80082234
  jal lerp
.org 0x800822f4
  jal lerp
.org 0x800823a0
  jal lerp
.org 0x80082400
  jal lerp
.org 0x800826bc
  jal lerp
.org 0x80083244
  jal lerp

.org 0x8008116c
  jal worldPosToScreenPos
.org 0x800815bc
  jal worldPosToScreenPos
.org 0x80081be8
  jal worldPosToScreenPos

.close


.open "work/DIGIMON/DOO2_REL.BIN",0x80070000
.psx

.org 0x80070cd4
  jal setRotTransMatrix

.close


.open "work/DIGIMON/FISH_REL.BIN",0x80070000
.psx

.org 0x800711c4
  jal getEntityScreenPos
.org 0x80071754
  jal getEntityScreenPos

.close
