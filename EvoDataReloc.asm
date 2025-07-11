.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x800e2544
.area 0x800e3584-.
.importobj "compiled/Evolution.lib"

  .notice "Empty space left: " + (0x800e3584-.) + " bytes"
.endarea

;.org 0x800a91b8
;  jal getFreshEvolutionTarget
;.org 0x800a91e8
;  jal getInTrainingEvolutionTarget
;.org 0x800a9218
;  jal getRegularEvolutionTarget
;.org 0x800a9248
;  jal getRegularEvolutionTarget
;.org 0x800a9274
;  jal handleSpecialEvolutionPraise

;.org 0x800ddea8
;  jal handleSpecialEvolutionPraise
;.org 0x800ddce4
;  jal handleSpecialEvolutionSleep

;;; Evo Requirements Start
; reimplemented in Evolution.cpp
; requirement score
;.org 0x800e26e4
;  li v1,EVO_REQ_DATA

; baby evo
;.org 0x800e2610
;  li v0,EVO_REQ_DATA+0x1A

; rookie evo
;.org 0x800e2c44
;  li v0,EVO_REQ_DATA+0x1A
;.org 0x800e2ca4
;  li.u a0,EVO_REQ_DATA+0x02
;.skip 4
;  li.l a0,EVO_REQ_DATA+0x02
;.org 0x800e2d08
;  li a0,EVO_REQ_DATA+0x04
;.org 0x800e2d68
;  li a0,EVO_REQ_DATA+0x06
;.org 0x800e2da8
;  li a0,EVO_REQ_DATA+0x08
;.org 0x800e2de8
;  li a0,EVO_REQ_DATA+0x0A
;.org 0x800e2e28
;  li a0,EVO_REQ_DATA+0x0C

; champion evo
;.org 0x800e2f98
;  li v0,EVO_REQ_DATA+0x1A
;.org 0x800e2ff8
;  li.u a0,EVO_REQ_DATA+0x02
;  .skip 4
;  li.l a0,EVO_REQ_DATA+0x02
;.org 0x800e305c
;  li a0,EVO_REQ_DATA+0x04
;.org 0x800e30bc
;  li a0,EVO_REQ_DATA+0x06
;.org 0x800e30fc
;  li a0,EVO_REQ_DATA+0x08
;.org 0x800e313c
;  li a0,EVO_REQ_DATA+0x0A
;.org 0x800e317c
;  li a0,EVO_REQ_DATA+0x0C
;;; Evo Requirements End

;;; Evo Paths Start
;.org 0x800ba9d0
;  li v1,EVO_PATHS_DATA+11
;.org 0x800baa24
;  li v1,EVO_PATHS_DATA+0x5+11

;.org 0x800bab20
;  li v0,EVO_PATHS_DATA+11
;.org 0x800bacf0
;  li v0,EVO_PATHS_DATA+0x5+11

;.org 0x800baff8
;  li v0,EVO_PATHS_DATA+11
;.org 0x800bb13c
;  li v0,EVO_PATHS_DATA+0x5+11

; reimplemented in Evolution.cpp
;.org 0x800e25c8
;  li v0,EVO_PATHS_DATA+11
;.org 0x800e2bec
;  li.u v0,EVO_PATHS_DATA+11
;  .skip 4
;  li.l v0,EVO_PATHS_DATA+11
;.org 0x800e2f40
;  li.u v0,EVO_PATHS_DATA+11
;.skip 4
;  li.l v0,EVO_PATHS_DATA+11
;;; Evo Paths End

;;; Statsgains Start
; reimplemented in Evolution.cpp
;.org 0x800e2b18
;  li v0,EVO_GAINS_DATA
;.org 0x800e2b40
;  li v0,EVO_GAINS_DATA

.org 0x800e35c8
  li v0,EVO_GAINS_DATA
.org 0x800e7370
  li.u v0,EVO_GAINS_DATA+0x0C
  .skip 4
  li.l v0,EVO_GAINS_DATA+0x0C
;;; Statsgains End

.close


.open "work/DIGIMON/EVL_REL.BIN",0x80060000
.psx

;;; Statsgains Start
; TO EVL_REL
.org 0x80063390
  li v0,EVO_GAINS_DATA
;;; Statsgains End

.close