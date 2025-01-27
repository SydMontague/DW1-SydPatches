.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

;; patch lookupFileTable function to use libcd_CdSearchFile instead, allowing for arbitrary LBAs and file sizes
.org lookupFileTable
.area lookupFileTable_end-.
  addiu sp,sp,-0x128
  sw ra,0x120(sp)
  sw a0,0x11C(sp)
  li v0,0x5c ; '\\'
  sw a1,0x118(sp)
  sb v0,0x00(sp)
  jal strcpy
  addiu a0,sp,0x01
  addiu a0,sp,0x00
  jal strcat
  addiu a1,gp,-0x790c
  addiu a1,sp,0x00
  jal libcd_CdSearchFile
  addiu a0,sp,0x100
  lw a1,0x11C(sp)
  lw at,0x100(sp)
  lw a0,0x104(sp)
  sw at,0x00(a1)
  sw a0,0x04(a1)
  lw ra,0x120(sp)
  sltu v0,r0,v0
  addiu sp,sp,0x128
  jr ra
  nop

ALLTIM_address:
.ascii "CHDAT\\ALLTIM.TIM",0x00

.align 4
SB_address: ; loadSounds, 0x800e8f80
.ascii "SOUND\\VHB\\SB",0
.align 4
SL_address: ; initializeMusic
.ascii "SOUND\\VHB\\SL",0
.align 4
SS_address: ; initializeMusic
.ascii "SOUND\\VHB\\SS",0

.endarea

;; change ALLTIM path loading
.org 0x800a2934
  li.u a1,ALLTIM_address
.org 0x800a2950
  li.l a1,ALLTIM_address
;.org 0x800a1d68
;  li.u a1,ALLTIM_address
;.org 0x800a1d74
;  li.l a1,ALLTIM_address

;; redirect SB string to new address
.org 0x800c65ec
  li.u a1,SB_address
.org 0x800c65fc
  li.l a1,SB_address
.org 0x800e8f88
  li.u a1,SB_address
.org 0x800e8f98
  li.l a1,SB_address

;; redirect SS string to new address
.org 0x800c6004
  li.u a1,SS_address
.org 0x800c600c
  li.l a1,SS_address

;; recired SL string to new address
.org 0x800c602c
  li.u a1,SL_address
.org 0x800c6034
  li.l a1,SL_address

;; write *ALL strings to new locations
.org 0x8013443c
.area 32h
ESALL_address:
.ascii "SOUND\\VHB\\ESALL",0 ; 0x800c662c, 0x800e9034
FAALL_address:
.ascii "SOUND\\VHB\\FAALL",0 ; loadMusicFont
.endarea

.org 0x8013436c
.area 32h
VBALL_address:
.ascii "SOUND\\VHB\\VBALL",0 ; loadDigimonSounds, 0x800e8fb4
VLALL_address:
.ascii "SOUND\\VHB\\VLALL",0 ; 0x800c64e0, 0x800e9154
.endarea

;; redirect ESALL string to new address
.org 0x800c66a0
  addiu a1,gp,ESALL_address-global_pointer
.org 0x800e90a8
  addiu a1,gp,ESALL_address-global_pointer

;; redirect FAALL string to new address
.org 0x800c5f0c
  addiu a1,gp,FAALL_address-global_pointer

;; redirect VBALL string to new address
.org 0x800c65ac
  addiu a1,gp,VBALL_address-global_pointer
.org 0x800e9014
  addiu a1,gp,VBALL_address-global_pointer

;; redirect VLALL string to new address
.org 0x800c6534
  addiu a1,gp,VLALL_address-global_pointer
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
; readVHBFileSectors
.org 0x800c5ca0
  b 0x800c5cbc
  nop

; readVHBFile
.org 0x800c5af8
  b 0x800c5b18
  nop

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