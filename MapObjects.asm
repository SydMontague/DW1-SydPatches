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

.org 0x801058ac
  jal tickRotateDoor

.org 0x80105e14
  jal tickOpenChestTray

.org 0x80105e34
  jal tickCloseChestTray

.org 0x80104108
  jal decreaseDirtPileSize

.org 0x80104de4
  jal moveBoulder

.org 0x801044e0
  jal openToyTownBox

.org 0x801044b4
  jal somethingToyTown

.org 0x80105ebc
  jal moveAngemonPedestal

.org 0x801040f0
  jal setDirtCartModel

.close