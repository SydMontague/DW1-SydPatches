
.open "work/DIGIMON/FISH_REL.BIN",0x80070000
.psx

.org 0x80079ab8
  jal getBestFishingRod

.org 0x80077ec0
  jal hasFoodItem
  
.org 0x80079c48
  jal isFishing

.close