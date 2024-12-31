#pragma once
#include "dw1.hpp"

extern "C" {
typedef uint8_t (*GetWidthFn)(uint16_t);
typedef uint16_t (*GetRowFn)(uint16_t, uint8_t);
typedef uint16_t (*GetCodePointFn)(uint8_t *, uint32_t);


struct CustomFont {
  uint8_t height;
  bool isWide;
  uint16_t mappingCount;
};

struct GlyphMapping {
  uint16_t codepoint;
  uint16_t index;
};

struct Glyph5PX {
  uint8_t width;
  uint8_t rows[6]; // height
};

typedef struct {
  uint8_t height;
  GetWidthFn getWidth;
  GetRowFn getRow;
  GetCodePointFn getCodePoint;
} Font;

extern Font vanillaFont;

GlyphData *getGlyphVanilla(uint16_t codepoint);
uint8_t getWidthVanilla(uint16_t codepoint);
uint16_t getRowVanilla(uint16_t codepoint, uint8_t row);
uint16_t getCodePointVanilla(uint8_t *string, uint32_t index);
}