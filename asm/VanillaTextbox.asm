.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x80101ea0
  li.u v0,tickCustomSizedTextbox
.org 0x80101ea8
  li.l v0,tickCustomSizedTextbox

.org 0x80101eb0
  li v0,renderCustomSizedTextbox

;.org 0x800ef200
;  jal initializeTextbox
.org 0x80104728
  jal initializeTextbox
.org 0x80104764
  jal initializeTextbox
.org 0x80105bbc
  jal initializeTextbox

;.org 0x800ef294
;  jal tickTextboxHandling
.org 0x80105bf4
  jal tickTextboxHandling

;.org 0x800fde44
;  jal getVRAMModeCoords
;.org 0x800ff240
;  jal getVRAMModeCoords
;.org 0x800ffab0
;  jal getVRAMModeCoords
;.org 0x80100304
;  jal getVRAMModeCoords
;.org 0x801008e4
;  jal getVRAMModeCoords
.org 0x80109078
  jal getVRAMModeCoords
.org 0x80109a58
  jal getVRAMModeCoords

;.org 0x800fc9f4
;  jal registerTextbox
;.org 0x800fcb00
;  jal registerTextbox
;.org 0x800fd1fc
;  jal registerTextbox
;.org 0x800fd384
;  jal registerTextbox
;.org 0x800fd734
;  jal registerTextbox
;.org 0x80101060
;  jal registerTextbox
.org 0x80101ee0
  jal registerTextbox
.org 0x801071c4
  jal registerTextbox
.org 0x801076d8
  jal registerTextbox
.org 0x80107754
  jal registerTextbox
.org 0x8010796c
  jal registerTextbox
.org 0x80107c10
  jal registerTextbox
.org 0x80107dcc
  jal registerTextbox
.org 0x80109d00
  jal registerTextbox
.org 0x80109ea8
  jal registerTextbox
.org 0x80109fc4
  jal registerTextbox

;.org 0x800fb0a8
;  jal triggerBoxCloseFlag
;.org 0x800fb2bc
;  jal triggerBoxCloseFlag
;.org 0x800fb4f0
;  jal triggerBoxCloseFlag
;.org 0x800fb910
;  jal triggerBoxCloseFlag
;.org 0x800fb990
;  jal triggerBoxCloseFlag
;.org 0x800fc724
;  jal triggerBoxCloseFlag
;.org 0x800fc780
;  jal triggerBoxCloseFlag
;.org 0x800fc7f8
;  jal triggerBoxCloseFlag
;.org 0x800fcc20
;  jal triggerBoxCloseFlag
;.org 0x8010004c
;  jal triggerBoxCloseFlag
;.org 0x801000c0
;  jal triggerBoxCloseFlag
;.org 0x801006dc
;  jal triggerBoxCloseFlag
.org 0x801040c8
  jal triggerBoxCloseFlag
.org 0x8010b7a4
  jal triggerBoxCloseFlag
.org 0x8010b800
  jal triggerBoxCloseFlag
.org 0x8010b880
  jal triggerBoxCloseFlag
.org 0x8010b8b8
  jal triggerBoxCloseFlag
.org 0x8010baec
  jal triggerBoxCloseFlag
.org 0x8010bbf0
  jal triggerBoxCloseFlag
.org 0x8010bdcc
  jal triggerBoxCloseFlag
.org 0x8010be2c
  jal triggerBoxCloseFlag
.org 0x8010beb8
  jal triggerBoxCloseFlag
.org 0x8010bf10
  jal triggerBoxCloseFlag
.org 0x8010c0e8
  jal triggerBoxCloseFlag
.org 0x8010c0f0
  jal triggerBoxCloseFlag
.org 0x8010c24c
  jal triggerBoxCloseFlag
.org 0x8010c3ac
  jal triggerBoxCloseFlag
.org 0x8010c3b4
  jal triggerBoxCloseFlag
.org 0x8010c468
  jal triggerBoxCloseFlag
.org 0x8010c470
  jal triggerBoxCloseFlag
.org 0x8010c610
  jal triggerBoxCloseFlag
.org 0x8010c6cc
  jal triggerBoxCloseFlag
.org 0x8010c9a8
  jal triggerBoxCloseFlag
.org 0x8010c9b0
  jal triggerBoxCloseFlag
.org 0x8010c9c8
  jal triggerBoxCloseFlag
.org 0x8010c9d0
  jal triggerBoxCloseFlag

;.org 0x800fc9dc
;  jal createTextbox
;.org 0x800fcae8
;  jal createTextbox
;.org 0x800fd1e4
;  jal createTextbox
;.org 0x800fd36c
;  jal createTextbox
;.org 0x800fd71c
;  jal createTextbox
;.org 0x80101048
;  jal createTextbox
.org 0x80101ec8
  jal createTextbox
.org 0x801071ac
  jal createTextbox
.org 0x801076c0
  jal createTextbox
.org 0x8010773c
  jal createTextbox
.org 0x80107954
  jal createTextbox
.org 0x80107bf8
  jal createTextbox
.org 0x80107db4
  jal createTextbox
.org 0x80109ce8
  jal createTextbox
.org 0x80109e90
  jal createTextbox
.org 0x80109fac
  jal createTextbox


;.org 0x800e6f2c
;  jal closeBox
;.org 0x800fe618
;  jal closeBox
;.org 0x801006a4
;  jal closeBox
.org 0x80102de0
  jal closeBox
.org 0x8010c740
  jal closeBox
.org 0x8010c90c
  jal closeBox
.org 0x8010cb10
  jal closeBox

.org 0x80106188
  jal closeAllTextboxes
.org 0x801061c0
  jal closeAllTextboxes

;.org 0x800fb4e8
;  jal setupBoxOrigin
;.org 0x800fc990
;  jal setupBoxOrigin
;.org 0x800fca78
;  jal setupBoxOrigin
;.org 0x80100fec
;  jal setupBoxOrigin
.org 0x80101e10
  jal setupBoxOrigin
.org 0x8010713c
  jal setupBoxOrigin
.org 0x80107674
  jal setupBoxOrigin
.org 0x801076fc
  jal setupBoxOrigin
.org 0x80107908
  jal setupBoxOrigin
.org 0x80107bac
  jal setupBoxOrigin
.org 0x80107d68
  jal setupBoxOrigin

.org 0x80109ccc
  li v0, tickScriptDialogueBox

.org 0x80109cd8
  li v0, renderScriptDialogueBox

.close

.open "work/DIGIMON/DGET_REL.BIN",0x80080800
.psx

.org 0x80080a58
  jal registerTextbox
.org 0x80080be8
  jal registerTextbox
.org 0x80080efc
  jal registerTextbox

.org 0x80081970
  jal triggerBoxCloseFlag
.org 0x80081b04
  jal triggerBoxCloseFlag
.org 0x80081b4c
  jal triggerBoxCloseFlag
.org 0x80081b54
  jal triggerBoxCloseFlag
.org 0x80081c38
  jal triggerBoxCloseFlag
.org 0x80081cac
  jal triggerBoxCloseFlag

.org 0x80080a40
  jal createTextbox
.org 0x80080bcc
  jal createTextbox
.org 0x80080ee4
  jal createTextbox

.org 0x800809fc
  jal setupBoxOrigin
.org 0x80080b80
  jal setupBoxOrigin
.org 0x80080df8
  jal setupBoxOrigin

.close


.open "work/DIGIMON/TRN_REL.BIN",0x80088800
.psx

.org 0x8008e544
  jal closeBox

.close

.open "work/DIGIMON/TRN2_REL.BIN",0x80088800
.psx

.org 0x8008cf1c
  jal closeBox

.close

.open "work/DIGIMON/FISH_REL.BIN",0x80070000
.psx

.org 0x8007162c
  jal renderConfirmPrompt

.close
