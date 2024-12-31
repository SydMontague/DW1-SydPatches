#include "Font.hpp"
#include "dw1.hpp"

extern "C" {
uint32_t RENDER_AREA_POINTER = 0;
uint8_t RENDER_AREA[128 * 32] = {0};

Font vanillaFont = {
    .height = 11,
    .getWidth = getWidthVanilla,
    .getRow = getRowVanilla,
    .getCodePoint = getCodePointVanilla,
};

GlyphData *getGlyphVanilla(uint16_t codepoint) {
  CHAR_TO_GLYPH_TABLE[78] = codepoint;
  uint32_t i = 0;
  for (; CHAR_TO_GLYPH_TABLE[i] != codepoint; i++)
    ;

  return GLYPH_DATA + i;
}

uint8_t getWidthVanilla(uint16_t codepoint) {
  return getGlyphVanilla(codepoint)->width;
}

uint16_t getRowVanilla(uint16_t codepoint, uint8_t row) {
  return getGlyphVanilla(codepoint)->pixelData[row];
}

uint16_t getCodePointVanilla(uint8_t *string, uint32_t index) {
  uint8_t *ptr = jis_at_index(string, index);
  uint8_t firstByte = *ptr;
  if (firstByte >= 0x80) {
    uint16_t bla = firstByte << 8 | ptr[1];
    return bla;
  }
  if (firstByte == 0x2e)
    return 0x8142;
  if (firstByte == 0x27)
    return 0x8175;

  return convertAsciiToJis(firstByte);
}

uint8_t *getRenderArea(uint32_t height, uint32_t width) {
  uint32_t size = height * ((width + 3) / 4) * 4;

  if (RENDER_AREA_POINTER + size > 128 * 32)
    RENDER_AREA_POINTER = 0;

  uint8_t *ptr = RENDER_AREA + RENDER_AREA_POINTER;

  RENDER_AREA_POINTER += size;
  return ptr;
}

uint32_t getStringWidth(Font *font, uint8_t *string) {
  GetCodePointFn getCodePoint = font->getCodePoint;
  GetWidthFn getWidth = font->getWidth;
  uint32_t index = 0;
  uint32_t width = 0;
  uint16_t codepoint;
  do {
    codepoint = getCodePoint(string, index++);
    width += getWidth(codepoint);
  } while (codepoint != 0);

  return width;
}

uint16_t drawGlyphNew(Font *font, uint16_t codepoint, uint8_t *render_area,
                      uint8_t offset_x, uint32_t string_width) {
  uint8_t *render_start = render_area + offset_x / 2;
  uint8_t font_height = font->height;
  uint8_t glyph_width = font->getWidth(codepoint);

  for (int i = 0; i < font->height; i++) {
    uint16_t row_data = font->getRow(codepoint, i);
    uint8_t *draw_row = render_start + (((string_width + 3) / 4) * 4 * i) / 2;
    uint8_t width = glyph_width;

    if (offset_x % 2 == 1) {
      uint8_t tmp = *draw_row;
      if ((row_data & 0x8000) == 0)
        tmp = tmp | COLORCODE_HIGHBITS;
      *draw_row = tmp;
      draw_row++;
      width--;
      row_data = row_data << 1;
    }

    for (int j = 0; j < width; j += 2) {
      uint8_t tmp = 0;
      if ((row_data & 0x8000) == 0)
        tmp = COLORCODE_LOWBITS;
      if ((row_data & 0x4000) == 0)
        tmp = tmp | COLORCODE_HIGHBITS;
      *draw_row = tmp;
      draw_row++;
      row_data = row_data << 2;
    }
  }

  return glyph_width;
}

uint16_t drawStringNew(Font *font, uint8_t *string, int16_t start_x,
                       int16_t start_y) {
  uint32_t string_width = getStringWidth(font, string);
  uint16_t font_height = font->height;
  uint8_t *render_area = getRenderArea(font->height, string_width);
  memset(render_area, 0, string_width * font_height);

  RECT rect = {
      .x = start_x,
      .y = start_y,
      .width = static_cast<int16_t>((string_width + 3) / 4),
      .height = static_cast<int16_t>(font->height),
  };

  uint32_t index = 0;
  uint32_t width = 0;
  do {
    uint16_t codepoint = font->getCodePoint(string, index++);
    if (codepoint == 0)
      break;

    width += drawGlyphNew(font, codepoint, render_area, width, string_width);
  } while (true);

  libgpu_LoadImage(&rect, reinterpret_cast<uint32_t *>(render_area));
  return width;
}
}