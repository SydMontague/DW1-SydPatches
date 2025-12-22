.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x80104304
  jal loadTrainingLibrary

;.org 0x800a21c8
;  jal readFile
;.org 0x800a236c
;  jal readFile
;.org 0x800a2c20
;  jal readFile
;.org 0x800a2ca4
;  jal readFile
;.org 0x800a3320
;  jal readFile
;.org 0x800a34b8
;  jal readFile
;.org 0x800a43d0
;  jal readFile
;.org 0x800c5b28
;  jal readFile
;.org 0x800d5bb0
;  jal readFile
;.org 0x800d6334
;  jal readFile
;.org 0x800dfeb0
;  jal readFile
;.org 0x800e0898
;  jal readFile
;.org 0x800e0b28
;  jal readFile
;.org 0x800e11e4
;  jal readFile
;.org 0x800e11fc
;  jal readFile
.org 0x800e6220
  jal readFile
.org 0x800e65d8
  jal readFile
.org 0x80105a14
  jal readFile

;.org 0x800a32d8
;  jal lookupFileTable
;.org 0x800a3428
;  jal lookupFileTable
;.org 0x800a3644
;  jal lookupFileTable
.org 0x800e3b38
  jal lookupFileTable
.org 0x800e3be8
  jal lookupFileTable


;.org 0x800e0fe8
;  jal loadTextureFile
.org 0x801043cc
  jal loadTextureFile

.org 0x800ee8e4
  jal loadTIMFile
.org 0x800ee8f4
  jal loadTIMFile
.org 0x800ef008
  jal loadTIMFile
.org 0x800f7adc
  jal loadTIMFile
.org 0x800f7aec
  jal loadTIMFile
.org 0x800f7afc
  jal loadTIMFile
.org 0x800f7b0c
  jal loadTIMFile
.org 0x800f7b1c
  jal loadTIMFile
.org 0x800f7b2c
  jal loadTIMFile
.org 0x800f8130
  jal loadTIMFile

;.org 0x800bb51c
;  j loadStackedTIMEntry

;.org 0x800c5cd4
;  jal readFileSectors

;; redirect SB string to new address
;.org 0x800c65ec
;  li.u a1,SB_address
;.org 0x800c65fc
;  li.l a1,SB_address
.org 0x800e8f88
  li.u a1,SB_address
.org 0x800e8f98
  li.l a1,SB_address

;; redirect SS string to new address
;.org 0x800c6004
;  li.u a1,SS_address
;.org 0x800c600c
;  li.l a1,SS_address

;; recired SL string to new address
;.org 0x800c602c
;  li.u a1,SL_address
;.org 0x800c6034
;  li.l a1,SL_address

;; write *ALL strings to new locations
.org 0x8013443c
.area 32h
ESALL_address:
.ascii "SOUND\\VHB\\ESALL",0 ; 0x800e9034
.endarea

.org 0x8013436c
.area 32h
VBALL_address:
.ascii "SOUND\\VHB\\VBALL",0 ; 0x800e8fb4
VLALL_address:
.ascii "SOUND\\VHB\\VLALL",0 ; 0x800e9154
.endarea

;; redirect ESALL string to new address
;.org 0x800c66a0
;  addiu a1,gp,ESALL_address-global_pointer
.org 0x800e90a8
  addiu a1,gp,ESALL_address-global_pointer

;; redirect VBALL string to new address
;.org 0x800c65ac
;  addiu a1,gp,VBALL_address-global_pointer
.org 0x800e9014
  addiu a1,gp,VBALL_address-global_pointer

;; redirect VLALL string to new address
;.org 0x800c6534
;  addiu a1,gp,VLALL_address-global_pointer
.org 0x800e91b0
  addiu a1,gp,VLALL_address-global_pointer
.org 0x800e91bc
  b 0x800e91dc
  nop

;; only use one ".VHB" instance
.org 0x800e8b5c
  addiu a2,gp,-0x77a0
.org 0x800e8c84
  addiu a2,gp,-0x77a0
.org 0x800e91e4
  addiu a2,gp,-0x77a0

;; jump over strrchr calls
; battles?
.org 0x800e8b34
  b 0x800e8b54
  nop

; quite a few _REL.BIN
.org 0x800e8c5c
  b 0x800e8c7c
  nop

; something evo+death sounds?
.org 0x800e91bc
  b 0x800e91dc
  nop

.close

.open "work/DIGIMON/FISH_REL.BIN",0x80070000
.psx

.org 0x80079f44
  jal lookupFileSize

.org 0x80072bc0
  jal loadTextureFile

.org 0x80079f80
  jal readFile

.close

.open "work/DIGIMON/STD_REL.BIN",0x80052ae0
.psx

.org 0x80057ebc
  jal lookupFileSize

.org 0x800578f4
  jal readFile
.org 0x80057d74
  jal readFile
.org 0x80057eec
  jal readFile
.org 0x80057f9c
  jal readFile
.org 0x8006b2e8
  jal readFile
.org 0x8006b8a4
  jal readFile
.org 0x8006baf4
  jal readFile
.org 0x8006bd80
  jal readFile
.org 0x8006bd94
  jal readFile


.org 0x80056db8
  jal loadTIMFile
.org 0x80056dd4
  jal loadTIMFile
.org 0x80057700
  jal loadTIMFile

.close


.open "work/DIGIMON/MURD_REL.BIN",0x8007C000
.psx

.org 0x8007d690
  jal loadTextureFile

.close


.open "work/DIGIMON/VS_REL.BIN",0x80052ae0
.psx

.org 0x80058000
  jal readFile
.org 0x800587dc
  jal readFile
.org 0x80058934
  jal readFile
.org 0x800589f0
  jal readFile
.org 0x800625a4
  jal readFile
.org 0x80062b8c
  jal readFile

.org 0x80056e6c
  jal loadTIMFile

.close


.open "work/DIGIMON/KAR_REL.BIN",0x80053800
.psx

.org 0x80053c50
  jal readFile

.close


.open "work/DIGIMON/DOOA_REL.BIN",0x80080000
.psx

.org 0x80080d3c
  jal readFile
.org 0x80080e24
  jal readFile

.close
