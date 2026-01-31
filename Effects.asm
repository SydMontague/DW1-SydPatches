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

;.org 0x800e9e30
;  jal translateConditionFXToEntity
;.org 0x800ec684
;  jal translateConditionFXToEntity
.org 0x800fbedc
  jal translateConditionFXToEntity

;.org 0x800ae5b0
;  jal renderSprite
;.org 0x800e9f98
;  jal renderSprite
;.org 0x800ec7c4
;  jal renderSprite
;.org 0x800ec898
;  jal renderSprite
;.org 0x800ec970
;  jal renderSprite
;.org 0x800ec9e4
;  jal renderSprite
;.org 0x800eca40
;  jal renderSprite
;.org 0x800eca98
;  jal renderSprite
;.org 0x800ecb0c
;  jal renderSprite
;.org 0x800ecbd0
;  jal renderSprite
;.org 0x800ecc0c
;  jal renderSprite
;.org 0x800ecc40
;  jal renderSprite

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

.org 0x8006eec4
  jal translateConditionFXToEntity
.org 0x8006f2a0
  jal translateConditionFXToEntity

.org 0x8006914c
  jal addScreenPolyFT4
.org 0x8007102c
  jal addScreenPolyFT4
.org 0x80072194
  jal addScreenPolyFT4

.org 0x80070854
  jal addScreenPolyFT3
.org 0x80072a14
  jal addScreenPolyFT3

.org 0x8006eff8
  jal renderSprite

.org 0x8006f378
  jal renderTMDModel
.org 0x8006fda0
  jal renderTMDModel
.org 0x800703b4
  jal renderTMDModel
.org 0x800703e0
  jal renderTMDModel
.org 0x80070450
  jal renderTMDModel
.org 0x80071ce8
  jal renderTMDModel
.org 0x800726b0
  jal renderTMDModel

.org 0x80065e0c
  jal renderFXParticle
.org 0x80071648
  jal renderFXParticle

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

.org 0x80077464
  jal translateConditionFXToEntity
.org 0x80077840
  jal translateConditionFXToEntity

.org 0x80078df4
  jal addScreenPolyFT3
.org 0x800716e4
  jal addScreenPolyFT4
.org 0x800795cc
  jal addScreenPolyFT4

.org 0x80077598
  jal renderSprite

.org 0x80077918
  jal renderTMDModel
.org 0x80078340
  jal renderTMDModel
.org 0x80078954
  jal renderTMDModel
.org 0x80078980
  jal renderTMDModel
.org 0x800789f0
  jal renderTMDModel

.org 0x8006e4f4
  jal renderFXParticle

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

.org 0x8006d534
  jal translateConditionFXToEntity
.org 0x8006d910
  jal translateConditionFXToEntity

.org 0x8006eec4
  jal addScreenPolyFT3
.org 0x800677b4
  jal addScreenPolyFT4
.org 0x8006f69c
  jal addScreenPolyFT4

.org 0x8006d668
  jal renderSprite

.org 0x8006d9e8
  jal renderTMDModel
.org 0x8006e410
  jal renderTMDModel
.org 0x8006ea24
  jal renderTMDModel
.org 0x8006ea50
  jal renderTMDModel
.org 0x8006eac0
  jal renderTMDModel

.org 0x800645c4
  jal renderFXParticle

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

.org 0x80083718
  jal addScreenPolyFT3

.org 0x80083b90
  jal addScreenPolyFT4

.close

.open "work/DIGIMON/DOO2_REL.BIN",0x80070000
.psx

.org 0x80070520
  jal addScreenPolyFT3
.org 0x800713ec
  jal addScreenPolyFT3

.org 0x8007087c
  jal addScreenPolyFT4

.close

.open "work/DIGIMON/MURD_REL.BIN",0x8007C000
.psx

.org 0x8007cfdc
  jal renderParticleFlash

.close

.open "work/DIGIMON/EVL_REL.BIN",0x80060000
.psx

.org 0x800627e8
  jal addScreenPolyFT3
.org 0x80063028
  jal addScreenPolyFT3

.org 0x80062aac
  jal addScreenPolyFT4

.close

.open "work/DIGIMON/EAB_REL.BIN",0x80060000
.psx

.org 0x800603b4
  jal renderTMDModel
.org 0x80060604
  jal renderTMDModel

.close

.open "work/DIGIMON/ENDI_REL.BIN",0x80060000
.psx

.org 0x80060cd0
  jal renderFXParticle

.close