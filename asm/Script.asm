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

.close