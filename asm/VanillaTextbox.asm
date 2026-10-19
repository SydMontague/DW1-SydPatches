.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x80100390
  jal drawTextboxStrings

.org 0x80100634
  jal clearTextboxLineCount

.org 0x80101178
  jal tickSelectionDialogue
.org 0x80101254
  jal tickSelectionDialogue

.org 0x801011d4
  jal tickConfirmDialogue
.org 0x80101228
  jal tickConfirmDialogue

.org 0x8010128c
  jal tickBackgroundDialogue
  
.close
