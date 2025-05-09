.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x800a0a50
.area 0x800afd78-.
  .importobj "compiled/Cave1.lib"

  .notice "Cave1 Empty space left: " + (0x800afd78-.) + " bytes"
  .fill 0x800afd78-.
.endarea

.org 0x800b3d84
.area 0x800c67b4-.
  .importobj "compiled/Cave2.lib"

  .notice "Cave2 Empty space left: " + (0x800c67b4-.) + " bytes"
  .fill 0x800c67b4-.
.endarea

.org 0x800d3078
.area 0x800d78bc-.
  .importobj "compiled/Cave3.lib"

  .notice "Cave3 Empty space left: " + (0x800d78bc-.) + " bytes"
  .fill 0x800d78bc-.
.endarea

.org 0x8011d46c
.area 0x80122d24-.
  .importobj "compiled/GameData.lib"
;;;
; Relocated game data start. 
; Everything placed in here must only be referenced from code that can deal with it changing its address.
; This is to allow the data in here to be shuffled at will to suite whatever need.
;;;
RAISE_DATA:
  .include "RaiseData.asm"
EVO_REQ_DATA:
  .include "EvoReqs.asm"
EVO_GAINS_DATA:
  .include "EvoGains.asm"
EVO_PATHS_DATA:
  .include "EvoPaths.asm"
NAMING_DATA:
  .include "NamingData.asm"

.align 4
SB_address: ; 0x800e8f80
.ascii "SOUND\\VHB\\SB",0

.align 4
  .importobj "compiled/utils.lib"
  .importobj "compiled/font.lib"
  .importobj "compiled/CustomUI.lib"
  .importobj "compiled/CustomInit.lib"
  
  .notice "Empty space left: " + (0x80122d24-.) + " bytes"
  .fill 0x80122d24-.
.endarea


; override vanilla attack objects
.org 0x80137a24
.area 0x80137c00-.

  .notice "Empty space left: " + (0x80137c00-.) + " bytes"
  .fill 0x80137c00-.
.endarea

; override some effect data
.org 0x80123370
.area 0x8012342c-.

  .notice "Empty space left: " + (0x8012342c-.) + " bytes"
  .fill 0x8012342c-.
.endarea

; override some menu data
.org 0x80123860
.area 0x80124c0c-.

  .notice "Empty space left: " + (0x80124c0c-.) + " bytes"
  .fill 0x80124c0c-.
.endarea


; override vanilla raise data
.org 0x8012abec
.area 0x8012b918-.

  .notice "Empty space left: " + (0x8012b918-.) + " bytes"
  .fill 0x8012b918-.
.endarea


; override effect data
.org 0x801387b8
.area 0x80138c0c-.

  .notice "Empty space left: " + (0x80138c0c-.) + " bytes"
  .fill 0x80138c0c-.
.endarea

; override vanilla rotation data
.org 0x80138694
.area 0x80138720-.

  .notice "Empty space left: " + (0x80138720-.) + " bytes"
  .fill 0x80138720-.
.endarea

; unused libgs functions
.org 0x800957c0
.area 0x80096bcc-.
  .notice "Empty space left: " + (0x80096bcc-.) + " bytes"
  .fill 0x80096bcc-.
.endarea

; relocated poop data (model, libgs data)
.org 0x80137c00
.area 0x80138460-.
  .notice "Empty space left: " + (0x80138460-.) + " bytes"
  .fill 0x80138460-.
.endarea

.org 0x800ee8d4
  jal customInit

.org drawString
  sra a1,a1,2
  addiu a3,a2,256
  addiu a2,a1,704
  move a1,a0
  li.u a0,vanillaFont
  j drawStringNew
  li.l a0,vanillaFont

; fix CARD_DATA entry for Machinedramon not having set the correct digimonId
.org 0x801300dc
.byte 115

.close