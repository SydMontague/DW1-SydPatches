.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x80103eb8
  jal checkTournamentMedalConditions

.org 0x80104640
  jal handleItemLoss

;.org 0x800dd1d8
;  jal dailyPStatTrigger
;.org 0x800dd6e0
;  jal dailyPStatTrigger
;.org 0x801033c0
;  jal dailyPStatTrigger
.org 0x80105a88
  jal dailyPStatTrigger

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

.org 0x801042dc
  jal loadShopLibrary

;.org 0x800fbd94
;  jal scriptIdToEntityId
;.org 0x800fbeb0
;  jal scriptIdToEntityId
;.org 0x800ff89c
;  jal scriptIdToEntityId
;.org 0x801010e8
;  jal scriptIdToEntityId
;.org 0x80101fa4
;  jal scriptIdToEntityId
.org 0x8010383c
  jal scriptIdToEntityId
.org 0x801038e0
  jal scriptIdToEntityId
.org 0x80103948
  jal scriptIdToEntityId
.org 0x801039d4
  jal scriptIdToEntityId
.org 0x80103aec
  jal scriptIdToEntityId
.org 0x80103b78
  jal scriptIdToEntityId
.org 0x80103c04
  jal scriptIdToEntityId
.org 0x80104408
  jal scriptIdToEntityId
.org 0x801045cc
  jal scriptIdToEntityId
.org 0x80104880
  jal scriptIdToEntityId
.org 0x80104908
  jal scriptIdToEntityId
.org 0x80104994
  jal scriptIdToEntityId
.org 0x80104a20
  jal scriptIdToEntityId
.org 0x80104aac
  jal scriptIdToEntityId
.org 0x80104c20
  jal scriptIdToEntityId
.org 0x80104cb4
  jal scriptIdToEntityId
.org 0x80106378
  jal scriptIdToEntityId
.org 0x80106408
  jal scriptIdToEntityId

.org 0x8010525c
  jal scriptCompareStat

.org 0x8010526c
  jal scriptCompareCard

.org 0x8010527c
  jal scriptCompareMove

.org 0x8010528c
  jal scriptCompareCondition

.org 0x8010529c
  jal scriptCompareItemCount

.org 0x801052ac
  jal scriptCompareMoney

;.org 0x800efc14
;  jal tickScript
.org 0x80105afc
  jal tickScript

.close


.open "work/DIGIMON/TRN_REL.BIN",0x80088800
.psx

.org 0x8008a370
  jal dailyPStatTrigger

.close


.open "work/DIGIMON/TRN2_REL.BIN",0x80088800
.psx

.org 0x8008bb6c
  jal dailyPStatTrigger

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

.close
