
.open "work/DIGIMON/DOOA_REL.BIN",0x80080000
.psx

.org 0x800800b8
  jal DOOA_storeDigimonY
.org 0x800806f8
  jal DOOA_storeDigimonY
  
.org 0x80080814
  jal DOOA_getStoredDigimonY
.org 0x80083224
  jal DOOA_getStoredDigimonY
.org 0x800835e8
  jal DOOA_getStoredDigimonY
.org 0x8008365c
  jal DOOA_getStoredDigimonY
.org 0x800836d0
  jal DOOA_getStoredDigimonY
.org 0x800839e4
  jal DOOA_getStoredDigimonY
.org 0x80083a58
  jal DOOA_getStoredDigimonY
.org 0x80083acc
  jal DOOA_getStoredDigimonY
.org 0x80083b40
  jal DOOA_getStoredDigimonY
.org 0x800840a4
  jal DOOA_getStoredDigimonY
.org 0x80084524
  jal DOOA_getStoredDigimonY

.org 0x80080230
  jal DOOA_setSomeDyingState
.org 0x80080240
  jal DOOA_setSomeDyingState

.close