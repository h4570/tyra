/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/core/texture/renderer_core_texture_sender.hpp"
#include <gs_psm.h>

namespace Tyra {

RendererCoreTextureSender::RendererCoreTextureSender() {
  isTextureVRAMAllocated = false;
  allocatedVRamMemForTextures = 0;
}
RendererCoreTextureSender::~RendererCoreTextureSender() {}

void RendererCoreTextureSender::init(Path3* t_path3, RendererCoreGS* t_gs) {
  gs = t_gs;
  path3 = t_path3;
  TYRA_LOG("Renderer texture initialized!");
}

RendererCoreTextureBuffers RendererCoreTextureSender::allocate(
    Texture* t_texture) {
  texbuffer_t* core = allocateTextureCore(t_texture);
  texbuffer_t* clut = nullptr;

  auto texClut = t_texture->getClutData();
  if (texClut != nullptr && texClut->width > 0) {
    clut = allocateTextureClut(t_texture);
  }
  return {t_texture->getId(), core, clut};
}

float RendererCoreTextureSender::getSizeInMB(texbuffer_t* texBuffer) {
  auto bpp = getBppByPsm(texBuffer->psm);
  auto width = pow(2, texBuffer->info.width);
  auto height = pow(2, texBuffer->info.height);
  return (width / 100.0F) * (height / 100.0F) * (bpp / 100.0F) / 8.0F;
}

void RendererCoreTextureSender::deallocate(
    const RendererCoreTextureBuffers& texBuffers) {
  if (texBuffers.clut != nullptr && texBuffers.clut->width > 0) {
    graph_vram_free(texBuffers.clut->address);
    delete texBuffers.clut;
  }

  graph_vram_free(texBuffers.core->address);
  delete texBuffers.core;

  allocatedVRamMemForTextures -= getSizeInMB(texBuffers.core);
}

texbuffer_t* RendererCoreTextureSender::allocateTextureCore(
    Texture* t_texture) {
  auto* result = new texbuffer_t;
  result->width = t_texture->getWidth();
  result->psm = t_texture->getCoreData().psm;
  result->info.components = t_texture->getCoreData().components;

  TYRA_ASSERT(t_texture->getSizeInMB() <= getFreeVRamInMB(),
              "Not enough VRAM memory for texture!");

  auto address =
      graph_vram_allocate(t_texture->getWidth(), t_texture->getHeight(),
                          result->psm, GRAPH_ALIGN_BLOCK);
  TYRA_ASSERT(address > 0, "Texture buffer allocation error, no memory!");
  result->address = address;

  allocatedVRamMemForTextures += t_texture->getSizeInMB();
  result->info.width = draw_log2(t_texture->getWidth());
  result->info.height = draw_log2(t_texture->getHeight());
  result->info.function = TEXTURE_FUNCTION_MODULATE;
  return result;
}

texbuffer_t* RendererCoreTextureSender::allocateTextureClut(
    Texture* t_texture) {
  auto* result = new texbuffer_t;
  const auto* clut = t_texture->getClutData();

  result->width = clut->width;
  result->psm = clut->psm;
  result->info.components = clut->components;

  auto address = graph_vram_allocate(clut->width, clut->height, result->psm,
                                     GRAPH_ALIGN_BLOCK);
  TYRA_ASSERT(address > 0, "Texture clut buffer allocation error, no memory!");
  result->address = address;

  result->info.width = draw_log2(clut->width);
  result->info.height = draw_log2(clut->height);
  result->info.function = TEXTURE_FUNCTION_MODULATE;
  return result;
}

float RendererCoreTextureSender::getFreeVRamInMB() {
  return gs->getVRamFreeSpaceInMB() - allocatedVRamMemForTextures;
}

TextureBpp RendererCoreTextureSender::getBppByPsm(const u32& psm) {
  if (psm == GS_PSM_32) {
    return bpp32;
  } else if (psm == GS_PSM_24) {
    return bpp24;
  } else if (psm == GS_PSM_8) {
    return bpp8;
  } else if (psm == GS_PSM_4) {
    return bpp4;
  } else {
    TYRA_TRAP("Unknown bpp!");
    return bpp32;
  }
}

}  // namespace Tyra
