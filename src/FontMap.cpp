#include "font.h"

using namespace std;
FontMap::FontMap(FontResLib & lib, int charsize)
{
  lib.set_char_size(0, 0, charsize);
  for (unsigned char c = 0; c < 128; c++) {
    if (lib.load_char(0, c))
      cout << "Error on loading char " << c << endl;
    FT_Face &face = lib.get_face(0);
    codemap[(char)c] = new FontTexture(face);
  }
}
