.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x80105f30
  jal getScriptSyncBit
.org 0x80105f78
  jal getScriptSyncBit

.org 0x80105dec
  jal scriptStartTournament

.org 0x80103eb8
  jal checkTournamentMedalConditions

.org 0x80105dfc
  jal scriptCheckTournamentMedal

.org 0x80104640
  jal handleItemLoss

;.org 0x800dd1d8
;  jal dailyPStatTrigger
;.org 0x800dd6e0
;  jal dailyPStatTrigger
.org 0x801033c0
  jal dailyPStatTrigger
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
.org 0x800fe5d4
  jal showMapHeadTextbox
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

.close
