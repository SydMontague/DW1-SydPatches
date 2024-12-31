/*
  Parameter:
    a0: pointer to first position
    a1: pointer to second position

  Returns:
    v0: squared distance between both positions
*/
.func getDistanceSquared
  lw v0,0x00(a0)
  lw v1,0x00(a1)
  lw a2,0x08(a0)
  sub v0,v0,v1
  lw a3,0x08(a1)
  mult v0,v0
  sub a2,a2,a3
  mflo v0
  mult a2,a2
  mflo a2
  jr ra
  add v0,v0,a2
.endfunc

/*
  Parameter:
    a0: digimon Type ID

  Returns:
    v0: radius field from the Digimon Parameter
*/
.func getDigimonRadius
  li v0,DIGIMON_PARA
  sll v1,a0,0x01
  add v1,v1,a0
  sll v1,v1,0x02
  add v1,v1,a0
  sll v1,v1,0x02
  add v0,v0,v1
  lh v0,0x18(v0)
  jr ra
  nop
.endfunc

/*
  void jis_concat_glyph(char* str, char* glyph) {
    str = str + strlen(str)
    str[0] = glyph[0];

    int idx = 1;
    if(isTwoByte(glyph[0])) {
      str[idx] = glyph[idx];
      idx++; 
    }
    str[idx] = 0;
  }
*/
/*
  Parameter:
    a0: pointer to start of string
    a1: pointer to the glyph that is to be concatenated
*/
.func jis_concat_glyph
  addiu sp,sp,-0x20
  sw ra,0x10(sp)
  sw s0,0x14(sp)
  sw s1,0x18(sp)
  move s0,a0
  jal strlen
  move s1,a1
  addu s0,s0,v0
  lbu v0,0x00(s1)
  addiu s1,s1,0x01
  sb v0,0x00(s0)
  sgtu a0,v0,0x7F ; greater 0x7F -> two bytes
  beq a0,zero,@@_jis_concat_terminate
  addiu s0,s0,0x01
  lbu v0,0x00(s1)
  addiu s1,s1,0x01
  sb v0,0x00(s0)
  addiu s0,s0,0x01
@@_jis_concat_terminate:
  sb zero,0x00(s0)
  lw ra,0x10(sp)
  lw s0,0x14(sp)
  lw s1,0x18(sp)
  jr ra
  addiu sp,sp,0x20
.endfunc

/*
  char* jis_at_index(char* str, int index) {
    for(int i = 0; i < index; i++) {
      firstByte = *str;
      if(firstByte == 0)
        return str;
      
      str += isAsciiEncoded(firstByte) + 1
    }
  }
*/
/*
  Parameter:
    a0: pointer to start of string
    a1: index to look for

  Returns:
    v0: a pointer to the n-th glyph or the end of the string, if it is shorter
*/
.func jis_at_index
  li v0,0x00
@@_jis_at_index_loop_start:
  lbu v1,0x00(a0)
  beq v0,a1,@@_jis_at_index_end
  nop
  beq v1,zero,@@_jis_at_index_end
  nop
  addiu v0,v0,0x01
  sgtu a2,v1,0x7F ; greater 0x7F -> two bytes
  addu a0,a0,a2
  b @@_jis_at_index_loop_start
  addiu a0,a0,0x01
@@_jis_at_index_end:
  jr ra
  move v0,a0
.endfunc

/*
  int jis_len(char* str) {
    int i = 0;
    do {
      firstByte = *str;
      if (firstByte == 0)
        return i;
      
      i++;
      str += isAsciiEncoded(firstByte) + 1;
    } while(true);
  }
*/
/*
  Parameter:
    a0: pointer to start of string

  Returns:
    v0: the number of sjift-jis glypths in the string
    v1: the number of bytes in the string
*/
.func jis_len
  li v0,0x00
  li v1,0x00
@@_jis_len_loop_start:
  lbu a1,0x00(a0)
  nop
  beq a1,zero,@@_jis_len_end
  nop
  addiu v0,v0,0x01
  sgtu a2,a1,0x7F ; greater 0x7F -> two bytes
  addu a0,a0,a2
  addu v1,v1,a2
  addiu v1,v1,0x01
  b @@_jis_len_loop_start
  addiu a0,a0,0x01
@@_jis_len_end:
  jr ra
  nop
.endfunc

/*
  void jis_strncpy(char* dest, char* src, int32_t byte_cnt) {
    strncpy(dest, src, byte_cnt);
    dest[byte_cnt-1] = 0;
    if(!isAsciiEncoded(dest[byte_cnt-2])
      dest[byte_cnt-2] = 0;
  }
*/
/*
  Parameter: 
    a0: destination pointer
    a1: source pointer
    a2: max number of bytes
*/
.func jis_strncpy
  addiu sp,sp,-0x18
  sw ra,0x10(sp)
  sw s0,0x14(sp)
  jal strncpy
  add s0,a0,a2
  lbu a0,-0x02(s0)
  sgtu a0,v0,0x7F ; greater 0x7F -> two bytes
  beq a0,zero,@@_jis_strncpy_terminate
  sb zero,-0x01(s0)
  sb zero,-0x02(s0)
@@_jis_strncpy_terminate:
  lw ra,0x10(sp)
  lw s0,0x14(sp)
  jr ra
  addiu sp,sp,0x18
.endfunc

/*
  Clears a rect in the text area

  Parameter:
    a0: posX
    a1: posY
    a2: width
    a3: height
*/
.func clearTextSubArea2
  addiu sp,sp,-0x20
  sw ra,0x10(sp)
  sh a0,0x18(sp)
  sh a1,0x1a(sp)
  sh a2,0x1c(sp)
  sh a3,0x1e(sp)
  jal clearTextSubArea
  addiu a0,sp,0x18
  lw ra,0x10(sp)
  nop
  jr ra
  addiu sp,sp,0x20
.endfunc
