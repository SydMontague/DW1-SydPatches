
.open "work/DIGIMON/ETCDAT/ETCTIM.BIN",0

; change Status View badges to be only backgrounds, no text
.org 0x2821+0x01A8A0
  .fill 25,0x33
.org 0x28A1+0x01A8A0
  .fill 25,0x22
.org 0x2921+0x01A8A0
  .fill 25,0x33
.org 0x29A1+0x01A8A0
  .fill 25,0x33
.org 0x2A21+0x01A8A0
  .fill 25,0x33
.org 0x2AA1+0x01A8A0
  .fill 25,0x44
.org 0x2B21+0x01A8A0
  .fill 25,0x55
.org 0x2BA1+0x01A8A0
  .fill 25,0x66
.org 0x2C21+0x01A8A0
  .fill 25,0x66

; Add tiredness icon
.org 0x01A8A0+0x220+75*128+54/2
.byte 0xFF, 0xEF, 0xFF, 0xFF, 0xFF
.org 0x01A8A0+0x220+76*128+54/2
.byte 0xFF, 0x0E, 0xFE, 0xFF, 0xFF
.org 0x01A8A0+0x220+77*128+54/2
.byte 0xFF, 0x0E, 0xFE, 0xFF, 0xFF
.org 0x01A8A0+0x220+78*128+54/2
.byte 0xEF, 0x01, 0xE1, 0xFF, 0xFF
.org 0x01A8A0+0x220+79*128+54/2
.byte 0xEF, 0x01, 0xE1, 0xFF, 0xFF
.org 0x01A8A0+0x220+80*128+54/2
.byte 0x2E, 0x00, 0x20, 0xFE, 0xFF
.org 0x01A8A0+0x220+81*128+54/2
.byte 0x1E, 0x00, 0x10, 0xFE, 0xFF
.org 0x01A8A0+0x220+82*128+54/2
.byte 0x0E, 0x00, 0x00, 0xFE, 0xFF
.org 0x01A8A0+0x220+83*128+54/2
.byte 0x1E, 0x00, 0x10, 0xFE, 0xFF
.org 0x01A8A0+0x220+84*128+54/2
.byte 0x2E, 0x11, 0x21, 0xFE, 0xFF
.org 0x01A8A0+0x220+85*128+54/2
.byte 0xEF, 0xEE, 0xEE, 0xFF, 0xFF

.close