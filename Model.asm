.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x800a0a50
.area 0x800a1430-.
  .importobj "compiled/Model.lib"

  .notice "Empty space left: " + (0x800a1430-.) + " bytes"
  .fill 0x800a1430-.
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

.close


.open "work/DIGIMON/STD_REL.BIN",0x80052ae0
.psx

.org 0x80056ec4
  jal initializeDigimonObject

.close


.open "work/DIGIMON/EVL_REL.BIN",0x80060000
.psx

.org 0x80063aa0
  jal unloadModel

.close