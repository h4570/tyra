/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/3d/pipeline/static/core/stapip_qbuffer_renderer.hpp"
#include "renderer/3d/pipeline/static/core/programs/stapip_vu1_shared_defines.h"

namespace Tyra {

/**
 * VU1 = 1000 vert
 *
 * Quadbuffering:
 * 2 main buffers = 1000 / 2 = 500 vert
 * 2 kick buffers =  500 / 2 = 250 vert
 *
 * Vert data:
 * Pos + Normal + ST + Color = 4
 * = 4 * 48 = 192
 *
 * Other data:
 * mvp matrix, light matrix, tags = 14
 * 20 light vectors, light intesities = 25
 * = 14 + 25 = 39
 *
 * All data:
 * = 192 + 39 = 231
 *
 */

StapipQBufferRenderer::StapipQBufferRenderer() {
  context = 0;
  lastProgramName = StdipUndefinedProgram;
  staticDataPacket = packet2_create(3, P2_TYPE_NORMAL, P2_MODE_CHAIN, true);
  objectDataPacket = packet2_create(16, P2_TYPE_NORMAL, P2_MODE_CHAIN, true);
  programsPacket = nullptr;
}
StapipQBufferRenderer::~StapipQBufferRenderer() {
  packet2_free(packets[0]);
  packet2_free(packets[1]);
  packet2_free(staticDataPacket);
  packet2_free(objectDataPacket);

  if (programsPacket) packet2_free(programsPacket);
}

void StapipQBufferRenderer::init(RendererCore* t_core) {
  path1 = t_core->getPath1();
  clipper.init(t_core->getSettings());
  rendererCore = t_core;

  dma_channel_initialize(DMA_CHANNEL_VIF1, NULL, 0);
  dma_channel_fast_waits(DMA_CHANNEL_VIF1);

  const u32 VU1_PACKET_SIZE = 16;

  packets[0] =
      packet2_create(VU1_PACKET_SIZE, P2_TYPE_NORMAL, P2_MODE_CHAIN, true);

  packets[1] =
      packet2_create(VU1_PACKET_SIZE, P2_TYPE_NORMAL, P2_MODE_CHAIN, true);

  setProgramsCache();

  reinitVU1();

  TYRA_LOG("Renderer3DQBufferRenderer initialized");
}

void StapipQBufferRenderer::reinitVU1() {
  sendStaticData();
  uploadPrograms();
  setDoubleBuffer();
}

StapipQBuffer* StapipQBufferRenderer::getBuffer() { return &buffers[context]; }

void StapipQBufferRenderer::sendObjectData(
    StapipBag* bag, M4x4* mvp, RendererCoreTextureBuffers* texBuffers) const {
  packet2_reset(objectDataPacket, false);
  packet2_utils_vu_add_unpack_data(objectDataPacket, VU1_MVP_MATRIX_ADDR,
                                   mvp->data, 4, false);

  if (bag->lighting) {
    packet2_utils_vu_add_unpack_data(objectDataPacket, VU1_LIGHTS_MATRIX_ADDR,
                                     bag->lighting->lightMatrix, 3, false);

    packet2_utils_vu_add_unpack_data(objectDataPacket, VU1_LIGHTS_DIRS_ADDR,
                                     bag->lighting->getLightDirections(), 3,
                                     false);

    packet2_utils_vu_add_unpack_data(objectDataPacket, VU1_LIGHTS_COLORS_ADDR,
                                     bag->lighting->getLightColors(), 4, false);
  }

  u8 singleColorEnabled = bag->color->single != nullptr;

  if (singleColorEnabled)  // Color is placed in 4th slot of
                           // VU1_LIGHTS_MATRIX_ADDR
    packet2_utils_vu_add_unpack_data(objectDataPacket, VU1_SINGLE_COLOR_ADDR,
                                     bag->color->single->rgba, 1, false);

  packet2_utils_vu_open_unpack(objectDataPacket, VU1_OPTIONS_ADDR, false);
  {
    packet2_add_u32(objectDataPacket,
                    singleColorEnabled);   // Single color enabled.
    packet2_add_u32(objectDataPacket, 0);  // not used, padding
    packet2_add_u32(objectDataPacket, 0);  // not used, padding
    packet2_add_u32(objectDataPacket, 0);  // not used, padding

    packet2_utils_gs_add_lod(objectDataPacket, &rendererCore->gs.lod);

    if (texBuffers != nullptr) {
      packet2_utils_gs_add_texbuff_clut(objectDataPacket, texBuffers->core,
                                        &rendererCore->texture.clut);

      rendererCore->texture.updateClutBuffer(texBuffers->clut);
    }
  }
  packet2_utils_vu_close_unpack(objectDataPacket);

  packet2_utils_vu_add_end_tag(objectDataPacket);
  dma_channel_wait(DMA_CHANNEL_VIF1, 0);
  dma_channel_send_packet2(objectDataPacket, DMA_CHANNEL_VIF1, true);
}

void StapipQBufferRenderer::setInfo(StapipInfoBag* bag) {
  rendererCore->gs.prim.antialiasing = bag->antiAliasingEnabled;
  rendererCore->gs.prim.blending = bag->blendingEnabled;
  rendererCore->gs.prim.shading = bag->shadingType;
}

void StapipQBufferRenderer::sendStaticData() const {
  packet2_reset(staticDataPacket, false);
  packet2_utils_vu_open_unpack(staticDataPacket, VU1_SET_GIFTAG_ADDR, false);
  { packet2_utils_gif_add_set(staticDataPacket, 1); }
  packet2_utils_vu_close_unpack(staticDataPacket);

  packet2_utils_vu_add_end_tag(staticDataPacket);
  dma_channel_wait(DMA_CHANNEL_VIF1, 0);
  dma_channel_send_packet2(staticDataPacket, DMA_CHANNEL_VIF1, true);
}

void StapipQBufferRenderer::setProgramsCache() {
  VU1Program** programs = new VU1Program*[8];
  programs[0] = repository.getProgram(StapipCullColor);
  programs[1] = repository.getProgram(StapipAsIsColor);
  programs[2] = repository.getProgram(StapipCullDirLights);
  programs[3] = repository.getProgram(StapipAsIsDirLights);
  programs[4] = repository.getProgram(StapipCullTextureDirLights);
  programs[5] = repository.getProgram(StapipAsIsTextureDirLights);
  programs[6] = repository.getProgram(StapipCullTextureColor);
  programs[7] = repository.getProgram(StapipAsIsTextureColor);
  programsPacket = path1->createProgramsCache(programs, 8, 0);
  delete[] programs;
}

void StapipQBufferRenderer::uploadPrograms() {
  dma_channel_wait(DMA_CHANNEL_VIF1, 0);
  dma_channel_send_packet2(programsPacket, DMA_CHANNEL_VIF1, true);
  dma_channel_wait(DMA_CHANNEL_VIF1, 0);
}

void StapipQBufferRenderer::setDoubleBuffer() {
  u16 startingAddr = VU1_LAST_ITEM_ADDR + 1;
  const u16 bufferMaxSize = 1000;
  bufferSize = (bufferMaxSize - startingAddr) / 2;

  path1->setDoubleBuffer(startingAddr, bufferSize);

  bufferSize -= 1;  // Because we don't want to upload anything from first
                    // buffer, to first addr of second buffer
}

void StapipQBufferRenderer::cull(StapipQBuffer* buffer) {
  if (buffer->size == 0) {
    return;
  }

  auto program = getCullProgramByBag(buffer->bag);
  addBufferDataToPacket(program, buffer);
  sendPacket();
}

// void StapipQBufferRenderer::sendFinishTag() {
// StapipQBufferRenderer way proposition
// auto program = path1->getProgramByName(Draw_Finish);
// addBufferDataToPacket(program, nullptr);
// sendPacket();

// packet2_t* packet2 = packet2_create(8, P2_TYPE_NORMAL, P2_MODE_CHAIN,
// true); auto program =
//     static_cast<VU1DrawFinish*>(getProgramByName(Draw_Finish));

// program->addTag(packet2, prim);
// packet2_utils_vu_add_start_program(packet2,
// program->getDestinationAddress()); packet2_utils_vu_add_end_tag(packet2);
// dma_channel_wait(DMA_CHANNEL_VIF1, 0);
// dma_channel_send_packet2(packet2, DMA_CHANNEL_VIF1, true);
// packet2_free(packet2);
// }

void StapipQBufferRenderer::clip(StapipQBuffer* buffer) {
  if (buffer->size == 0) {
    return;
  }

  auto program = getAsIsProgramByBag(buffer->bag);
  clipper.clip(buffer);

  if (buffer->any()) {
    if (buffer) addBufferDataToPacket(program, buffer);
    sendPacket();
  }
}

void StapipQBufferRenderer::clearLastProgramName() {
  lastProgramName = StdipUndefinedProgram;
}

void StapipQBufferRenderer::addBufferDataToPacket(StapipVU1Program* program,
                                                  StapipQBuffer* buffer) {
  currentPacket = packets[context];
  packet2_reset(currentPacket, false);

  program->addBufferDataToPacket(currentPacket, buffer, &rendererCore->gs.prim);

  if (lastProgramName != program->getName()) {
    packet2_utils_vu_add_start_program(currentPacket,
                                       program->getDestinationAddress());
    lastProgramName = program->getName();
  } else {
    packet2_utils_vu_add_continue_program(currentPacket);
  }
  packet2_utils_vu_add_end_tag(currentPacket);
}

void StapipQBufferRenderer::sendPacket() {
  dma_channel_wait(DMA_CHANNEL_VIF1, 0);
  dma_channel_send_packet2(currentPacket, DMA_CHANNEL_VIF1, true);

  // Switch packet, so we can proceed during DMA transfer
  context = !context;
}

void StapipQBufferRenderer::setMaxVertCount(const u32& count) {
  buffers[0].setMaxVertCount(count);
  buffers[1].setMaxVertCount(count);
  clipper.setMaxVertCount(count);
}

StapipVU1Program* StapipQBufferRenderer::getAsIsProgramByBag(
    const StapipBag* bag) {
  auto programType = getDrawProgramTypeByBag(bag);

  if (programType == StapipVU1TextureDirLights)
    return getProgramByName(StapipAsIsTextureDirLights);
  else if (programType == StapipVU1DirLights)
    return getProgramByName(StapipAsIsDirLights);
  else if (programType == StapipVU1TextureColor)
    return getProgramByName(StapipAsIsTextureColor);
  else
    return getProgramByName(StapipAsIsColor);
}

StapipVU1Program* StapipQBufferRenderer::getCullProgramByBag(
    const StapipBag* bag) {
  auto programType = getDrawProgramTypeByBag(bag);
  return getCullProgramByType(programType);
}

StapipVU1Program* StapipQBufferRenderer::getProgramByName(
    const StapipProgramName& name) {
  return repository.getProgram(name);
}

StapipVU1Program* StapipQBufferRenderer::getCullProgramByParams(
    const bool& isLightingEnabled, const bool& isTextureEnabled) {
  auto type = getDrawProgramTypeByParams(isLightingEnabled, isTextureEnabled);
  return getCullProgramByType(type);
}

StapipVU1Program* StapipQBufferRenderer::getCullProgramByType(
    const StapipProgramType& programType) {
  if (programType == StapipVU1TextureDirLights)
    return getProgramByName(StapipCullTextureDirLights);
  else if (programType == StapipVU1DirLights)
    return getProgramByName(StapipCullDirLights);
  else if (programType == StapipVU1TextureColor)
    return getProgramByName(StapipCullTextureColor);
  else
    return getProgramByName(StapipCullColor);
}

StapipProgramType StapipQBufferRenderer::getDrawProgramTypeByBag(
    const StapipBag* bag) const {
  auto isLightingEnabled = bag->lighting != nullptr;
  auto isTextureEnabled = bag->texture != nullptr;
  return getDrawProgramTypeByParams(isLightingEnabled, isTextureEnabled);
}

StapipProgramType StapipQBufferRenderer::getDrawProgramTypeByParams(
    const bool& isLightingEnabled, const bool& isTextureEnabled) const {
  if (isLightingEnabled && isTextureEnabled)
    return StapipVU1TextureDirLights;
  else if (isLightingEnabled)
    return StapipVU1DirLights;
  else if (isTextureEnabled)
    return StapipVU1TextureColor;
  else
    return StapipVU1Color;
}

}  // namespace Tyra
