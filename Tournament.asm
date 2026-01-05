.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x800efbfc
  jal updateTournamentRegistration

.close

.open "work/DIGIMON/DGET_REL.BIN",0x80080800
.psx

.org 0x80080900
  jal minutesOfDay

.org 0x80080f34
  jal getCupDataJumpTable
.org 0x80080f88
  jal getCupDataJumpTable
.org 0x80081010
  jal getCupDataJumpTable
.org 0x80081228
  jal getCupDataJumpTable

.org 0x80080f40
  jal getCupDataJumpTableEntry
.org 0x80080f94
  jal getCupDataJumpTableEntry
.org 0x80081020
  jal getCupDataJumpTableEntry
.org 0x80081030
  jal getCupDataJumpTableEntry
.org 0x80081198
  jal getCupDataJumpTableEntry
.org 0x80081234
  jal getCupDataJumpTableEntry

.close