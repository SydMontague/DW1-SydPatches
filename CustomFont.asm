
.func getGlyphVanilla
; write terminator
  li at,CHAR_TO_GLYPH_TABLE
  li v0,78
  sll v0,v0,1
  addu v0,v0,at
  sh a0,0x00(v0)
  li v0,0
@@_loop_start:
  lhu a1,0x00(at)
  nop
  beq a1,a0,@@_loop_end
  addiu at,at,2
  b @@_loop_start
  addiu v0,v0,1
@@_loop_end:
  sll at,v0,4
  sll v0,v0,3
  addu at,at,v0
  li v0,GLYPH_DATA
  jr ra
  addu v0,v0,at
.endfunc

.func getWidthVanilla
  addiu sp,sp,-0x18
  sw ra,0x10(sp)
  jal getGlyphVanilla
  nop
  lw ra,0x10(sp)
  lb v0,0x16(v0)
  jr ra
  addiu sp,sp,0x18
.endfunc

.func getRowVanilla
  addiu sp,sp,-0x18
  sw ra,0x10(sp)
  sw s0,0x14(sp)
  jal getGlyphVanilla
  sll s0,a1,1
  addu v0,v0,s0
  lhu v0,0x00(v0)
  lw ra,0x10(sp)
  lw s0,0x14(sp)
  jr ra
  addiu sp,sp,0x18
.endfunc

.func getCodePointVanilla
  addiu sp,sp,-0x18
  sw ra,0x10(sp)
  jal jis_at_index
  nop
  lbu a0,0x00(v0)
  lbu a1,0x01(v0)
  bltu a0,0x80,@@_else
  sll v0,a0,0x08
  b @@_return
  addu v0,v0,a1
@@_else:
  li v0,0x2e ; dot
  beq a0,v0,@@_return
  li v0,0x8142
  li v0,0x27 ; apostroph
  beq a0,v0,@@_return
  li v0,0x8175
  jal convertAsciiToGameChar
  nop
  ;jal swapShortBytes
  move a0,v0
@@_return:
  lw ra,0x10(sp)
  nop
  jr ra
  addiu sp,sp,0x18
.endfunc

vanillaFont:
  .word 11                  ; height
  .word getWidthVanilla     ; getWidth
  .word getRowVanilla       ; getRow
  .word getCodePointVanilla ; getCodePoint
RENDER_AREA_POINTER:
  .word 0
RENDER_AREA:
  .fill 128 * 32

/*
  getRenderArea(uint32_t height, uint32_t width) {
    uint32_t size = height * width
    if (RENDER_AREA_POINTER + size > 128 * 32)
      RENDER_AREA_POINTER = 0;

    uint8_t* ptr = RENDER_AREA + RENDER_AREA_POINTER;

    RENDER_AREA_POINTER += size;
    return ptr;
  }
*/
.func getRenderArea
  addiu a1,a1,3
  sra a1,a1,2
  sll a1,a1,2
  mult a0,a1
  li v1,RENDER_AREA_POINTER
  lw a2,0x00(v1)
  mflo a0
  addu a3,a2,a0
  slti at,a3,128*32
  bne at,zero,@@_endif
  nop
  move a2,zero
@@_endif:
  li v0,RENDER_AREA
  addu v0,v0,a2
  jr ra
  sw s3,0x00(v1)
.endfunc

/*
uint32_t getStringWidth(Font* font, char* string) {
  uint32_t index = 0;
  uint16_t codepoint;
  do {
    codepoint = font->getCodePoint(string, index++);
    width += font->getWidth(codepoint);
  } while(codepoint != 0);
}

*/
; s0 -> getCodePoint
; s1 -> getWidth
; s2 -> i
; s3 -> width
; s4 -> string
.func getStringWidth
  addiu sp,sp,-0x28
  sw ra,0x10(sp)
  sw s0,0x14(sp)
  sw s1,0x18(sp)
  sw s2,0x1C(sp)
  sw s3,0x20(sp)
  sw s4,0x24(sp)
  lw s0,0x0C(a0)
  lw s1,0x04(a0)
  move s2,zero
  move s3,zero
  move s4,a1
@@_loop_start:
  move a0,s4
  jalr s0,ra ; getCodePoint
  move a1,s2
  beq v0,zero,@@_end
  nop
  jalr s1,ra ; getWidth
  move a0,v0
  addu s3,s3,v0
  b @@_loop_start
  addiu s2,s2,1
@@_end:
  move v0,s3
  lw ra,0x10(sp)
  lw s0,0x14(sp)
  lw s1,0x18(sp)
  lw s2,0x1C(sp)
  lw s3,0x20(sp)
  lw s4,0x24(sp)
  jr ra
  addiu sp,sp,0x28
.endfunc


/*
// outdated
uint16_t drawString(Font* font, char* string, uint16_t start_x, uint16_t start_y) {
  uint8_t* render_area = getRenderArea(font->height); 
  uint32_t width = 0;
  uint8_t index = 0;

  do {
    uint16_t codepoint = font->getCodePoint(string, index++);
    if(codepoint == 0)
      break;

    width += drawGlyph(font, codepoint, render_area, width)
  } while(true);

  // get final width
  width = (width + 3) / 4;

  Rect rect;
  rect.width = width;
  rect.height = font->height;
  rect.x = start_x;
  rect.y = start_y;
  libgpu::LoadImage(&rect, pixel_data);

  return width;
}
*/
; s0 -> font_ptr
; s1 -> string_ptr
; s2 -> height
; s3 -> width
; s4 -> index
; s5 -> render_area
.func drawStringNew
; preamble
  addiu sp,sp,-0x40
  sw ra,0x10(sp)
  sw s0,0x14(sp)
  sw s1,0x18(sp)
  sw s2,0x1C(sp)
  sw s3,0x20(sp)
  sw s4,0x24(sp)
  sw s5,0x28(sp)
; fill variables
  move s0,a0
  move s1,a1
  lw s2,0x00(s0)
  move s3,zero
  move s4,zero
  sh a2,0x38(sp) ; rect.x
  sh a3,0x3A(sp) ; rect.y
  sh s2,0x3E(sp) ; rect.height
  move a0,s0
  jal getStringWidth
  move a1,s1
  sw v0,0x30(sp)
  move a1,v0
  addiu v0,v0,3
  sra v0,v0,2
  sh v0,0x3C(sp) ; rect.width
  jal getRenderArea
  move a0,s2
  move s5,v0
  move a0,s5
  move a1,zero
  jal memset
  sll a2,s2,0x07
@@_loop_start:
  lw v0,0x0C(s0)
  move a0,s1
  jalr v0,ra  ; getCodePoint
  move a1,s4
  beq v0,zero,@@_end_loop
  addiu s4,s4,0x01
  move a0,s0
  move a1,v0
  move a2,s5
  jal drawGlyphNew
  move a3,s3
  b @@_loop_start
  addu s3,s3,v0
@@_end_loop:
  addiu a0,sp,0x38
  jal libgpu_LoadImage
  move a1,s5
; return
  move v0,s3
  lw ra,0x10(sp)
  lw s0,0x14(sp)
  lw s1,0x18(sp)
  lw s2,0x1C(sp)
  lw s3,0x20(sp)
  lw s4,0x24(sp)
  lw s5,0x28(sp)
  jr ra
  addiu sp,sp,0x40
.endfunc


/*
uint16_t drawGlyph(Font* font, uint16_t codepoint, uint8_t* render_area, uint8_t offset_x) {
  uint8_t* render_start = render_area + offset_x / 2;
  uint8_t glyph_width = font->getWidth(codepoint);

  for(int i = 0; i < font->height; i++) {
    uint16_t row_data = font->getRow(codepoint, i);
    uint8_t* render_row = render_start + 64 * i;
    uint8_t width = glyph_width;

    if (offset_x % 2 == 1) 
    {
      uint8_t tmp = *render_row;
      if ((row_data & 0x8000) == 0)
        tmp = tmp | COLORCODE_HIGHBITS
      *render_row = tmp;
      render_row++;
      width--;
      row_data = row_data << 1;
    }

    for (int j = 0; j < width; j += 2) {
      uint8_t tmp = 0;
      if ((row_data & 0x8000) == 0)
        tmp = COLORCODE_LOWBITS;
      if ((row_data & 0x4000) == 0)
        tmp = tmp | COLORCODE_HIGHBITS;
      *render_row = tmp;
      render_row++;
      row_data = row_data << 2;
    }
  }

  return glyph_width;
}
*/
; s0 -> font_ptr
; s1 -> codepoint
; s2 -> render_start
; s3 -> is_odd
; s4 -> font_height
; s5 -> index
; s6 -> glyph_width
; s7 -> string_width
.func drawGlyphNew
; preamble
  addiu sp,sp,-0x40
  sw ra,0x10(sp)
  sw s0,0x14(sp)
  sw s1,0x18(sp)
  sw s2,0x1C(sp)
  sw s3,0x20(sp)
  sw s4,0x24(sp)
  sw s5,0x28(sp)
  sw s6,0x2C(sp)
  sw s7,0x30(sp)
; fill variables
  move s0,a0
  move s1,a1
  sra s2,a3,1
  addu s2,s2,a2
  andi s3,a3,1
  lw s7,0x70(sp)
  lw s4,0x00(s0)
  lw v0,0x04(s0)
  move s5,zero
  jalr v0,ra ; getWidth
  move a0,s1
  move s6,v0
; loop
  b @@_outer_loop_condition
  nop
  @@_outer_loop_head:
    lw v0,0x08(s0)
    move a0,s1
    jalr v0,ra ; getRow
    move a1,s5
    ; v0 -> row_data
    ; t0 -> render_row
    ; t1 -> width
    ; t2 -> COLORCODE_HIGHBITS
    ; t3 -> COLORCODE_LOWBITS
    ; t4 -> j
    addiu s7,s7,3
    sra s7,s7,2
    sll s7,s7,2
    mult s5,s7
    mflo at
    sra at,at,1
    addu t0,s2,at
    move t1,s6
    lbu t2,-0x74cf(gp)
    lbu t3,-0x74d0(gp)
  ; odd offset handling
    beq s3,zero,@@_odd_handling_end
    nop
      lbu at,0x00(t0)
      andi a2,v0,0x8000
      bne a2,zero,@@_odd_if_bit_set_end
      nop
      or at,at,t2
    @@_odd_if_bit_set_end:
      sb at,0x00(t0)
      addiu t0,t0,1
      addiu t1,t1,-1
      sll v0,v0,1
  @@_odd_handling_end:
  ; inner loop start
    b @@_inner_loop_condition
    move t4,zero
  @@_inner_loop_head:
      move at,zero
      andi a2,v0,0x8000
      bne a2,zero,@@_inner_loop_low_end
      nop
      or at,at,t3
    @@_inner_loop_low_end:
      andi a2,v0,0x4000
      bne a2,zero,@@_inner_loop_high_end
      nop
      or at,at,t2
    @@_inner_loop_high_end:
      sb at,0x00(t0)
      addiu t0,t0,1
      sll v0,v0,2
      addiu t4,t4,2
  @@_inner_loop_condition:
    slt at,t4,t1
    bne at,zero,@@_inner_loop_head
    nop
    addiu s5,s5,1
@@_outer_loop_condition:
  slt at,s5,s4
  bne at,zero,@@_outer_loop_head
  nop
; return
  move v0,s6
  lw ra,0x10(sp)
  lw s0,0x14(sp)
  lw s1,0x18(sp)
  lw s2,0x1C(sp)
  lw s3,0x20(sp)
  lw s4,0x24(sp)
  lw s5,0x28(sp)
  lw s6,0x2C(sp)
  lw s7,0x30(sp)
  jr ra
  addiu sp,sp,0x40
.endfunc

/*




class Font {
  uint8_t height;

  uint8_t getWidth(uint16_t codepoint);
  uint16_t getRow(uint16_t codepoint, uint8_t row);
  uint16_t getCodePoint(uint8_t* string, uint32_t index);
}




*/