/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include <dma.h>
#include <graph.h>
#include <gs_psm.h>
#include "renderer/core/gs/renderer_core_gs_vram.hpp"

#define GS_VRAM_TEXTURE_ALIGNMENT GRAPH_ALIGN_BLOCK
#define GS_VRAM_BUFFER_ALIGNMENT GRAPH_ALIGN_PAGE

namespace Tyra {

RendererCoreGSVRam::RendererCoreGSVRam() {
  touched = false;
  pointer = 0;
  cachedFreeSpace = getFreeSpaceInMB();
}

RendererCoreGSVRam::~RendererCoreGSVRam() {}

const float& RendererCoreGSVRam::getFreeSpaceInMB() {
  if (touched) {
    cachedFreeSpace = (GRAPH_VRAM_MAX_WORDS - pointer) / ptr2MB;
    touched = false;
  }

  return cachedFreeSpace;
}

float RendererCoreGSVRam::getSizeInMB(const Texture& texture) {
  float result = getSizeInMB(texture.getCoreData());

  if (texture.getClutData() != nullptr) {
    result += getSizeInMB(*texture.getClutData());
  }

  return result;
}

float RendererCoreGSVRam::getSizeInMB(const TextureData& texData) {
  return getSizeInMB(texData.width, texData.height, texData.psm,
                     GS_VRAM_TEXTURE_ALIGNMENT);
}

float RendererCoreGSVRam::getSizeInMB(int width, const int& height,
                                      const int& psm, const int& alignment) {
  return getSize(width, height, psm, alignment) / ptr2MB;
}

int RendererCoreGSVRam::allocate(const TextureData& texData) {
  return allocate(texData.width, texData.height, texData.psm,
                  GS_VRAM_TEXTURE_ALIGNMENT);
}

int RendererCoreGSVRam::allocateBuffer(const int& width, const int& height,
                                       const int& psm) {
  return allocate(width, height, psm, GS_VRAM_BUFFER_ALIGNMENT);
}

int RendererCoreGSVRam::allocate(const int& width, const int& height,
                                 const int& psm, const int& alignment) {
  auto size = getSize(width, height, psm, alignment);

  pointer += size;

  // If the pointer overflows the vram size
  if (pointer > GRAPH_VRAM_MAX_WORDS) {
    pointer -= size;
    return -1;
  }

  touched = true;

  return pointer - size;
}

void RendererCoreGSVRam::free(const int& address) {
  pointer = address;
  touched = true;
}

int RendererCoreGSVRam::getSize(int width, const int& height, const int& psm,
                                const int& alignment) {
  int size = 0;

  // First correct the buffer width to be a multiple of 64 or 128
  // If the width is less than or equal to 16, then it's a palette
  if (width > 16) {
    switch (psm) {
      case GS_PSM_8:
      case GS_PSM_4:
      case GS_PSM_8H:
      case GS_PSM_4HL:
      case GS_PSM_4HH:
        width = -128 & (width + 127);
        break;
      default:
        width = -64 & (width + 63);
        break;
    }
  }

  // Texture storage size is in pixels/word
  switch (psm) {
    case GS_PSM_4:
      size = width * (height >> 3);
      break;
    case GS_PSM_8:
      size = width * (height >> 2);
      break;
    case GS_PSM_24:
    case GS_PSM_32:
    case GS_PSM_8H:
    case GS_PSM_4HL:
    case GS_PSM_4HH:
    case GS_PSMZ_24:
    case GS_PSMZ_32:
      size = width * height;
      break;
    case GS_PSM_16:
    case GS_PSM_16S:
    case GS_PSMZ_16:
    case GS_PSMZ_16S:
      size = width * (height >> 1);
      break;
    default:
      return 0;
  }

  if (alignment == GS_VRAM_TEXTURE_ALIGNMENT) {
    // TODO: Without this hack, textures are overlapping ourselves
    size += 1024 * 2;
  }

  // The buffer size is dependent on alignment
  size = -alignment & (size + (alignment - 1));

  return size;
}

}  // namespace Tyra
