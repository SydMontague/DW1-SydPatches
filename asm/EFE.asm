.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.close

.open "work/DIGIMON/BTL_REL.BIN",0x80052ae0
.psx

.org 0x800660ec
  jal initializeFlashData

.org 0x80073328
  .word EFE_rotateVector

.org 0x80073390
  .word EFE_createFlash

.org 0x80064f90
  jal getEFEDATEntry


.org 0x80073480
  .word EFE_copyVector

.org 0x800734a8
  .word EFE_maskVectors

; BTL_loadEFEImmediate
.org 0x80073604
  .word BTL_loadEFEImmediate

; BTL_loadEFEVariable
.org 0x80073608
  .word BTL_loadEFEVariable

; BTL_loadEFERandomValue
.org 0x8007360c
  .word BTL_loadEFERandomValue

; BTL_loadEFEIndexedVariable
.org 0x80073610
  .word BTL_loadEFEIndexedVariable

; BTL_pushEFEImmediate
.org 0x8007362c
  .word BTL_pushEFEImmediate

; BTL_pushEFEVariable
.org 0x80073630
  .word BTL_pushEFEVariable

; BTL_pushEFEVariableAddress
.org 0x80073634
  .word BTL_pushEFEVariableAddress

; BTL_popEFEValueToVariable
.org 0x80073644
  .word BTL_popEFEValueToVariable

.org 0x800736ec
  .word EFE_compareEqual
.org 0x800736f0
  .word EFE_compareNotEqual
.org 0x800736f4
  .word EFE_compareLess
.org 0x800736f8
  .word EFE_compareLessOrEqual
.org 0x800736fc
  .word EFE_compareGreater
.org 0x80073700
  .word EFE_compareGreaterOrEqual

; BTL_branchEFEOnComparison
.org 0x80073620
  .word BTL_branchEFEOnComparison

; BTL_callEFESubroutine
.org 0x80073638
  .word BTL_callEFESubroutine

; BTL_dispatchEFESubOpcode
.org 0x8007363c
  .word BTL_dispatchEFESubOpcode

; BTL_jumpEFEScript
.org 0x80073628
  .word BTL_jumpEFEScript

; BTL_returnFromEFESubroutine
.org 0x80073640
  .word BTL_returnFromEFESubroutine

; BTL_stopEFEScript
.org 0x80073618
  .word BTL_stopEFEScript
.org 0x8007361c
  .word BTL_stopEFEScript
.org 0x80073624
  .word BTL_stopEFEScript

.close

.open "work/DIGIMON/STD_REL.BIN",0x80052ae0
.psx

.org 0x8006e7d4
  jal initializeFlashData

.org 0x8007ab44
  .word EFE_rotateVector

.org 0x8007abac
  .word EFE_createFlash

.org 0x8006d678
  jal getEFEDATEntry

.org 0x8006e89c
  jal EFE_downloadSomeImage

.org 0x8006eb94
  j EFE_modifySomeImage


.org 0x8007ac9c
  .word EFE_copyVector

.org 0x8007acc4
  .word EFE_maskVectors

; STD_loadEFEImmediate
.org 0x8007ae20
  .word STD_loadEFEImmediate

; STD_loadEFEVariable
.org 0x8007ae24
  .word STD_loadEFEVariable

; STD_loadEFERandomValue
.org 0x8007ae28
  .word STD_loadEFERandomValue

; STD_loadEFEIndexedVariable
.org 0x8007ae2c
  .word STD_loadEFEIndexedVariable

; STD_pushEFEImmediate
.org 0x8007ae48
  .word STD_pushEFEImmediate

; STD_pushEFEVariable
.org 0x8007ae4c
  .word STD_pushEFEVariable

; STD_pushEFEVariableAddress
.org 0x8007ae50
  .word STD_pushEFEVariableAddress

; STD_popEFEValueToVariable
.org 0x8007ae60
  .word STD_popEFEValueToVariable

.org 0x8007af08
  .word EFE_compareEqual
.org 0x8007af0c
  .word EFE_compareNotEqual
.org 0x8007af10
  .word EFE_compareLess
.org 0x8007af14
  .word EFE_compareLessOrEqual
.org 0x8007af18
  .word EFE_compareGreater
.org 0x8007af1c
  .word EFE_compareGreaterOrEqual

; STD_branchEFEOnComparison
.org 0x8007ae3c
  .word STD_branchEFEOnComparison

; STD_callEFESubroutine
.org 0x8007ae54
  .word STD_callEFESubroutine

; STD_dispatchEFESubOpcode
.org 0x8007ae58
  .word STD_dispatchEFESubOpcode

; STD_jumpEFEScript
.org 0x8007ae44
  .word STD_jumpEFEScript

; STD_returnFromEFESubroutine
.org 0x8007ae5c
  .word STD_returnFromEFESubroutine

; STD_stopEFEScript
.org 0x8007ae34
  .word STD_stopEFEScript
.org 0x8007ae38
  .word STD_stopEFEScript
.org 0x8007ae40
  .word STD_stopEFEScript

.close

.open "work/DIGIMON/VS_REL.BIN",0x80052ae0
.psx

.org 0x800648a4
  jal initializeFlashData

.org 0x80070b70
  .word EFE_rotateVector

.org 0x80070bd8
  .word EFE_createFlash

.org 0x80063748
  jal getEFEDATEntry

.org 0x8006496c
  jal EFE_downloadSomeImage

.org 0x80064c64
  j EFE_modifySomeImage


.org 0x80070cc8
  .word EFE_copyVector

.org 0x80070cf0
  .word EFE_maskVectors

; VS_loadEFEImmediate
.org 0x80070e4c
  .word VS_loadEFEImmediate

; VS_loadEFEVariable
.org 0x80070e50
  .word VS_loadEFEVariable

; VS_loadEFERandomValue
.org 0x80070e54
  .word VS_loadEFERandomValue

; VS_loadEFEIndexedVariable
.org 0x80070e58
  .word VS_loadEFEIndexedVariable

; VS_pushEFEImmediate
.org 0x80070e74
  .word VS_pushEFEImmediate

; VS_pushEFEVariable
.org 0x80070e78
  .word VS_pushEFEVariable

; VS_pushEFEVariableAddress
.org 0x80070e7c
  .word VS_pushEFEVariableAddress

; VS_popEFEValueToVariable
.org 0x80070e8c
  .word VS_popEFEValueToVariable

.org 0x80070f34
  .word EFE_compareEqual
.org 0x80070f38
  .word EFE_compareNotEqual
.org 0x80070f3c
  .word EFE_compareLess
.org 0x80070f40
  .word EFE_compareLessOrEqual
.org 0x80070f44
  .word EFE_compareGreater
.org 0x80070f48
  .word EFE_compareGreaterOrEqual

; VS_branchEFEOnComparison
.org 0x80070e68
  .word VS_branchEFEOnComparison

; VS_callEFESubroutine
.org 0x80070e80
  .word VS_callEFESubroutine

; VS_dispatchEFESubOpcode
.org 0x80070e84
  .word VS_dispatchEFESubOpcode

; VS_jumpEFEScript
.org 0x80070e70
  .word VS_jumpEFEScript

; VS_returnFromEFESubroutine
.org 0x80070e88
  .word VS_returnFromEFESubroutine

; VS_stopEFEScript
.org 0x80070e60
  .word VS_stopEFEScript
.org 0x80070e64
  .word VS_stopEFEScript
.org 0x80070e6c
  .word VS_stopEFEScript

.close

.open "work/DIGIMON/EVL_REL.BIN",0x80060000
.psx

.org 0x80063164
  jal initializeFlashData

.org 0x80060d90
  jal EFE_createFlash
.org 0x800607d0
  jal EFE_createFlash

.close


.open "work/DIGIMON/EAB_REL.BIN",0x80060000
.psx

.org 0x800615b0
  jal initializeFlashData

.org 0x8006169c
  jal EFE_createFlash

.close


.open "work/DIGIMON/DOOA_REL.BIN",0x80080000
.psx

.org 0x8008467c
  jal initializeFlashData

.org 0x800824d0
  jal EFE_createFlash
.org 0x80080c64
  jal EFE_createFlash

.org 0x80080cb8
  jal EFE_setFlashOffset

.close
