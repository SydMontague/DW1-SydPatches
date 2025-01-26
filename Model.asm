.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

; 0x800a1540
.org 0x800a0a50
.area 0x800a15dc-.
  .importobj "compiled/Model.lib"

  .notice "Empty space left: " + (0x800a15dc-.) + " bytes"
  .fill 0x800a15dc-.
.endarea

.org 0x800a13b0
  jal renderFlatDigimon

;.org 0x800a446c
;  jal loadMMD
.org 0x800a983c
  jal loadMMD
.org 0x800f1534
  jal loadMMD
.org 0x800b6110
  j loadMMD

.org 0x800b63fc
  j unloadModel
.org 0x800b6634
  jal unloadModel
.org 0x800d9764
  jal unloadModel
.org 0x800d9770
  jal unloadModel
.org 0x800d97a0
  jal unloadModel
.org 0x800e3624
  jal unloadModel
.org 0x800f172c
  jal unloadModel

;.org 0x800a4490
;  jal initializeDigimonObject
;.org 0x800a4ff4
;  jal initializeDigimonObject
;.org 0x800a5218
;  jal initializeDigimonObject
.org 0x800a9860
  jal initializeDigimonObject
.org 0x800b61f0
  jal initializeDigimonObject
.org 0x800f1558
  jal initializeDigimonObject

.org 0x800b61a8		
  jal removeEntity
.org 0x800b63e4		
  jal removeEntity
.org 0x800b65e0		
  jal removeEntity
.org 0x800d9744		
  jal removeEntity
.org 0x800e3610		
  jal removeEntity
.org 0x800f1720		
  jal removeEntity

.org 0x800a9890
  jal setupEntityMatrix
.org 0x800a9c84
  jal setupEntityMatrix
.org 0x800ac700
  jal setupEntityMatrix
.org 0x800ac96c
  jal setupEntityMatrix
.org 0x800b6240
  jal setupEntityMatrix
.org 0x800bf078
  jal setupEntityMatrix
.org 0x800bf1c8
  jal setupEntityMatrix
.org 0x800f14e4
  jal setupEntityMatrix
.org 0x800f15a8
  jal setupEntityMatrix

.org 0x800a9874
  jal setEntityPosition
.org 0x800a9c60
  jal setEntityPosition
.org 0x800ac6f4
  jal setEntityPosition
.org 0x800ac960
  jal setEntityPosition
.org 0x800b6214
  jal setEntityPosition
.org 0x800bf05c
  jal setEntityPosition
.org 0x800bf1ac
  jal setEntityPosition
.org 0x800f158c
  jal setEntityPosition
.org 0x800f1af8
  jal setEntityPosition
.org 0x800f1b68
  jal setEntityPosition


.close

.open "work/DIGIMON/VS_REL.BIN",0x80052ae0
.psx
  
.org 0x80056f88
  jal loadMMD
.org 0x80057000
  jal loadMMD

.org 0x80056f7c
  jal unloadModel
.org 0x80056ff4
  jal unloadModel
.org 0x80057234
  jal unloadModel
.org 0x80057258
  jal unloadModel

.org 0x80056fac
  jal initializeDigimonObject
.org 0x80057024
  jal initializeDigimonObject

.org 0x80056f70
  jal removeEntity
.org 0x80056fe8
  jal removeEntity
.org 0x80057228
  jal removeEntity
.org 0x8005724c
  jal removeEntity

.close


.open "work/DIGIMON/STD_REL.BIN",0x80052ae0
.psx

.org 0x80056ec4
  jal initializeDigimonObject

.org 0x800584c8
  jal setEntityPosition
.org 0x80058554
  jal setEntityPosition

.close


.open "work/DIGIMON/TRN_REL.BIN",0x80088800
.psx

.org 0x8008ccdc
  jal setupEntityMatrix

.close

.open "work/DIGIMON/EVL_REL.BIN",0x80060000
.psx

.org 0x80063aa0
  jal unloadModel

.org 0x80063a8c
  jal removeEntity

.close


.open "work/DIGIMON/DOOA_REL.BIN",0x80080000
.psx

.org 0x8008060c
  jal setupEntityMatrix
.org 0x800806d0
  jal setupEntityMatrix
.org 0x80080a80
  jal setupEntityMatrix
.org 0x80080b20
  jal setupEntityMatrix

.org 0x80080604
  jal setEntityPosition
.org 0x800806c8
  jal setEntityPosition
.org 0x80080a78
  jal setEntityPosition
.org 0x80080b18
  jal setEntityPosition
.org 0x8008255c
  jal setEntityPosition

.close