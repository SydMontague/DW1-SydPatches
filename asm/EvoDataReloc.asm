.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.close


.open "work/DIGIMON/EVL_REL.BIN",0x80060000
.psx

;;; Statsgains Start
; TO EVL_REL
.org 0x80063390
  li v0,EVO_GAINS_DATA
;;; Statsgains End

.close