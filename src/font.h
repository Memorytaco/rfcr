#ifndef RFCFONT
#define RFCFONT

#include "core.h"

#include <ft2build.h>
#include FT_FREETYPE_H

class FontResLib {
  public:
    FontResLib() { FT_Init_FreeType(&library); }
    ~FontResLib() { }

    void add_fontface(std::string base, std::string name, int index);
    void set_char_size(int idx, int w, int h, int hdpi, int vdpi);
    void set_char_size(int idx, int w, int h);
    int get_glyf_index(int idx, char c);
    bool load_char(int idx, char c);
    FT_Face& get_face(int idx) { return faces.at(idx); }
  private:
    FT_Library library;
    std::vector<FT_Face> faces{};
};

class FontTexture {
  public:
    FontTexture() {}
    FontTexture(FT_Face &face);
    FontTexture(const FontTexture& font) {
      id = font.id;
      size = font.size;
      bearing = font.bearing;
      advance = font.advance;
    }
    // TODO: clean up the resources
    ~FontTexture() {}

    GLuint id;
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned int advance;
};

class FontMap {
  std::map<char, FontTexture const*> codemap;

  public:
  // TODO: add clean up function
    FontMap(FontResLib& lib, int charsize);
    inline int getLineHeight() { return codemap['W']->size.y; }
    inline const FontTexture& operator[](char c) { return *codemap[c]; }
};

#endif
