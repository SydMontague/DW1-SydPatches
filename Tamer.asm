.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x800de200
  jal removeOneSelectedItem

.org 0x800de07c
  li.u a3,renderFeedingItem
.org 0x800de090
  li.l a3,renderFeedingItem

.org 0x80105dc4
  jal isTrainingComplete

.org 0x800de6a8
  jal addTamerLevel
;.org 0x800dd254
;  jal addTamerLevel
;.org 0x800dd238
;  jal addTamerLevel
;.org 0x800c46f4
;  jal addTamerLevel
;.org 0x800a5db0
;  jal addTamerLevel


;.org 0x800a8924	
;  jal Tamer_setState
;.org 0x800a9348	
;  jal Tamer_setState
;.org 0x800a9cf4	
;  jal Tamer_setState
;.org 0x800a9e68	
;  jal Tamer_setState
;.org 0x800aa288	
;  jal Tamer_setState
;.org 0x800aa618	
;  jal Tamer_setState
;.org 0x800aaa20	
;  jal Tamer_setState
;.org 0x800aac50	
;  jal Tamer_setState
;.org 0x800ab1a4	
;  jal Tamer_setState
;.org 0x800ab32c	
;  jal Tamer_setState
;.org 0x800ab580	
;  jal Tamer_setState
;.org 0x800ab654	
;  jal Tamer_setState
;.org 0x800ab908	
;  jal Tamer_setState
;.org 0x800b7afc	
;  jal Tamer_setState
;.org 0x800b7b0c	
;  jal Tamer_setState
;.org 0x800b7ba0	
;  jal Tamer_setState
;.org 0x800b7bb8	
;  jal Tamer_setState
;.org 0x800b8e24	
;  jal Tamer_setState
;.org 0x800ba3f0	
;  jal Tamer_setState
;.org 0x800ba438	
;  jal Tamer_setState
;.org 0x800c394c	
;  jal Tamer_setState
;.org 0x800c5a7c	
;  jal Tamer_setState
;.org 0x800d8f4c	
;  jal Tamer_setState
;.org 0x800d8f74	
;  jal Tamer_setState
;.org 0x800d9340	
;  jal Tamer_setState
;.org 0x800dda70	
;  jal Tamer_setState
;.org 0x800ddccc	
;  jal Tamer_setState
;.org 0x800dddfc	
;  jal Tamer_setState
;.org 0x800dde0c	
;  jal Tamer_setState
;.org 0x800dde90	
;  jal Tamer_setState
.org 0x800de1b4	
  jal Tamer_setState
.org 0x800de290	
  jal Tamer_setState
.org 0x800de334	
  jal Tamer_setState
.org 0x800de584	
  jal Tamer_setState
.org 0x800de68c	
  jal Tamer_setState
.org 0x800de7cc	
  jal Tamer_setState
.org 0x800de93c	
  jal Tamer_setState
.org 0x800dea08	
  jal Tamer_setState
.org 0x800debf0	
  jal Tamer_setState
.org 0x800df5a8	
  jal Tamer_setState
.org 0x800df63c	
  jal Tamer_setState
.org 0x800df690	
  jal Tamer_setState
.org 0x800df6a8	
  jal Tamer_setState
.org 0x800df6c0	
  jal Tamer_setState
.org 0x800df73c	
  jal Tamer_setState
.org 0x800df754	
  jal Tamer_setState
;.org 0x800e34cc	
;  jal Tamer_setState
.org 0x800eeac4	
  jal Tamer_setState
.org 0x800eeb80	
  jal Tamer_setState
.org 0x80104440	
  jal Tamer_setState
.org 0x80105488	
  jal Tamer_setState
.org 0x80105504	
  jal Tamer_setState

;.org 0x800a7d70
;  jal Tamer_getState
;.org 0x800a8898
;  jal Tamer_getState
;.org 0x800a89bc
;  jal Tamer_getState
;.org 0x800a8a88
;  jal Tamer_getState
;.org 0x800a9160
;  jal Tamer_getState
;.org 0x800b69fc
;  jal Tamer_getState
;.org 0x800d6f88
;  jal Tamer_getState
;.org 0x800d7130
;  jal Tamer_getState
;.org 0x800d7144
;  jal Tamer_getState
;.org 0x800d7268
;  jal Tamer_getState
;.org 0x800d727c
;  jal Tamer_getState
;.org 0x800e3264
;  jal Tamer_getState
.org 0x800e6dcc
  jal Tamer_getState
.org 0x800eeab4
  jal Tamer_getState
.org 0x800eeb70
  jal Tamer_getState

;.org 0x800aa1c4
;  jal Tamer_startAnimation
.org 0x800de058
  jal Tamer_startAnimation
.org 0x800de25c
  jal Tamer_startAnimation
.org 0x800de934
  jal Tamer_startAnimation
.org 0x80105480
  jal Tamer_startAnimation
.org 0x801054fc
  jal Tamer_startAnimation
.org 0x80105968
  jal Tamer_startAnimation

;.org 0x800d4bf4
;  jal clearTamerWaypoints

.close

.open "work/DIGIMON/BTL_REL.BIN",0x80052ae0
.psx

.org 0x80057808
  jal clearTamerWaypoints

.close

.open "work/DIGIMON/FISH_REL.BIN",0x80070000
.psx

.org 0x80079c58
  jal Tamer_setState
.org 0x80079c9c
  jal Tamer_setState

.close

.open "work/DIGIMON/TRN_REL.BIN",0x80088800
.psx

.org 0x8008ac20
  jal Tamer_setState
.org 0x8008b088
  jal Tamer_setState
.org 0x8008b244
  jal Tamer_setState
.org 0x8008b5a0
  jal Tamer_setState
.org 0x8008b878
  jal Tamer_setState
.org 0x8008bdd0
  jal Tamer_setState
.org 0x8008c234
  jal Tamer_setState
.org 0x8008c9b0
  jal Tamer_setState
.org 0x8008cb58
  jal Tamer_setState
.org 0x8008d024
  jal Tamer_setState
.org 0x8008d484
  jal Tamer_setState

.close


.open "work/DIGIMON/TRN2_REL.BIN",0x80088800
.psx

.org 0x80088898
  jal Tamer_setState
.org 0x80088c84
  jal Tamer_setState
.org 0x80088d18
  jal Tamer_setState
.org 0x800890f0
  jal Tamer_setState
.org 0x80089274
  jal Tamer_setState
.org 0x80089654
  jal Tamer_setState
.org 0x80089784
  jal Tamer_setState
.org 0x80089a24
  jal Tamer_setState
.org 0x80089bbc
  jal Tamer_setState
.org 0x80089fd0
  jal Tamer_setState
.org 0x8008a27c
  jal Tamer_setState
.org 0x8008a60c
  jal Tamer_setState
.org 0x8008a738
  jal Tamer_setState

.close

.open "work/DIGIMON/MURD_REL.BIN",0x8007C000
.psx

.org 0x8007c3f0
  jal Tamer_setFullState

.close


.open "work/DIGIMON/EVL_REL.BIN",0x80060000
.psx

.org 0x80063750
  jal addTamerLevel
.org 0x80063728
  jal addTamerLevel

.close 
