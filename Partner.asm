.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x800ef8e0
  jal setImmortalHour

;.org 0x800a8fa8
;  jal tickSicknessMechanics

;.org 0x800a8f98
;  jal tickDeathCondition

.org 0x8010334c
  jal skipHours

.org 0x801053cc
  jal setFoodTimer

.org 0x800ee9f4
  jal initializePartner
.org 0x800eeb38
  jal initializePartner
.org 0x800eea7c
  jal initializePartner

;.org 0x800dd9c8
;  jal Partner_tickCollision
.org 0x800e7734
  jal Partner_tickCollision
.org 0x800e7834
  jal Partner_tickCollision
.org 0x800e788c
  jal Partner_tickCollision
.org 0x800e78c0
  jal Partner_tickCollision
.org 0x800e78ec
  jal Partner_tickCollision
.org 0x800e7918
  jal Partner_tickCollision
.org 0x800e794c
  jal Partner_tickCollision
.org 0x800e7b10
  jal Partner_tickCollision

;.org 0x800dda78
;  jal tickPartnerWaypoints
;.org 0x800ddd48
;  jal tickPartnerWaypoints
;.org 0x800ddf6c
;  jal tickPartnerWaypoints
;.org 0x800de95c
;  jal tickPartnerWaypoints
.org 0x800e7624
  jal tickPartnerWaypoints

;.org 0x800df624
;  jal Partner_setState
;.org 0x800df644
;  jal Partner_setState
;.org 0x800df660
;  jal Partner_setState
;.org 0x800df670
;  jal Partner_setState
;.org 0x800df680
;  jal Partner_setState
;.org 0x800df698
;  jal Partner_setState
;.org 0x800df6b0
;  jal Partner_setState
;.org 0x800df6c8
;  jal Partner_setState
;.org 0x800df744
;  jal Partner_setState
;.org 0x800df75c
;  jal Partner_setState
;.org 0x800df7dc
;  jal Partner_setState
.org 0x801054b0
  jal Partner_setState
.org 0x80105528
  jal Partner_setState

.org 0x800e6ddc
  jal Partner_getState

.org 0x801054a8
  jal Partner_startAnimation
.org 0x80105520
  jal Partner_startAnimation
.org 0x80105984
  jal Partner_startAnimation

.org 0x80103668
  jal callDigimonRoutine

.org 0x800ed400
  jal hasMove
.org 0x800fe444
  jal hasMove
.org 0x800fe554
  jal hasMove
.org 0x80106d20
  j hasMove

.org 0x800ed580
  jal learnMove
.org 0x801067e4
  j learnMove

.org 0x800fe5ac
  jal unlearnMove

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