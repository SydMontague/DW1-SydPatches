.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x800a0a50
.area 0x800a0e58-.
  .importobj "compiled/Model.lib"

  .notice "Empty space left: " + (0x800a0e58-.) + " bytes"
  .fill 0x800a0e58-.
.endarea

.org 0x800a1134
  jal initializePosData
.org 0x800a10ac
  jal initializePosData

.org 0x800a13b0
  jal renderFlatDigimon

.close