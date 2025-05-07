.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x800ef8e0
  jal setImmortalHour

.org 0x800dd99c
  jal tickConditionBoundaries
;.org 0x800abc28
;  jal tickConditionBoundaries
;.org 0x800a8fc8
;  jal tickConditionBoundaries

;.org 0x800a8fa0
;  jal tickSleepMechanics

;.org 0x800a8fc0
;  jal tickUnhappinessMechanics

.org 0x800dde34
  jal handlePraiseScold

.org 0x800ddbd4
  jal handleSleeping

.org 0x800de63c
  jal createPoopPile

.org 0x800ddbcc
  jal sleepRegen

;.org 0x800a8fb0
;  jal tickTirednessMechanics

.org 0x800de0b8
  jal partnerWillRefuseItem

;.org 0x800a8fb8
;  jal tickHungerMechanics

.org 0x800dd944
  jal handleConditionBubble
.org 0x800dd9f0
  jal handleConditionBubble

.org 0x800de210
  jal partnerHandleFoodFeed

.org 0x800dd9e0
  jal tickPartnerPoopingMechanic

.org 0x800dd9e8
  jal detectEdiblePoop

.org 0x800de4b4
  jal handleToilet

.org 0x800de648
  jal handleWildPoop

.org 0x800de9e8
  jal handleEatingPoop

;.org 0x800a8fa8
;  jal tickSicknessMechanics

;.org 0x800a8f98
;  jal tickDeathCondition

.org 0x8010334c
  jal skipHours

.org 0x800dd9d8
  jal tickPartner

.org 0x800e3644
  jal initializeReincarnatedPartner

.org 0x800d9328
  li.u a3,renderPoop
.org 0x800d933c
  li.l a3,renderPoop

.org 0x801053cc
  jal setFoodTimer

.org 0x800ee9f4
  jal initializePartner
.org 0x800eeb38
  jal initializePartner
.org 0x800eea7c
  jal initializePartner

;.org 0x800d415c
;  jal initializePartnerWaypoint
;.org 0x800d4180
;  jal initializePartnerWaypoint
;.org 0x800d5dbc
;  jal initializePartnerWaypoint

.org 0x800dd9c8
  jal tickPartnerCollision
.org 0x800e7734
  jal tickPartnerCollision
.org 0x800e7834
  jal tickPartnerCollision
.org 0x800e788c
  jal tickPartnerCollision
.org 0x800e78c0
  jal tickPartnerCollision
.org 0x800e78ec
  jal tickPartnerCollision
.org 0x800e7918
  jal tickPartnerCollision
.org 0x800e794c
  jal tickPartnerCollision
.org 0x800e7b10
  jal tickPartnerCollision

.org 0x800dda78
  jal tickPartnerWaypoints
.org 0x800ddd48
  jal tickPartnerWaypoints
.org 0x800ddf6c
  jal tickPartnerWaypoints
.org 0x800de95c
  jal tickPartnerWaypoints
.org 0x800e7624
  jal tickPartnerWaypoints

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
  jal tickPartnerCollision

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

.close


.open "work/DIGIMON/TRN2_REL.BIN",0x80088800
.psx

.org 0x8008be8c
  jal tickConditionBoundaries

.close