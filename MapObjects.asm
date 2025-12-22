.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x8010418c
  jal loadDirtCartModel

.org 0x8010419c
  jal loadDirtPileModel

.org 0x800eea0c
  jal initializeChest
.org 0x800eea8c
  jal initializeChest
.org 0x800eeb48
  jal initializeChest

.org 0x80104db8
  jal spawnBoulder

.org 0x801044c4
  jal spawnToyTownBoxes

.org 0x80104498
  jal spawnGearbox

.org 0x80104528
  jal spawnAngemonPedestal

.org 0x80104da0
  jal spawnChest

.close