.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x800c566c
.area 0x800c5ad8-.
  .importobj "compiled/ItemEffects.lib"

  .notice "Empty space left: " + (0x800c5ad8-.) + " bytes"
.endarea

.org 0x800c396c
.area 0x800c4728-.
  .importobj "compiled/ItemFunctions.lib"

  .notice "Empty space left: " + (0x800c4728-.) + " bytes"
.endarea
.close
