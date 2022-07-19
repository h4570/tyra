/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include <iomanip>
#include <draw_buffers.h>
#include <gs_psm.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include "renderer/core/texture/models/texture.hpp"

namespace Tyra {

Texture::Texture(TextureBuilderData* t_data) {
  id = rand() % 1000000;

  name = t_data->name;

  core = new TextureData(t_data->data, t_data->bpp, t_data->gsComponents,
                         t_data->width, t_data->height);

  clut =
      new TextureData(t_data->clut, t_data->clutBpp, t_data->clutGsComponents,
                      t_data->clutWidth, t_data->clutHeight);

  setDefaultWrapSettings();
}

Texture::~Texture() {
  if (getTextureLinksCount() > 0) links.clear();
  if (core) delete core;
  if (clut) delete clut;
}

float Texture::getSizeInMB() const {
  return (core->width / 100.0F) * (core->height / 100.0F) *
         (core->bpp / 100.0F) / 8.0F;
}

/** Based on gsKit code, thank you guys! */
u32 Texture::getTextureSize() const {
  int widthBlocks, heightBlocks;
  int widthAlign, heightAlign;

  // Calculate the number of blocks width and height
  // A block is 256 bytes in size
  switch (core->bpp) {
    case bpp32:
    case bpp24:
      // 1 block = 8x8 pixels
      widthBlocks = (core->width + 7) / 8;
      heightBlocks = (core->height + 7) / 8;
      break;

    case bpp8:
      // 1 block = 16x16 pixels
      widthBlocks = (core->width + 15) / 16;
      heightBlocks = (core->height + 15) / 16;
      break;
    case bpp4:
      // 1 block = 32x16 pixels
      widthBlocks = (core->width + 31) / 32;
      heightBlocks = (core->height + 15) / 16;
      break;
    default:
      TYRA_TRAP("Unknown texture bpp");
      return -1;
  }

  // Calculate the minimum block alignment
  if (core->bpp == bpp32 || core->bpp == bpp24 || core->bpp == bpp8) {
    // 8x4 blocks in a page.
    // block traversing order:
    // 0145....
    // 2367....
    // ........
    // ........
    if (widthBlocks <= 2 && heightBlocks <= 1) {
      widthAlign = 1;
      heightAlign = 1;
    } else if (widthBlocks <= 4 && heightBlocks <= 2) {
      widthAlign = 2;
      heightAlign = 2;
    } else if (widthBlocks <= 8 && heightBlocks <= 4) {
      widthAlign = 4;
      heightAlign = 4;
    } else {
      widthAlign = 8;
      heightAlign = 4;
    }
  } else {
    if (widthBlocks <= 1 && heightBlocks <= 2) {
      widthAlign = 1;
      heightAlign = 1;
    } else if (widthBlocks <= 2 && heightBlocks <= 2) {
      widthAlign = 2;
      heightAlign = 2;
    } else if (widthBlocks <= 2 && heightBlocks <= 8) {
      widthAlign = 2;
      heightAlign = 8;
    } else {
      widthAlign = 4;
      heightAlign = 8;
    }
  }

  widthBlocks = (-widthAlign) & (widthBlocks + widthAlign - 1);
  heightBlocks = (-heightAlign) & (heightBlocks + heightAlign - 1);

  return widthBlocks * heightBlocks * 256;
}

void Texture::setDefaultWrapSettings() {
  wrap.horizontal = WRAP_REPEAT;
  wrap.vertical = WRAP_REPEAT;
  wrap.maxu = 0;
  wrap.maxv = 0;
  wrap.minu = 0;
  wrap.minv = 0;
}

void Texture::setWrapSettings(const TextureWrap t_horizontal,
                              const TextureWrap t_vertical) {
  wrap.horizontal = t_horizontal;
  wrap.vertical = t_vertical;
}

void Texture::addLink(const u32& t_id) {
  TextureLink link;
  link.id = t_id;
  links.push_back(link);
}

void Texture::print() const {
  auto text = getPrint(nullptr);
  printf("%s\n", text.c_str());
}

void Texture::print(const char* name) const {
  auto text = getPrint(name);
  printf("%s\n", text.c_str());
}

std::string Texture::getPrint(const char* objectName) const {
  std::stringstream res;
  if (objectName) {
    res << objectName << "(";
  } else {
    res << "Texture(";
  }

  std::string wrapString;
  if (wrap.horizontal == WRAP_REPEAT)
    wrapString = "WRAP_REPEAT";
  else if (wrap.horizontal == WRAP_CLAMP)
    wrapString = "WRAP_CLAMP";

  res << "id: " << id << ", ";
  res << "name: " << name << ", ";
  res << "core: " << core->getPrint() << ", ";
  if (clut != nullptr) res << "clut: " << clut->getPrint() << ", ";
  res << "wrap: " << wrapString << ")";
  return res.str();
}

}  // namespace Tyra
