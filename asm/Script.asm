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

.org 0x800dd1d8
  jal dailyPStatTrigger
.org 0x800dd6e0
  jal dailyPStatTrigger
.org 0x801033c0
  jal dailyPStatTrigger
.org 0x80105a88
  jal dailyPStatTrigger

;.org 0x800fb394
;  jal isPartnerBaby
.org 0x800fc798
  jal isPartnerBaby
.org 0x8010b808
  jal isPartnerBaby

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
