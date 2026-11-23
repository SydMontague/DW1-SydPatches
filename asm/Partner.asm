.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.close 

.open "work/DIGIMON/BTL_REL.BIN",0x80052ae0
.psx

.org 0x80058c14
  jal initializePartnerWaypoint
.org 0x80058c38
  jal initializePartnerWaypoint

.org 0x80058c04
  jal setPartnerWaypoint
.org 0x80058c28
  jal setPartnerWaypoint

.org 0x80057780
  jal Partner_tickCollision

.close

.open "work/DIGIMON/EVL_REL.BIN",0x80060000
.psx

.org 0x80063ac4
  jal initializeEvolvedPartner

.close

.open "work/DIGIMON/TRN_REL.BIN",0x80088800
.psx

.org 0x8008a6e0
  jal tickConditionBoundaries


.org 0x8008ac28
  jal Partner_setState
.org 0x8008b064
  jal Partner_setState
.org 0x8008b24c
  jal Partner_setState
.org 0x8008b5a8
  jal Partner_setState
.org 0x8008b888
  jal Partner_setState
.org 0x8008bdc8
  jal Partner_setState
.org 0x8008c244
  jal Partner_setState
.org 0x8008c9c0
  jal Partner_setState
.org 0x8008cb68
  jal Partner_setState
.org 0x8008ce48
  jal Partner_setState
.org 0x8008d03c
  jal Partner_setState
.org 0x8008d48c
  jal Partner_setState

.org 0x8008a944
  jal learnMove
.org 0x8008a800
  jal hasMove

.close


.open "work/DIGIMON/TRN2_REL.BIN",0x80088800
.psx

.org 0x8008be8c
  jal tickConditionBoundaries

.org 0x800888a0
  jal Partner_setState
.org 0x80088c60
  jal Partner_setState
.org 0x80088d20
  jal Partner_setState
.org 0x80089100
  jal Partner_setState
.org 0x8008927c
  jal Partner_setState
.org 0x80089630
  jal Partner_setState
.org 0x8008978c
  jal Partner_setState
.org 0x80089a2c
  jal Partner_setState
.org 0x80089bcc
  jal Partner_setState
.org 0x80089fc8
  jal Partner_setState
.org 0x8008a284
  jal Partner_setState
.org 0x8008a61c
  jal Partner_setState
.org 0x8008a748
  jal Partner_setState
.org 0x8008aa08
  jal Partner_setState

.close

.open "work/DIGIMON/MURD_REL.BIN",0x8007C000
.psx

.org 0x8007c3f8
  jal Partner_setState

.close


.open "work/DIGIMON/EVL_REL.BIN",0x80060000
.psx

.org 0x80063944
  jal learnMove

.close


.open "work/DIGIMON/DOOA_REL.BIN",0x80080000
.psx

.org 0x80082534
  jal reincarnatePartner

.close
