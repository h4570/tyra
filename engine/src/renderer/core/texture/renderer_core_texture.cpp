/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/core/texture/renderer_core_texture.hpp"

namespace Tyra {

RendererCoreTexture::RendererCoreTexture() {}

RendererCoreTexture::~RendererCoreTexture() {}

void RendererCoreTexture::init(RendererCoreGS* t_gs, Path3* t_path3) {
  gs = t_gs;
  sender.init(t_path3, t_gs);
  repository.init(&currentAllocations);
  path3 = t_path3;
  initClut();
}

void RendererCoreTexture::updateClutBuffer(texbuffer_t* clutBuffer) {
  if (clutBuffer == nullptr || clutBuffer->width == 0) {
    clut.psm = 0;
    clut.load_method = CLUT_NO_LOAD;
    clut.address = 0;
  } else {
    clut.psm = clutBuffer->psm;
    clut.load_method = CLUT_LOAD;
    clut.address = clutBuffer->address;
  }
}

RendererCoreTextureBuffers RendererCoreTexture::useTexture(
    const Texture* t_tex) {
  TYRA_ASSERT(t_tex != nullptr, "Provided nullptr texture!");

  auto allocated = getAllocatedBuffersByTextureId(t_tex->id);
  if (allocated.id != 0) return allocated;

  if (gs->vram.getSizeInMB(*t_tex) >= gs->vram.getFreeSpaceInMB()) {
    for (int i = currentAllocations.size() - 1; i >= 0; i--) {
      sender.deallocate(currentAllocations[i]);
    }
    currentAllocations.clear();
  }

  auto newTexBuffer = sender.allocate(t_tex);
  path3->sendTexture(t_tex, newTexBuffer);
  registerAllocation(newTexBuffer);

  return newTexBuffer;
}

RendererCoreTextureBuffers RendererCoreTexture::updateTextureInfo(
    const Texture* t_tex) {
  TYRA_ASSERT(t_tex != nullptr, "Provided nullptr texture!");

  auto allocated = getAllocatedBuffersByTextureId(t_tex->id);
  TYRA_ASSERT(allocated.id != 0, "Can't update an unallocated texture!");

  path3->sendTexture(t_tex, allocated);
  return allocated;
}

RendererCoreTextureBuffers RendererCoreTexture::getAllocatedBuffersByTextureId(
    const u32& t_id) {
  for (u32 i = 0; i < currentAllocations.size(); i++)
    if (currentAllocations[i].id == t_id) return currentAllocations[i];
  return {0, nullptr, nullptr};
}

void RendererCoreTexture::registerAllocation(
    const RendererCoreTextureBuffers& t_buffers) {
  currentAllocations.push_back(t_buffers);
}

void RendererCoreTexture::unregisterAllocation(const u32& textureId) {
  u32 foundIndex;

  for (u32 i = 0; i < currentAllocations.size(); i++) {
    if (currentAllocations[i].id == textureId) {
      foundIndex = i;
      break;
    }
  }

  currentAllocations.erase(currentAllocations.begin() + foundIndex);
}

void RendererCoreTexture::initClut() {
  clut.storage_mode = CLUT_STORAGE_MODE1;
  clut.start = 0;
  clut.psm = 0;
  clut.load_method = CLUT_NO_LOAD;
  clut.address = 0;
  TYRA_LOG("Clut set!");
}

}  // namespace Tyra
