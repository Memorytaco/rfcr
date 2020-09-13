#include "font.h"

using std::string;
using std::endl;
using std::cout;

void FontResLib::add_fontface(string base, std::string name, int index)
{
  FT_Face face;
  FT_New_Face(library, (base + name).c_str(), index, &face);
  faces.push_back(face);
}

void FontResLib::set_char_size(int idx, int w, int h, int hdpi, int vdpi)
{
  FT_Face& face = faces.at(idx);
  FT_Set_Char_Size(face, w, h, hdpi, vdpi);
}

void FontResLib::set_char_size(int idx, int w, int h) 
{
  FT_Face& face = faces.at(idx);
  FT_Set_Pixel_Sizes(face, w, h);
}

int FontResLib::get_glyf_index(int idx, char c)
{
  FT_Face& face = faces.at(idx);
  return FT_Get_Char_Index(face ,c);
}

bool FontResLib::load_char(int idx, char c) 
{
  FT_Face& face = faces.at(idx);
  return FT_Load_Char(face, c, FT_LOAD_RENDER);
}

