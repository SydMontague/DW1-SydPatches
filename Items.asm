.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x800c566c
.area 0x800c5ad8-.
  .importobj "compiled/ItemEffects.lib"

  .notice "Empty space left: " + (0x800c5ad8-.) + " bytes"
.endarea

.org 0x80102d04
  jal setInventorySize

.close
