.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

;;; Used for code in vanilla
.notice "Start Code Caves"

; unused libgs functions — repurposed as Cave6
.org 0x800957c0
.area 0x80096bcc-.
  .importobj "compiled/Cave6.lib"

  .notice "Cave6 Empty space left: " + (0x80096bcc-.) + " bytes"
  .fill 0x80096bcc-.
.endarea

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
.area 0x800e9294-.
  .importobj "compiled/Cave3.lib"

  .notice "Cave3 Empty space left: " + (0x800e9294-.) + " bytes"
  .fill 0x800e9294-.
.endarea

.org 0x800e9ce8
.area 0x800e9ff4-.
  .importobj "compiled/Cave4.lib"

  .notice "Cave4 Empty space left: " + (0x800e9ff4-.) + " bytes"
  .fill 0x800e9ff4-.
.endarea

.org 0x800ec520
.area 0x800f0b2c-.
  .importobj "compiled/Cave5.lib"

  .notice "Cave5 Empty space left: " + (0x800f0b2c-.) + " bytes"
  .fill 0x800f0b2c-.
.endarea

;;; Initialized memory, in vanilla only for data
.notice "Start Memory Caves"

.org 0x8011d190
.area 0x8012344c-.
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
  .importobj "compiled/utils.lib"
  .importobj "compiled/font.lib"
  .importobj "compiled/CustomUI.lib"
  
  .notice "Empty space left: " + (0x8012344c-.) + " bytes"
  .fill 0x8012344c-.
.endarea

; override some menu data
.org 0x80123860
.area 0x80124c0c-.

  .notice "Empty space left: " + (0x80124c0c-.) + " bytes"
  .fill 0x80124c0c-.
.endarea

; override vanilla raise data, dynamic library data
.org 0x8012abec
.area 0x8012bacc-.

  .notice "Empty space left: " + (0x8012bacc-.) + " bytes"
  .fill 0x8012bacc-.
.endarea

; override condition bubble data, some file paths
.org 0x8012cb8c
.area 0x8012ceb4-.

  .notice "Empty space left: " + (0x8012ceb4-.) + " bytes"
  .fill 0x8012ceb4-.
.endarea

;;; Not caves, uninitialized memory
.notice "Start Uninitialized Caves"

; override vanilla attack objects, poop data
.org 0x80137a24
.area 0x80138460-.

  .notice "Empty space left: " + (0x80138460-.) + " bytes"
.endarea

; override vanilla rotation data
.org 0x80138694
.area 0x80138720-.

  .notice "Empty space left: " + (0x80138720-.) + " bytes"
.endarea

; override effect data
.org 0x801387b8
.area 0x80138c0c-.

  .notice "Empty space left: " + (0x80138c0c-.) + " bytes"
.endarea

; override chest object data
.org 0x80153408
.area 0x80153cc8-.

  .notice "Empty space left: " + (0x80153cc8-.) + " bytes"
.endarea


;;; TODO: patches, need to be moved somewhere else / deleted when reimplemented

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