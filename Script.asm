.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x80105f30
  jal getScriptSyncBit
.org 0x80105f78
  jal getScriptSyncBit

.close