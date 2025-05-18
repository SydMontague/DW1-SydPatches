.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x800f03a0
  jal initializeCloudFXData

.org 0x800f0390
  jal initializeParticleFX

.org 0x800f0398
  jal initializeEntityParticleFX

.org 0x800f03a8
  jal initializeHealingParticles

.org 0x801042cc
  jal createMeramonShake

.close

.open "work/DIGIMON/BTL_REL.BIN",0x80052ae0
.psx

.org 0x8006c9e0
  jal createCloudFX

.org 0x8006bfa4
  j addEntityParticleFX

.org 0x8005c774
  jal createParticleFX
.org 0x8005cca8
  jal createParticleFX

.org 0x8006c63c
  jal renderParticleFlash

.org 0x80069d3c
  jal getRViewCopy

.org 0x80069d44
  jal getViewportDistanceCopy

.org 0x8006648c
  jal getDrawingOffsetCopy
.org 0x800693e0
  jal getDrawingOffsetCopy
.org 0x80069d50
  jal getDrawingOffsetCopy
.org 0x8006c8b4
  jal getDrawingOffsetCopy
.org 0x8006e9a8
  jal getDrawingOffsetCopy

.close


.open "work/DIGIMON/STD_REL.BIN",0x80052ae0
.psx

.org 0x80074fbc
  jal createCloudFX

.org 0x800745b0
  j addEntityParticleFX

.org 0x800647d4
  jal createParticleFX
.org 0x80064d38
  jal createParticleFX

.org 0x80074c48
  jal renderParticleFlash

.close


.open "work/DIGIMON/VS_REL.BIN",0x80052ae0
.psx

.org 0x8006b08c
  jal createCloudFX

.org 0x8006a680
  j addEntityParticleFX

.org 0x8005c3bc
  jal createParticleFX
.org 0x8005c88c
  jal createParticleFX

.org 0x8006ad18
  jal renderParticleFlash

.close


.open "work/DIGIMON/TRN_REL.BIN",0x80088800
.psx

.org 0x800889c4
  jal createCloudFX
.org 0x8008c458
  jal createCloudFX
.org 0x8008c520
  jal createCloudFX

.org 0x8008b090
  jal removeAllCloudFX

.org 0x8008ba2c
  jal createParticleFX
.org 0x8008ba48
  jal createParticleFX
.org 0x8008ba64
  jal createParticleFX
.org 0x8008be68
  jal createParticleFX
.org 0x8008be84
  jal createParticleFX
.org 0x8008bea0
  jal createParticleFX
.org 0x8008c384
  jal createParticleFX
.org 0x8008c3a0
  jal createParticleFX
.org 0x8008c3bc
  jal createParticleFX

.close


.open "work/DIGIMON/TRN2_REL.BIN",0x80088800
.psx

.org 0x8008965c
  jal removeAllCloudFX
.org 0x80088c8c
  jal removeAllCloudFX

.org 0x80088e48
  jal createParticleFX
.org 0x80089d40
  jal createParticleFX
.org 0x80089d5c
  jal createParticleFX
.org 0x80089d78
  jal createParticleFX
.org 0x8008a3b0
  jal createParticleFX
.org 0x8008a3cc
  jal createParticleFX
.org 0x8008a3e8
  jal createParticleFX

.close

.open "work/DIGIMON/DOOA_REL.BIN",0x80080000
.psx

.org 0x80081cb4
  jal renderParticleFlash

.close

.open "work/DIGIMON/MURD_REL.BIN",0x8007C000
.psx

.org 0x8007cfdc
  jal renderParticleFlash

.close