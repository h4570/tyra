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

StaPipQBufferRenderer::StaPipQBufferRenderer() {
  context = 0;
  lastProgramName = StaPipUndefinedProgram;
  staticDataPacket = packet2_create(3, P2_TYPE_NORMAL, P2_MODE_CHAIN, true);
  objectDataPacket = packet2_create(16, P2_TYPE_NORMAL, P2_MODE_CHAIN, true);
  programsPacket = nullptr;
}
StaPipQBufferRenderer::~StaPipQBufferRenderer() {
  packet2_free(packets[0]);
  packet2_free(packets[1]);
  packet2_free(staticDataPacket);
  packet2_free(objectDataPacket);

  if (programsPacket) packet2_free(programsPacket);
}

void StaPipQBufferRenderer::init(RendererCore* t_core) {
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

void StaPipQBufferRenderer::reinitVU1() {
  sendStaticData();
  uploadPrograms();
  setDoubleBuffer();
}

StaPipQBuffer* StaPipQBufferRenderer::getBuffer() { return &buffers[context]; }

void StaPipQBufferRenderer::sendObjectData(
    StaPipBag* bag, M4x4* mvp, RendererCoreTextureBuffers* texBuffers) const {
  packet2_reset(objectDataPacket, false);
  packet2_utils_vu_add_unpack_data(objectDataPacket, VU1_MVP_MATRIX_ADDR,
                                   mvp->data, 4, false);

  if (bag->lighting) {
    packet2_utils_vu_add_unpack_data(objectDataPacket, VU1_LIGHTS_MATRIX_ADDR,
                                     bag->lighting->lightMatrix, 3, false);

    packet2_utils_vu_add_unpack_data(
        objectDataPacket, VU1_LIGHTS_DIRS_ADDR,
        bag->lighting->dirLights->getLightDirections(), 3, false);

    packet2_utils_vu_add_unpack_data(objectDataPacket, VU1_LIGHTS_COLORS_ADDR,
                                     bag->lighting->dirLights->getLightColors(),
                                     4, false);
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

void StaPipQBufferRenderer::setInfo(PipelineInfoBag* bag) {
  rendererCore->gs.prim.antialiasing = bag->antiAliasingEnabled;
  rendererCore->gs.prim.blending = bag->blendingEnabled;
  rendererCore->gs.prim.shading = bag->shadingType;
}

void StaPipQBufferRenderer::sendStaticData() const {
  packet2_reset(staticDataPacket, false);
  packet2_utils_vu_open_unpack(staticDataPacket, VU1_SET_GIFTAG_ADDR, false);
  { packet2_utils_gif_add_set(staticDataPacket, 1); }
  packet2_utils_vu_close_unpack(staticDataPacket);

  packet2_utils_vu_add_end_tag(staticDataPacket);
  dma_channel_wait(DMA_CHANNEL_VIF1, 0);
  dma_channel_send_packet2(staticDataPacket, DMA_CHANNEL_VIF1, true);
}

void StaPipQBufferRenderer::setProgramsCache() {
  VU1Program** programs = new VU1Program*[8];
  programs[0] = repository.getProgram(StaPipCullColor);
  programs[1] = repository.getProgram(StaPipAsIsColor);
  programs[2] = repository.getProgram(StaPipCullDirLights);
  programs[3] = repository.getProgram(StaPipAsIsDirLights);
  programs[4] = repository.getProgram(StaPipCullTextureDirLights);
  programs[5] = repository.getProgram(StaPipAsIsTextureDirLights);
  programs[6] = repository.getProgram(StaPipCullTextureColor);
  programs[7] = repository.getProgram(StaPipAsIsTextureColor);
  programsPacket = path1->createProgramsCache(programs, 8, 0);
  delete[] programs;
}

void StaPipQBufferRenderer::uploadPrograms() {
  dma_channel_wait(DMA_CHANNEL_VIF1, 0);
  dma_channel_send_packet2(programsPacket, DMA_CHANNEL_VIF1, true);
  dma_channel_wait(DMA_CHANNEL_VIF1, 0);
}

void StaPipQBufferRenderer::setDoubleBuffer() {
  u16 startingAddr = VU1_LAST_ITEM_ADDR + 1;
  const u16 bufferMaxSize = 1000;
  bufferSize = (bufferMaxSize - startingAddr) / 2;

  path1->setDoubleBuffer(startingAddr, bufferSize);

  bufferSize -= 1;  // Because we don't want to upload anything from first
                    // buffer, to first addr of second buffer
}

void StaPipQBufferRenderer::cull(StaPipQBuffer* buffer) {
  if (buffer->size == 0) {
    return;
  }

  auto program = getCullProgramByBag(buffer->bag);
  addBufferDataToPacket(program, buffer);
  sendPacket();
}

void StaPipQBufferRenderer::clip(StaPipQBuffer* buffer) {
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

void StaPipQBufferRenderer::clearLastProgramName() {
  lastProgramName = StaPipUndefinedProgram;
}

void StaPipQBufferRenderer::addBufferDataToPacket(StaPipVU1Program* program,
                                                  StaPipQBuffer* buffer) {
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

void StaPipQBufferRenderer::sendPacket() {
  dma_channel_wait(DMA_CHANNEL_VIF1, 0);
  dma_channel_send_packet2(currentPacket, DMA_CHANNEL_VIF1, true);

  // Switch packet, so we can proceed during DMA transfer
  context = !context;
}

void StaPipQBufferRenderer::setMaxVertCount(const u32& count) {
  buffers[0].setMaxVertCount(count);
  buffers[1].setMaxVertCount(count);
  clipper.setMaxVertCount(count);
}

StaPipVU1Program* StaPipQBufferRenderer::getAsIsProgramByBag(
    const StaPipBag* bag) {
  auto programType = getDrawProgramTypeByBag(bag);

  if (programType == StaPipVU1TextureDirLights)
    return getProgramByName(StaPipAsIsTextureDirLights);
  else if (programType == StaPipVU1DirLights)
    return getProgramByName(StaPipAsIsDirLights);
  else if (programType == StaPipVU1TextureColor)
    return getProgramByName(StaPipAsIsTextureColor);
  else
    return getProgramByName(StaPipAsIsColor);
}

StaPipVU1Program* StaPipQBufferRenderer::getCullProgramByBag(
    const StaPipBag* bag) {
  auto programType = getDrawProgramTypeByBag(bag);
  return getCullProgramByType(programType);
}

StaPipVU1Program* StaPipQBufferRenderer::getProgramByName(
    const StaPipProgramName& name) {
  return repository.getProgram(name);
}

StaPipVU1Program* StaPipQBufferRenderer::getCullProgramByParams(
    const bool& isLightingEnabled, const bool& isTextureEnabled) {
  auto type = getDrawProgramTypeByParams(isLightingEnabled, isTextureEnabled);
  return getCullProgramByType(type);
}

StaPipVU1Program* StaPipQBufferRenderer::getCullProgramByType(
    const StaPipProgramType& programType) {
  if (programType == StaPipVU1TextureDirLights)
    return getProgramByName(StaPipCullTextureDirLights);
  else if (programType == StaPipVU1DirLights)
    return getProgramByName(StaPipCullDirLights);
  else if (programType == StaPipVU1TextureColor)
    return getProgramByName(StaPipCullTextureColor);
  else
    return getProgramByName(StaPipCullColor);
}

StaPipProgramType StaPipQBufferRenderer::getDrawProgramTypeByBag(
    const StaPipBag* bag) const {
  auto isLightingEnabled = bag->lighting != nullptr;
  auto isTextureEnabled = bag->texture != nullptr;
  return getDrawProgramTypeByParams(isLightingEnabled, isTextureEnabled);
}

StaPipProgramType StaPipQBufferRenderer::getDrawProgramTypeByParams(
    const bool& isLightingEnabled, const bool& isTextureEnabled) const {
  if (isLightingEnabled && isTextureEnabled)
    return StaPipVU1TextureDirLights;
  else if (isLightingEnabled)
    return StaPipVU1DirLights;
  else if (isTextureEnabled)
    return StaPipVU1TextureColor;
  else
    return StaPipVU1Color;
}

}  // namespace Tyra
