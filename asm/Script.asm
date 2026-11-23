.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

;.org 0x800fb394
;  jal isPartnerBaby
;.org 0x800fc798
;  jal isPartnerBaby
.org 0x8010b808
  jal isPartnerBaby

;.org 0x800e71f8
;  jal showMapHeadTextbox
;.org 0x800fc91c
;  j showMapHeadTextbox
;.org 0x800fd398
;  jal showMapHeadTextbox
;.org 0x800fe5d4
;  jal showMapHeadTextbox
.org 0x80106fe4
  jal showMapHeadTextbox
.org 0x801082e4
  jal showMapHeadTextbox
.org 0x80108628
  jal showMapHeadTextbox
.org 0x80108680
  jal showMapHeadTextbox
.org 0x801092d8
  jal showMapHeadTextbox
.org 0x801095b8
  jal showMapHeadTextbox
.org 0x801098cc
  jal showMapHeadTextbox
.org 0x80109bd0
  jal showMapHeadTextbox
.org 0x80109d34
  jal showMapHeadTextbox
.org 0x8010ba74
  jal showMapHeadTextbox
.org 0x8010bac8
  jal showMapHeadTextbox
.org 0x8010bbcc
  jal showMapHeadTextbox
.org 0x8010bcbc
  jal showMapHeadTextbox
.org 0x8010bd68
  jal showMapHeadTextbox
.org 0x8010bdb0
  jal showMapHeadTextbox
.org 0x8010c024
  jal showMapHeadTextbox
.org 0x8010c0c4
  jal showMapHeadTextbox
.org 0x8010c110
  jal showMapHeadTextbox
.org 0x8010c1c4
  jal showMapHeadTextbox
.org 0x8010c228
  jal showMapHeadTextbox
.org 0x8010c260
  jal showMapHeadTextbox
.org 0x8010c318
  jal showMapHeadTextbox
.org 0x8010c484
  jal showMapHeadTextbox
.org 0x8010c560
  jal showMapHeadTextbox
.org 0x8010c5b4
  jal showMapHeadTextbox
.org 0x8010c680
  jal showMapHeadTextbox
.org 0x8010c698
  jal showMapHeadTextbox
.org 0x8010c6e0
  jal showMapHeadTextbox

;.org 0x800fcfa0
;  jal flipTextboxPage
.org 0x8010a02c
  jal flipTextboxPage

;.org 0x800fca24
;  jal showMapheadSelection
.org 0x80109d80
  jal showMapheadSelection
.org 0x8010bd18
  jal showMapheadSelection
.org 0x8010be08
  jal showMapheadSelection
.org 0x8010beec
  jal showMapheadSelection
.org 0x8010c074
  jal showMapheadSelection
.org 0x8010c388
  jal showMapheadSelection
.org 0x8010c5ec
  jal showMapheadSelection

.org 0x8010c424
  jal getScript

.org 0x8010c434
  jal getScriptSection

.org 0x80106f28
  jal readPStat
.org 0x80107130
  jal readPStat
.org 0x80107668
  jal readPStat
.org 0x801078fc
  jal readPStat
.org 0x801079a0
  jal readPStat
.org 0x80107ba0
  jal readPStat
.org 0x80107c6c
  jal readPStat
.org 0x80107d5c
  jal readPStat
.org 0x801082d0
  jal readPStat
.org 0x80109234
  jal readPStat
.org 0x801092c4
  jal readPStat
.org 0x801095a4
  jal readPStat
.org 0x80109848
  jal readPStat
.org 0x801098b8
  jal readPStat
.org 0x8010b654
  jal readPStat
.org 0x8010b9e4
  jal readPStat
.org 0x8010bb18
  jal readPStat
.org 0x8010bc20
  jal readPStat
.org 0x8010bf78
  jal readPStat
.org 0x8010c148
  jal readPStat
.org 0x8010c29c
  jal readPStat
.org 0x8010c4c0
  jal readPStat
.org 0x8010c658
  jal readPStat
.org 0x801130e4
  jal readPStat

.org 0x80106fd0
  jal writePStat
.org 0x801078a0
  jal writePStat
.org 0x801078cc
  jal writePStat
.org 0x801092bc
  jal writePStat
.org 0x801098b0
  jal writePStat
.org 0x8010b934
  jal writePStat
.org 0x8010b968
  jal writePStat
.org 0x8010b99c
  jal writePStat
.org 0x8010c404
  jal writePStat
.org 0x8010c41c
  jal writePStat
.org 0x8010c54c
  jal writePStat
.org 0x8010c890
  jal writePStat
.org 0x8010c8a8
  jal writePStat
.org 0x801130fc
  jal writePStat
.org 0x80113400
  jal writePStat

.org 0x80107790
  jal isTriggerSet
.org 0x80107af4
  jal isTriggerSet
.org 0x80107e34
  jal isTriggerSet

.org 0x80107e24
  jal setTrigger
.org 0x80107e54
  jal setTrigger
.org 0x8010c7e0
  jal setTrigger

.org 0x8010cb18
  jal unsetTrigger

.org 0x80106f34
  jal getCardAmount
.org 0x801070a0
  jal getCardAmount
.org 0x80107244
  jal getCardAmount
.org 0x8010be74
  jal getCardAmount
  
.org 0x80106fb4
  jal setCardAmount
.org 0x8010be8c
  jal setCardAmount

.org 0x8010c1f0
  jal playBGM

.close


.open "work/DIGIMON/BTL_REL.BIN",0x80052ae0
.psx

.org 0x80056d2c
  jal isTriggerSet

.close


.open "work/DIGIMON/STD_REL.BIN",0x80052ae0
.psx

.org 0x80056e6c
  jal scriptLoadModel

.close


.open "work/DIGIMON/TRN_REL.BIN",0x80088800
.psx

.org 0x8008a370
  jal dailyPStatTrigger

.org 0x8008ab48
  jal readPStat
.org 0x8008b1c4
  jal readPStat
.org 0x8008b7f8
  jal readPStat
.org 0x8008c1a0
  jal readPStat
.org 0x8008caf0
  jal readPStat
.org 0x8008cfc0
  jal readPStat

.org 0x8008a0f4
  jal isTriggerSet
.org 0x8008a140
  jal isTriggerSet

.close

  
.open "work/DIGIMON/DOOA_REL.BIN",0x80080000
.psx

.org 0x800847cc
  jal readPStat

.org 0x800847a4
  jal isTriggerSet
.org 0x800847b8
  jal isTriggerSet

.close


.open "work/DIGIMON/MURD_REL.BIN",0x8007C000
.psx

.org 0x8007d6d0
  jal readPStat

.org 0x8007d6a8
  jal isTriggerSet
.org 0x8007d6bc
  jal isTriggerSet

.close


.open "work/DIGIMON/TRN2_REL.BIN",0x80088800
.psx

.org 0x8008bb6c
  jal dailyPStatTrigger

.org 0x800891f4
  jal readPStat
.org 0x80089720
  jal readPStat
.org 0x80089b3c
  jal readPStat
.org 0x8008a214
  jal readPStat
.org 0x8008a6cc
  jal readPStat

.close

.open "work/DIGIMON/DGET_REL.BIN",0x80080800
.psx

.org 0x80080a6c
  jal showMapHeadTextbox
.org 0x80081bac
  jal showMapHeadTextbox
.org 0x80081bd8
  jal showMapHeadTextbox

.org 0x80081c18
  jal showMapheadSelection

.org 0x80080f54
  jal showTextboxReady

.org 0x8008112c
  jal getScript

.org 0x8008113c
  jal getScriptSection

.org 0x800809f0
  jal readPStat
.org 0x80080b74
  jal readPStat
.org 0x80080dec
  jal readPStat
.org 0x80081b5c
  jal readPStat
.org 0x80081b98
  jal readPStat
.org 0x80081bc4
  jal readPStat

.org 0x80081408
  jal writePStat
.org 0x80081af4
  jal writePStat
.org 0x80081c84
  jal writePStat
.org 0x80081c98
  jal writePStat

.org 0x80081074
  jal isTriggerSet
.org 0x800810b4
  jal isTriggerSet
.org 0x80081118
  jal isTriggerSet
.org 0x80081154
  jal isTriggerSet
.org 0x800811b0
  jal isTriggerSet
.org 0x80081abc
  jal isTriggerSet

.org 0x80081bb4
  jal setTrigger
.org 0x80081c40
  jal setTrigger
.org 0x80081c48
  jal setTrigger
.org 0x80081c50
  jal setTrigger

.org 0x80081be0
  jal unsetTrigger

.close


.open "work/DIGIMON/EVL_REL.BIN",0x80060000
.psx

.org 0x80060ff0
  jal forceUpdateBGM

.close
