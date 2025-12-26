.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x800e4c00
  jal matrixToEuler2

.close

.open "work/DIGIMON/DOOA_REL.BIN",0x80080000
.psx

.org 0x8008112c
  jal getDistance
.org 0x80081ba8
  jal getDistance
.org 0x8008410c
  jal getDistance
.org 0x8008458c
  jal getDistance

.close


.open "work/DIGIMON/MURD_REL.BIN",0x8007C000
.psx

.org 0x8007c7b0
  jal getDistance
.org 0x8007ced0
  jal getDistance

.close


.open "work/DIGIMON/FISH_REL.BIN",0x80070000
.psx

.org 0x80079934
  jal calculatePosition

.org 0x80071b9c
  jal mapToWorldPosition
.org 0x80074888
  jal mapToWorldPosition
.org 0x800748bc
  jal mapToWorldPosition
.org 0x80074a70
  jal mapToWorldPosition

.org 0x800733b0
  jal rotateVector
.org 0x800734cc
  jal rotateVector
.org 0x80074248
  jal rotateVector
.org 0x8007440c
  jal rotateVector
.org 0x80074990
  jal rotateVector
.org 0x80074c00
  jal rotateVector
.org 0x80074ea0
  jal rotateVector
.org 0x800752e4
  jal rotateVector
.org 0x800753c4
  jal rotateVector
.org 0x800762b4
  jal rotateVector
.org 0x8007643c
  jal rotateVector
.org 0x800765d0
  jal rotateVector
.org 0x8007666c
  jal rotateVector
.org 0x800767c8
  jal rotateVector
.org 0x80076a90
  jal rotateVector
.org 0x80076ba8
  jal rotateVector
.org 0x80076cec
  jal rotateVector
.org 0x80076e64
  jal rotateVector

.org 0x80073394
  jal toEulerAngles
.org 0x800734a8
  jal toEulerAngles
.org 0x80074220
  jal toEulerAngles
.org 0x800743e4
  jal toEulerAngles
.org 0x800748e4
  jal toEulerAngles
.org 0x80074ad8
  jal toEulerAngles
.org 0x80074e04
  jal toEulerAngles
.org 0x800750b0
  jal toEulerAngles
.org 0x8007510c
  jal toEulerAngles
.org 0x8007520c
  jal toEulerAngles
.org 0x80075380
  jal toEulerAngles
.org 0x80075fb8
  jal toEulerAngles
.org 0x8007624c
  jal toEulerAngles
.org 0x800763d4
  jal toEulerAngles
.org 0x80076518
  jal toEulerAngles
.org 0x80076754
  jal toEulerAngles
.org 0x8007693c
  jal toEulerAngles
.org 0x80076b78
  jal toEulerAngles
.org 0x80076ca8
  jal toEulerAngles
.org 0x80076e3c
  jal toEulerAngles
.org 0x800770d8
  jal toEulerAngles
.org 0x80077234
  jal toEulerAngles
.org 0x80078658
  jal toEulerAngles
.org 0x80078b64
  jal toEulerAngles

.org 0x80070284
  jal getDistance
.org 0x800706d0
  jal getDistance
.org 0x80073354
  jal getDistance
.org 0x800741d4
  jal getDistance
.org 0x80074398
  jal getDistance
.org 0x80074c5c
  jal getDistance
.org 0x80074fdc
  jal getDistance
.org 0x80075060
  jal getDistance
.org 0x8007512c
  jal getDistance
.org 0x80075228
  jal getDistance
.org 0x80075cd4
  jal getDistance
.org 0x80076214
  jal getDistance
.org 0x8007639c
  jal getDistance
.org 0x80076dd0
  jal getDistance
.org 0x80077bfc
  jal getDistance
.org 0x80078268
  jal getDistance
.org 0x80078598
  jal getDistance
.org 0x80079494
  jal getDistance

.org 0x80077100
  jal matrixToEuler2
.org 0x80077140
  jal matrixToEuler2
.org 0x80079948
  jal matrixToEuler2

.close


.open "work/DIGIMON/BTL_REL.BIN",0x80052ae0
.psx

.org 0x8006a28c
  jal multiplyRotations

.org 0x8006a4e0
  jal calculatePosition

.org 0x8006ac1c
  jal toEulerAngles

.org 0x8006a9a8
  jal getDistance

.org 0x8006a32c
  jal matrixToEuler1
.org 0x8006aa50
  jal matrixToEuler1

.org 0x80068acc
  jal matrixToEuler2
.org 0x80068cf8
  jal matrixToEuler2
.org 0x80069264
  jal matrixToEuler2
.org 0x80069e70
  jal matrixToEuler2
.org 0x8006a4ec
  jal matrixToEuler2

.close


.open "work/DIGIMON/STD_REL.BIN",0x80052ae0
.psx

.org 0x80072898
  jal multiplyRotations

.org 0x80072aec
  jal calculatePosition

.org 0x80073228
  jal toEulerAngles

.org 0x8005a96c
  jal getDistance
.org 0x8005a998
  jal getDistance
.org 0x8005b7bc
  jal getDistance
.org 0x8005bb54
  jal getDistance
.org 0x80072fb4
  jal getDistance

.org 0x80072938
  jal matrixToEuler1
.org 0x8007305c
  jal matrixToEuler1

.org 0x80071064
  jal matrixToEuler2
.org 0x80071290
  jal matrixToEuler2
.org 0x800717fc
  jal matrixToEuler2
.org 0x8007247c
  jal matrixToEuler2
.org 0x80072af8
  jal matrixToEuler2

.close

.open "work/DIGIMON/VS_REL.BIN",0x80052ae0
.psx

.org 0x80068968
  jal multiplyRotations

.org 0x80068bbc
  jal calculatePosition

.org 0x800692f8
  jal toEulerAngles

.org 0x80059be8
  jal getDistance
.org 0x80059c14
  jal getDistance
.org 0x8005aa3c
  jal getDistance
.org 0x8005add4
  jal getDistance
.org 0x80069084
  jal getDistance

.org 0x80068a08
  jal matrixToEuler1
.org 0x8006912c
  jal matrixToEuler1

.org 0x80067134
  jal matrixToEuler2
.org 0x80067360
  jal matrixToEuler2
.org 0x800678cc
  jal matrixToEuler2
.org 0x8006854c
  jal matrixToEuler2
.org 0x80068bc8
  jal matrixToEuler2

.close