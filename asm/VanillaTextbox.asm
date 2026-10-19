.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x80100390
  jal drawTextboxStrings

.org 0x80100634
  jal clearTextboxLineCount
  
.close
