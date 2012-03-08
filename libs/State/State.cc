#include <State/State.h>
#include <Type/TFM.h>
#include <Render/Units.h>

using namespace tex;

State::State(void) {
  // first initialize ccode to all "CC_OTHER_CHAR" as the default code.
  for (unsigned i = 0; i < 128; i++) {
    ccode[i] = CC_OTHER_CHAR;
  }

    // now initialize letters.
  for (unsigned i = 0; i < 26; i++) {
    ccode['A' + i] = CC_LETTER;
    ccode['a' + i] = CC_LETTER;
  }
  
  ccode[0x00] = CC_IGNORE;
  ccode[0x20] = CC_SPACER; // ' ' 
  ccode[0x5C] = CC_ESCAPE; // '\\'
  ccode[0x25] = CC_COMMENT; // '%'
  ccode[0x7F] = CC_INVALID;
  ccode[0x0A] = CC_CAR_RET; // '\n'; technically deviates from tex.
  ccode[0x0D] = CC_CAR_RET; // '\r'

  // load the null font.
  Font f;
  fonts.append(f);

  // and load computer modern.
  curr_font = load_font("cmr10.tfm", -1000);

  // Internal variables
  tex_mem[LEFT_SKIP_CODE].scaled = scaled(0);
  tex_mem[RIGHT_SKIP_CODE].scaled = scaled(0);
  tex_mem[HSIZE_CODE].scaled = scaled_from(0x67FFF, UNIT_IN); // 6.5in
  tex_mem[VSIZE_CODE].scaled = scaled_from(9 << 16, UNIT_IN); // 9in
  tex_mem[PARINDENT_CODE].scaled = scaled(18 << 16);
  tex_mem[BASELINE_SKIP_CODE].scaled = scaled(12 << 16);

  // enter vmode.
  r_state.set_mode(VMODE);

  // initialize primitives.
  primitive("par", CC_PAR_END, (word){0});
  primitive("end", CC_STOP, (word){0});
}

void State::init(UniquePtr<State> &result) {
    result.reset(new State());
}

uint32_t State::load_font(const char *path, int32_t at) {
  uint32_t f = fonts.entries();
  Font empty_font;
  fonts.append(empty_font);
  Font &font = fonts.get(f);
  TFM::load_font(path, font, at);
  return f;
}
