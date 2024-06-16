/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/core/texture/renderer_core_texture_sender.hpp"
#include <gs_psm.h>

namespace Tyra {

RendererCoreTextureSender::RendererCoreTextureSender() {}
RendererCoreTextureSender::~RendererCoreTextureSender() {}

void RendererCoreTextureSender::init(Path3* t_path3, RendererCoreGS* t_gs) {
  gs = t_gs;
  path3 = t_path3;
  TYRA_LOG("Renderer texture initialized!");
}

RendererCoreTextureBuffers RendererCoreTextureSender::allocate(
    const Texture* t_texture) {
  texbuffer_t* core = allocateTextureCore(t_texture);
  texbuffer_t* clut = nullptr;

  auto texClut = t_texture->clut;
  if (texClut != nullptr && texClut->width > 0) {
    clut = allocateTextureClut(t_texture);
  }
  return {t_texture->id, core, clut};
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
    gs->vram.free(texBuffers.clut->address);
    delete texBuffers.clut;
  }

  gs->vram.free(texBuffers.core->address);

  delete texBuffers.core;
}

texbuffer_t* RendererCoreTextureSender::allocateTextureCore(
    const Texture* t_texture) {
  auto* result = new texbuffer_t;
  const auto* core = t_texture->core;

  int coreWidth = core->width <= 64 ? 64 : core->width;

  result->width = coreWidth;
  result->psm = core->psm;
  result->info.components = core->components;

  auto address = gs->vram.allocate(*core);
  TYRA_ASSERT(address > 0, "Texture buffer allocation error, no memory!");
  result->address = address;

  result->info.width = draw_log2(t_texture->getWidth());
  result->info.height = draw_log2(t_texture->getHeight());
  result->info.function = TEXTURE_FUNCTION_MODULATE;
  return result;
}

texbuffer_t* RendererCoreTextureSender::allocateTextureClut(
    const Texture* t_texture) {
  auto* result = new texbuffer_t;
  const auto* clut = t_texture->clut;

  int clutWidth = clut->width <= 64 ? 64 : clut->width;

  result->width = clutWidth;
  result->psm = clut->psm;
  result->info.components = clut->components;

  auto address = gs->vram.allocate(*clut);
  TYRA_ASSERT(address > 0, "Texture clut buffer allocation error, no memory!");
  result->address = address;

  result->info.width = draw_log2(clut->width);
  result->info.height = draw_log2(clut->height);
  result->info.function = TEXTURE_FUNCTION_MODULATE;
  return result;
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
