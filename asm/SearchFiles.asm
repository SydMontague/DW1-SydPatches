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
;.org 0x800e6220
;  jal readFile
;.org 0x800e65d8
;  jal readFile
.org 0x80105a14
  jal readFile

;.org 0x800e0fe8
;  jal loadTextureFile
.org 0x801043cc
  jal loadTextureFile

;.org 0x800e625c
;  jal addFileReadRequestPath
.org 0x80104384
  jal addFileReadRequestPath

;.org 0x800e62a8
;  jal loadDynamicLibrary
;.org 0x800e6c0c
;  jal loadDynamicLibrary
;.org 0x800e743c
;  jal loadDynamicLibrary
;.org 0x800e761c
;  jal loadDynamicLibrary
;.org 0x800e89e8
;  jal loadDynamicLibrary
.org 0x800ff978
  jal loadDynamicLibrary
.org 0x800ff9c4
  jal loadDynamicLibrary
.org 0x800ffa18
  jal loadDynamicLibrary
.org 0x8010439c
  jal loadDynamicLibrary
.org 0x801043e4
  jal loadDynamicLibrary
.org 0x80111b88
  jal loadDynamicLibrary

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


.org 0x8005f174
  jal tickFileReadQueue

.org 0x8006d6c4
  jal addFileReadRequest

.org 0x8006dee0
  j setFileReadCallback2

.close


.open "work/DIGIMON/MURD_REL.BIN",0x8007C000
.psx

.org 0x8007d690
  jal loadTextureFile

.org 0x8007c54c
  jal tickFileReadQueue

.org 0x8007d70c
  jal loadDynamicLibrary

.close


.open "work/DIGIMON/BTL_REL.BIN",0x80052ae0
.psx

.org 0x80064fdc
  jal addFileReadRequest

.org 0x800657f8
  j setFileReadCallback2

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

.org 0x80063fb0
  j setFileReadCallback2

.org 0x80063794
  jal addFileReadRequest

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

.org 0x80080280
  jal tickFileReadQueue
.org 0x80080d18
  jal tickFileReadQueue
.org 0x800824f4
  jal tickFileReadQueue

.org 0x80080270
  jal loadDynamicLibrary

.close

.open "work/DIGIMON/EVL_REL.BIN",0x80060000
.psx

.org 0x80060de4
  jal tickFileReadQueue

.close
