/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/core/texture/texture_data.hpp"
#include "debug/debug.hpp"
#include <gs_psm.h>
#include <draw_buffers.h>

namespace Tyra {

TextureData::TextureData(unsigned char* t_data, const TextureBpp& t_bpp,
                         const unsigned char& t_gsComponents,
                         const int& t_width, const int& t_height) {
  data = t_data;
  bpp = t_bpp;
  components = t_gsComponents;
  width = t_width;
  height = t_height;
  psm = getPsmByBpp(t_bpp);
}

TextureData::~TextureData() {
  if (data) {
    delete[] data;
  }
}

unsigned int TextureData::getPsmByBpp(const TextureBpp& bpp) {
  if (bpp == bpp32) {
    return GS_PSM_32;
  } else if (bpp == bpp24) {
    return GS_PSM_24;
  } else if (bpp == bpp8) {
    return GS_PSM_8;
  } else if (bpp == bpp4) {
    return GS_PSM_4;
  } else {
    TYRA_TRAP("Unknown bpp!");
    return 0;
  }
}

void TextureData::print() const {
  auto text = getPrint(nullptr);
  printf("%s\n", text.c_str());
}

void TextureData::print(const unsigned char* name) const {
  auto text = getPrint(name);
  printf("%s\n", text.c_str());
}

std::string TextureData::getPrint(const unsigned char* objectName) const {
  std::stringstream res;
  if (objectName) {
    res << objectName << "(";
  } else {
    res << "TextureData(";
  }

  std::string psmString;
  if (psm == GS_PSM_32)
    psmString = "GS_PSM_32";
  else if (psm == GS_PSM_24)
    psmString = "GS_PSM_24";
  else if (psm == GS_PSM_8)
    psmString = "GS_PSM_8";
  else if (psm == GS_PSM_4)
    psmString = "GS_PSM_4";

  std::string componentsString;
  if (components == TEXTURE_COMPONENTS_RGBA)
    componentsString = "TEXTURE_COMPONENTS_RGBA";
  else if (components == TEXTURE_COMPONENTS_RGB)
    componentsString = "TEXTURE_COMPONENTS_RGB";

  res << "data exist: " << static_cast<int>(data != nullptr) << ", ";
  res << "bpp: " << static_cast<unsigned int>(bpp) << ", ";
  res << "psm: " << psmString << ", ";
  res << "width: " << width << ", ";
  res << "height: " << height << ", ";
  res << "components: " << componentsString << ")";

  return res.str();
}

}  // namespace Tyra
