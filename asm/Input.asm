.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x80105a80
  jal inputInit
;.org 0x800fabb0
;  jal isKeyDown
;.org 0x800fac48
;  jal isKeyDown
;.org 0x800faccc
;  jal isKeyDown
;.org 0x800fad18
;  jal isKeyDown
;.org 0x800fad64
;  jal isKeyDown
;.org 0x800fb098
;  jal isKeyDown
;.org 0x800fb190
;  jal isKeyDown
;.org 0x800fb224
;  jal isKeyDown
;.org 0x800fb2ac
;  jal isKeyDown
;.org 0x800fb2d8
;  jal isKeyDown
;.org 0x800fb304
;  jal isKeyDown
;.org 0x800fb900
;  jal isKeyDown
;.org 0x800fb92c
;  jal isKeyDown
;.org 0x800fb954
;  jal isKeyDown
;.org 0x800fb980
;  jal isKeyDown
.org 0x800ffc54
  jal isKeyDown
.org 0x800ffc88
  jal isKeyDown
.org 0x800ffd28
  jal isKeyDown
.org 0x800ffd88
  jal isKeyDown
.org 0x800ffe68
  jal isKeyDown
.org 0x80100018
  jal isKeyDown
.org 0x80100068
  jal isKeyDown
.org 0x80107edc
  jal isKeyDown
.org 0x80107fa8
  jal isKeyDown
.org 0x80107ff0
  jal isKeyDown
.org 0x8010803c
  jal isKeyDown
.org 0x801083ec
  jal isKeyDown
.org 0x80108418
  jal isKeyDown
.org 0x80108440
  jal isKeyDown
.org 0x8010847c
  jal isKeyDown
.org 0x801084a8
  jal isKeyDown
.org 0x801084d4
  jal isKeyDown
.org 0x80108500
  jal isKeyDown
.org 0x80108550
  jal isKeyDown
.org 0x801085a0
  jal isKeyDown
.org 0x80109284
  jal isKeyDown
.org 0x80109314
  jal isKeyDown
.org 0x8010933c
  jal isKeyDown
.org 0x8010938c
  jal isKeyDown
.org 0x80109558
  jal isKeyDown
.org 0x80109614
  jal isKeyDown
.org 0x80109640
  jal isKeyDown
.org 0x80109690
  jal isKeyDown
.org 0x8010985c
  jal isKeyDown
.org 0x8010990c
  jal isKeyDown
.org 0x80109938
  jal isKeyDown
.org 0x8010998c
  jal isKeyDown
.org 0x8010a0bc
  jal isKeyDown
.org 0x8010a0f4
  jal isKeyDown
.org 0x8010a2f8
  jal isKeyDown
.org 0x8010a324
  jal isKeyDown
.org 0x8010a344
  jal isKeyDown
.org 0x8010a370
  jal isKeyDown
.org 0x8010a39c
  jal isKeyDown
.org 0x8010a3c8
  jal isKeyDown
.org 0x8010a3ec
  jal isKeyDown

;.org 0x800fab80
;  jal isXPressedAfterDialogue
;.org 0x800fb088
;  jal isXPressedAfterDialogue
;.org 0x800fb16c
;  jal isXPressedAfterDialogue
;.org 0x800fb8f0
;  jal isXPressedAfterDialogue
.org 0x800ffc3c
  jal isXPressedAfterDialogue
.org 0x800ffe2c
  jal isXPressedAfterDialogue
.org 0x80100008
  jal isXPressedAfterDialogue
.org 0x80107ebc
  jal isXPressedAfterDialogue
.org 0x801083ac
  jal isXPressedAfterDialogue
.org 0x80109224
  jal isXPressedAfterDialogue
.org 0x80109538
  jal isXPressedAfterDialogue
.org 0x80109838
  jal isXPressedAfterDialogue
.org 0x8010a064
  jal isXPressedAfterDialogue

.org 0x801007bc
  jal setFreshDialogue

.org 0x800fc6a0
  jal setInputRepeatMask
.org 0x800fc6e0
  jal setInputRepeatMask
.org 0x800fc778
  jal setInputRepeatMask
.org 0x800fc7f0
  jal setInputRepeatMask
.org 0x800fcbf4
  jal setInputRepeatMask
.org 0x800fcc18
  jal setInputRepeatMask
.org 0x8010b768
  jal setInputRepeatMask
.org 0x8010b7f8
  jal setInputRepeatMask
.org 0x8010b878
  jal setInputRepeatMask
.org 0x8010b8b0
  jal setInputRepeatMask
.org 0x8010baa4
  jal setInputRepeatMask
.org 0x8010bae4
  jal setInputRepeatMask
.org 0x8010bba8
  jal setInputRepeatMask
.org 0x8010bbe8
  jal setInputRepeatMask
.org 0x8010bcf4
  jal setInputRepeatMask
.org 0x8010bd3c
  jal setInputRepeatMask
.org 0x8010bd84
  jal setInputRepeatMask
.org 0x8010bdec
  jal setInputRepeatMask
.org 0x8010beb0
  jal setInputRepeatMask
.org 0x8010bed0
  jal setInputRepeatMask
.org 0x8010c098
  jal setInputRepeatMask
.org 0x8010c0e0
  jal setInputRepeatMask
.org 0x8010c20c
  jal setInputRepeatMask
.org 0x8010c244
  jal setInputRepeatMask
.org 0x8010c358
  jal setInputRepeatMask
.org 0x8010c36c
  jal setInputRepeatMask
.org 0x8010c460
  jal setInputRepeatMask
.org 0x8010c598
  jal setInputRepeatMask
.org 0x8010c5d0
  jal setInputRepeatMask
.org 0x8010c6c4
  jal setInputRepeatMask
.org 0x8010c920
  jal setInputRepeatMask
.org 0x8010c948
  jal setInputRepeatMask

.close

.open "work/DIGIMON/DGET_REL.BIN",0x80080800
.psx

.org 0x80081380
  jal isKeyDown
.org 0x800813ac
  jal isKeyDown
.org 0x8008143c
  jal isKeyDown
.org 0x80081484
  jal isKeyDown
.org 0x800814c8
  jal isKeyDown
.org 0x80081510
  jal isKeyDown
.org 0x80081554
  jal isKeyDown
.org 0x80081960
  jal isKeyDown

.org 0x80081370
  jal isXPressedAfterDialogue
.org 0x80081950
  jal isXPressedAfterDialogue

.org 0x80081b0c
  jal setInputRepeatMask
.org 0x80081b44
  jal setInputRepeatMask

.close
