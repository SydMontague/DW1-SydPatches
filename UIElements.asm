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
.org 0x800db288
  jal removeStaticUIBox
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
.org 0x800dbc60
  jal removeAnimatedUIBox
.org 0x800dbcd0
  jal removeAnimatedUIBox
.org 0x800dc020
  jal removeAnimatedUIBox
.org 0x800dc2c0
  jal removeAnimatedUIBox
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
.org 0x800db3fc
  jal createAnimatedUIBox
.org 0x800db4a8
  jal createAnimatedUIBox
.org 0x800dbe50
  jal createAnimatedUIBox
.org 0x800dc138
  jal createAnimatedUIBox
.org 0x800dc3cc
  jal createAnimatedUIBox
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

.close

.open "work/DIGIMON/STD_REL.BIN",0x80052ae0
.psx

.org 0x80060d30
  jal removeAnimatedUIBox
.org 0x80060e98
  jal removeAnimatedUIBox

.org 0x80061074
  jal createAnimatedUIBox

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

.close