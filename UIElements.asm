.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x800ee8ac
  jal initializeUIBoxData

.org 0x800e64c8
  jal createStaticUIBox
.org 0x800f7fa8
  jal createStaticUIBox
.org 0x800f9fc8
  jal createStaticUIBox
.org 0x800fa518
  jal createStaticUIBox
.org 0x80100434
  jal createStaticUIBox

;.org 0x800c0518
;  jal removeStaticUIBox
;.org 0x800c053c
;  j removeStaticUIBox
;.org 0x800c31d0
;  jal removeStaticUIBox
;.org 0x800db288
;  jal removeStaticUIBox
.org 0x800e6440
  jal removeStaticUIBox
.org 0x800f7ffc
  jal removeStaticUIBox
.org 0x800fa058
  jal removeStaticUIBox
.org 0x800fa59c
  jal removeStaticUIBox
.org 0x8010061c
  jal removeStaticUIBox


;.org 0x800aa5f4
;  jal removeAnimatedUIBox
;.org 0x800aa9fc
;  jal removeAnimatedUIBox
;.org 0x800ab19c
;  jal removeAnimatedUIBox
;.org 0x800b80a8
;  jal removeAnimatedUIBox
;.org 0x800b9b6c
;  jal removeAnimatedUIBox
;.org 0x800b9f8c
;  jal removeAnimatedUIBox
;.org 0x800b9f98
;  jal removeAnimatedUIBox
;.org 0x800dbc60
;  jal removeAnimatedUIBox
;.org 0x800dbcd0
;  jal removeAnimatedUIBox
;.org 0x800dc020
;  jal removeAnimatedUIBox
;.org 0x800dc2c0
;  jal removeAnimatedUIBox
.org 0x800ee7e8
  jal removeAnimatedUIBox
.org 0x800f4dd8
  jal removeAnimatedUIBox
.org 0x800f4f70
  jal removeAnimatedUIBox
.org 0x8010062c
  jal removeAnimatedUIBox

;.org 0x800aa490
;  jal createAnimatedUIBox
;.org 0x800aa880
;  jal createAnimatedUIBox
;.org 0x800ab120
;  jal createAnimatedUIBox
;.org 0x800b8f2c
;  jal createAnimatedUIBox
;.org 0x800b9a60
;  jal createAnimatedUIBox
;.org 0x800b9ddc
;  jal createAnimatedUIBox
;.org 0x800b9e08
;  jal createAnimatedUIBox
;.org 0x800db3fc
;  jal createAnimatedUIBox
;.org 0x800db4a8
;  jal createAnimatedUIBox
;.org 0x800dbe50
;  jal createAnimatedUIBox
;.org 0x800dc138
;  jal createAnimatedUIBox
;.org 0x800dc3cc
;  jal createAnimatedUIBox
.org 0x800ed654
  jal createAnimatedUIBox
.org 0x800ed6f8
  jal createAnimatedUIBox
.org 0x800ee068
  jal createAnimatedUIBox
.org 0x800f5104
  jal createAnimatedUIBox
.org 0x80100460
  jal createAnimatedUIBox

.org 0x8010d300
  jal renderUIBoxBorder

.org 0x800ee720
  jal drawLine3P
.org 0x800ee754
  jal drawLine3P
.org 0x800fda94
  jal drawLine3P
.org 0x800fdac0
  jal drawLine3P
.org 0x800fdb08
  jal drawLine3P
.org 0x800fdb3c
  jal drawLine3P
.org 0x800fe204
  jal drawLine3P
.org 0x800fe230
  jal drawLine3P
.org 0x800ff3f0
  jal drawLine3P
.org 0x800ff41c
  jal drawLine3P


.org 0x800edce8
  jal drawLine2P
.org 0x800edd0c
  jal drawLine2P
.org 0x800ee4c0
  jal drawLine2P
.org 0x800ee4f8
  jal drawLine2P
.org 0x800fe0cc
  jal drawLine2P
.org 0x800fe0fc
  jal drawLine2P
.org 0x800fe12c
  jal drawLine2P
.org 0x800ff600
  jal drawLine2P
.org 0x800ff630
  jal drawLine2P
.org 0x800ff660
  jal drawLine2P
.org 0x8010b5c4
  jal drawLine2P
.org 0x8010b5f4
  jal drawLine2P
.org 0x8010b624
  jal drawLine2P


;.org 0x800e5e78
;  jal setUVDataPolyFT4
;.org 0x800e6148
;  jal setUVDataPolyFT4
.org 0x800eced0
  jal setUVDataPolyFT4
.org 0x800ee37c
  jal setUVDataPolyFT4
.org 0x800f2a10
  jal setUVDataPolyFT4
.org 0x800f72e4
  jal setUVDataPolyFT4
.org 0x800f8be0
  jal setUVDataPolyFT4
.org 0x800f8cac
  jal setUVDataPolyFT4
.org 0x800f8dd0
  jal setUVDataPolyFT4
.org 0x800f8e4c
  jal setUVDataPolyFT4
.org 0x800f8ed4
  jal setUVDataPolyFT4
.org 0x800f8f58
  jal setUVDataPolyFT4
.org 0x800f9014
  jal setUVDataPolyFT4
.org 0x800f91f4
  jal setUVDataPolyFT4
.org 0x800f93b4
  jal setUVDataPolyFT4
.org 0x800f96d0
  jal setUVDataPolyFT4
.org 0x800f9770
  jal setUVDataPolyFT4
.org 0x800f980c
  jal setUVDataPolyFT4
.org 0x800f9a1c
  jal setUVDataPolyFT4
.org 0x800f9a90
  jal setUVDataPolyFT4
.org 0x800f9afc
  jal setUVDataPolyFT4
.org 0x800f9c18
  jal setUVDataPolyFT4
.org 0x800f9d24
  jal setUVDataPolyFT4
.org 0x800fa3b4
  jal setUVDataPolyFT4
.org 0x800fa404
  jal setUVDataPolyFT4
.org 0x800fa660
  jal setUVDataPolyFT4
.org 0x800fa69c
  jal setUVDataPolyFT4
.org 0x800fa6e8
  jal setUVDataPolyFT4
.org 0x800fa734
  jal setUVDataPolyFT4
.org 0x800fd87c
  jal setUVDataPolyFT4
.org 0x800fe2dc
  jal setUVDataPolyFT4
.org 0x80101538
  jal setUVDataPolyFT4

;.org 0x800e5db0
;  jal setPosDataPolyFT4
;.org 0x800e6160
;  jal setPosDataPolyFT4
.org 0x800ee3a0
  jal setPosDataPolyFT4
.org 0x800f2a38
  jal setPosDataPolyFT4
.org 0x800f7300
  jal setPosDataPolyFT4
.org 0x800f8bfc
  jal setPosDataPolyFT4
.org 0x800f8cc8
  jal setPosDataPolyFT4
.org 0x800f8e64
  jal setPosDataPolyFT4
.org 0x800f8eec
  jal setPosDataPolyFT4
.org 0x800f8f70
  jal setPosDataPolyFT4
.org 0x800f9030
  jal setPosDataPolyFT4
.org 0x800f9234
  jal setPosDataPolyFT4
.org 0x800f93ec
  jal setPosDataPolyFT4
.org 0x800f9894
  jal setPosDataPolyFT4
.org 0x800f9b60
  jal setPosDataPolyFT4
.org 0x800f9c48
  jal setPosDataPolyFT4
.org 0x800f9d50
  jal setPosDataPolyFT4
.org 0x800fa454
  jal setPosDataPolyFT4
.org 0x800fa7d0
  jal setPosDataPolyFT4
.org 0x800fd898
  jal setPosDataPolyFT4
.org 0x800fe2f4
  jal setPosDataPolyFT4
.org 0x80101554
  jal setPosDataPolyFT4
.org 0x8010b310
  jal setPosDataPolyFT4
.org 0x8010b348
  jal setPosDataPolyFT4

.org 0x800faf50
  jal renderSelectionCursor
.org 0x800fbbd0
  jal renderSelectionCursor
.org 0x800fffd8
  jal renderSelectionCursor
.org 0x801081cc
  jal renderSelectionCursor
.org 0x80109044
  jal renderSelectionCursor
.org 0x801094a0
  jal renderSelectionCursor
.org 0x801097a4
  jal renderSelectionCursor
.org 0x80109b68
  jal renderSelectionCursor

.close


.open "work/DIGIMON/DGET_REL.BIN",0x80080800
.psx

.org 0x800816e4
  jal renderSelectionCursor

.close


.open "work/DIGIMON/BTL_REL.BIN",0x80052ae0
.psx

.org 0x8005d16c
  jal drawLine3P
.org 0x8005d1a0
  jal drawLine3P

.org 0x8005d1cc
  jal drawLine2P
.org 0x80065de4
  jal drawLine2P
.org 0x8006a180
  jal drawLine2P
.org 0x8006a1dc
  jal drawLine2P

.org 0x80061754
  jal setUVDataPolyFT4
.org 0x80063fb0
  jal setUVDataPolyFT4

.org 0x800617b8
  jal setPosDataPolyFT4
.org 0x8006180c
  jal setPosDataPolyFT4
.org 0x80063fd8
  jal setPosDataPolyFT4

.close

.open "work/DIGIMON/VS_REL.BIN",0x80052ae0
.psx

.org 0x8006459c
  jal drawLine2P
.org 0x8006885c
  jal drawLine2P
.org 0x800688b8
  jal drawLine2P

.org 0x800572e8
  jal setUVDataPolyFT4
.org 0x80057554
  jal setUVDataPolyFT4
.org 0x80057710
  jal setUVDataPolyFT4
.org 0x800578c8
  jal setUVDataPolyFT4
.org 0x800578e8
  jal setUVDataPolyFT4
.org 0x80057ad8
  jal setUVDataPolyFT4
.org 0x80057b18
  jal setUVDataPolyFT4
.org 0x80057c94
  jal setUVDataPolyFT4
.org 0x8005b084
  jal setUVDataPolyFT4
.org 0x8005b110
  jal setUVDataPolyFT4
.org 0x8005b1d4
  jal setUVDataPolyFT4
.org 0x8005b288
  jal setUVDataPolyFT4
.org 0x8005fe50
  jal setUVDataPolyFT4
.org 0x80061934
  jal setUVDataPolyFT4

.org 0x80057314
  jal setPosDataPolyFT4
.org 0x8005757c
  jal setPosDataPolyFT4
.org 0x8005759c
  jal setPosDataPolyFT4
.org 0x8005773c
  jal setPosDataPolyFT4
.org 0x8005775c
  jal setPosDataPolyFT4
.org 0x80057900
  jal setPosDataPolyFT4
.org 0x80057af4
  jal setPosDataPolyFT4
.org 0x80057b44
  jal setPosDataPolyFT4
.org 0x80057b68
  jal setPosDataPolyFT4
.org 0x80057cb8
  jal setPosDataPolyFT4
.org 0x8005b0a0
  jal setPosDataPolyFT4
.org 0x8005b12c
  jal setPosDataPolyFT4
.org 0x8005b200
  jal setPosDataPolyFT4
.org 0x8005b2a4
  jal setPosDataPolyFT4
.org 0x8005fee0
  jal setPosDataPolyFT4
.org 0x8005ff60
  jal setPosDataPolyFT4
.org 0x8006195c
  jal setPosDataPolyFT4

.close

.open "work/DIGIMON/STD_REL.BIN",0x80052ae0
.psx

.org 0x80060d30
  jal removeAnimatedUIBox
.org 0x80060e98
  jal removeAnimatedUIBox

.org 0x80061074
  jal createAnimatedUIBox

.org 0x8006e4cc
  jal drawLine2P
.org 0x8007278c
  jal drawLine2P
.org 0x800727e8
  jal drawLine2P

.org 0x80059498
  jal setUVDataPolyFT4
.org 0x8005be04
  jal setUVDataPolyFT4
.org 0x8005be90
  jal setUVDataPolyFT4
.org 0x8005bf54
  jal setUVDataPolyFT4
.org 0x8005c008
  jal setUVDataPolyFT4
.org 0x8005d2b4
  jal setUVDataPolyFT4
.org 0x8005d6f0
  jal setUVDataPolyFT4
.org 0x8005d7fc
  jal setUVDataPolyFT4
.org 0x8005d920
  jal setUVDataPolyFT4
.org 0x8005e0c4
  jal setUVDataPolyFT4
.org 0x8005e2b4
  jal setUVDataPolyFT4
.org 0x8005e41c
  jal setUVDataPolyFT4
.org 0x8005e768
  jal setUVDataPolyFT4
.org 0x8005e830
  jal setUVDataPolyFT4
.org 0x800632ac
  jal setUVDataPolyFT4
.org 0x800689ac
  jal setUVDataPolyFT4
.org 0x8006a5d4
  jal setUVDataPolyFT4

.org 0x800594c0
  jal setPosDataPolyFT4
.org 0x8005be20
  jal setPosDataPolyFT4
.org 0x8005beac
  jal setPosDataPolyFT4
.org 0x8005bf80
  jal setPosDataPolyFT4
.org 0x8005c024
  jal setPosDataPolyFT4
.org 0x8005d2d0
  jal setPosDataPolyFT4
.org 0x8005d714
  jal setPosDataPolyFT4
.org 0x8005d938
  jal setPosDataPolyFT4
.org 0x8005e0e8
  jal setPosDataPolyFT4
.org 0x8005e2e4
  jal setPosDataPolyFT4
.org 0x8005e444
  jal setPosDataPolyFT4
.org 0x8005e784
  jal setPosDataPolyFT4
.org 0x8005e860
  jal setPosDataPolyFT4
.org 0x800632c8
  jal setPosDataPolyFT4
.org 0x80068a3c
  jal setPosDataPolyFT4
.org 0x80068abc
  jal setPosDataPolyFT4
.org 0x8006a5fc
  jal setPosDataPolyFT4

.close 

.open "work/DIGIMON/KAR_REL.BIN",0x80053800
.psx

.org 0x80058c78
  jal removeAnimatedUIBox
.org 0x80058fa8
  jal removeAnimatedUIBox

.org 0x80058bf8
  jal createAnimatedUIBox
.org 0x80058eb0
  jal createAnimatedUIBox

.org 0x80058dcc
  jal renderSelectionCursor

.close

.open "work/DIGIMON/DOO2_REL.BIN",0x80070000
.psx

.org 0x80071a58
  jal removeAnimatedUIBox

.org 0x80071908
  jal createAnimatedUIBox

.close

.open "work/DIGIMON/MURD_REL.BIN",0x8007C000
.psx

.org 0x8007cc1c
  j removeStaticUIBox

.org 0x8007cb10
  jal removeAnimatedUIBox

.org 0x8007ca40
  jal createAnimatedUIBox

.close 

.open "work/DIGIMON/FISH_REL.BIN",0x80070000
.psx

.org 0x800717d0
  jal createStaticUIBox

.org 0x80071460
  jal removeStaticUIBox

.org 0x800707b8
  jal removeAnimatedUIBox

.org 0x800712e4
  jal createAnimatedUIBox

.org 0x80077d60
  jal drawLine2P

.org 0x80070fec
  jal renderSelectionCursor

.close

.open "work/DIGIMON/TRN_REL.BIN",0x80088800
.psx

.org 0x80089ca8
  jal createMenuBox
.org 0x8008a9c8
  jal createMenuBox

.org 0x8008d47c
  jal removeAnimatedUIBox
.org 0x8008ded4
  jal removeAnimatedUIBox
.org 0x8008e53c
  jal removeAnimatedUIBox

.org 0x8008d750
  jal createAnimatedUIBox
.org 0x8008ef1c
  jal createAnimatedUIBox

.org 0x8008de0c
  jal drawLine3P
.org 0x8008de40
  jal drawLine3P

.org 0x8008dba8
  jal drawLine2P
.org 0x8008dbe0
  jal drawLine2P

.org 0x8008da64
  jal setUVDataPolyFT4

.org 0x8008da88
  jal setPosDataPolyFT4

.close

.open "work/DIGIMON/TRN2_REL.BIN",0x80088800
.psx

.org 0x8008c8ac
  jal removeAnimatedUIBox
.org 0x8008cf14
  jal removeAnimatedUIBox

.org 0x8008c128
  jal createAnimatedUIBox
.org 0x8008d8f4
  jal createAnimatedUIBox

.org 0x8008c7e4
  jal drawLine3P
.org 0x8008c818
  jal drawLine3P

.org 0x8008c580
  jal drawLine2P
.org 0x8008c5b8
  jal drawLine2P

.org 0x8008c460
  jal setPosDataPolyFT4

.org 0x8008c43c
  jal setUVDataPolyFT4

.close
