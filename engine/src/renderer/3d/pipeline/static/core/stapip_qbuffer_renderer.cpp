/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/3d/pipeline/static/core/stapip_qbuffer_renderer.hpp"
#include "renderer/3d/pipeline/static/core/programs/stapip_vu1_shared_defines.h"
#include "packet2/packet2_tyra_utils.hpp"

// #define TYRA_QBUFF_RENDERER_VERBOSE_LOG 1

#ifdef TYRA_QBUFF_RENDERER_VERBOSE_LOG
#define Verbose(...) TyraDebug::writeLines("VRB: ", ##__VA_ARGS__, "\n")
#else
#define Verbose(...) ((void)0)
#endif

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

const u16 StaPipQBufferRenderer::buffersCount = 32;

StaPipQBufferRenderer::StaPipQBufferRenderer() {
  currentBufferIndex = 0;
  nextBufferIndex = 0;
  context = 0;
  lastProgramName = StaPipUndefinedProgram;

  qbuffersPacketSize = 4 * buffersCount;
  programsPacket = nullptr;
}

void StaPipQBufferRenderer::allocateOnUse() {
  staticDataPacket = packet2_create(3, P2_TYPE_NORMAL, P2_MODE_CHAIN, true);
  objectDataPacket = packet2_create(20, P2_TYPE_NORMAL, P2_MODE_CHAIN, true);

  packets = new packet2_t*[2];
  for (u16 i = 0; i < 2; i++)
    packets[i] =
        packet2_create(qbuffersPacketSize, P2_TYPE_NORMAL, P2_MODE_CHAIN, true);

  buffers = new StaPipQBuffer*[buffersCount];
  for (u16 i = 0; i < buffersCount; i++) {
    buffers[i] = new StaPipQBuffer();
  }

  dBufferPrograms = new StaPipVU1Program*[buffersCount];

  sendStaticData();
}

void StaPipQBufferRenderer::deallocateOnUse() {
  packet2_free(staticDataPacket);
  packet2_free(objectDataPacket);

  for (u16 i = 0; i < 2; i++) packet2_free(packets[i]);
  delete[] packets;

  for (u16 i = 0; i < buffersCount; i++) delete buffers[i];
  delete[] buffers;

  delete[] dBufferPrograms;
}

StaPipQBufferRenderer::~StaPipQBufferRenderer() {
  if (programsPacket) packet2_free(programsPacket);
}

void StaPipQBufferRenderer::init(RendererCore* t_core, prim_t* t_prim,
                                 lod_t* t_lod) {
  path1 = t_core->getPath1();
  clipper.init(t_core->getSettings());
  rendererCore = t_core;
  prim = t_prim;
  lod = t_lod;

  dma_channel_initialize(DMA_CHANNEL_VIF1, nullptr, 0);

  setProgramsCache();

  reinitVU1();

  TYRA_LOG("StaPipQBufferRenderer initialized");
}

void StaPipQBufferRenderer::reinitVU1() {
  uploadPrograms();
  setDoubleBuffer();
}

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
    Packet2TyraUtils::addUnpackData(objectDataPacket, VU1_SINGLE_COLOR_ADDR,
                                    bag->color->single->rgba, 1, false);

  packet2_utils_vu_open_unpack(objectDataPacket, VU1_OPTIONS_ADDR, false);
  {
    packet2_add_u32(objectDataPacket,
                    singleColorEnabled);   // Single color enabled.
    packet2_add_u32(objectDataPacket, 0);  // not used, padding
    packet2_add_u32(objectDataPacket, 0);  // not used, padding
    packet2_add_u32(objectDataPacket, 0);  // not used, padding

    packet2_utils_gs_add_lod(objectDataPacket, lod);

    if (bag->info->zTestType == PipelineZTest_AllPass) {
      packet2_add_2x_s64(objectDataPacket,
                         GS_SET_TEST(0, 0, 0, 0, 0, 0, 0, ZTEST_METHOD_ALLPASS),
                         GS_REG_TEST);
    } else {
      packet2_add_2x_s64(
          objectDataPacket,
          GS_SET_TEST(DRAW_ENABLE, ATEST_METHOD_NOTEQUAL, 0x00,
                      ATEST_KEEP_FRAMEBUFFER, DRAW_DISABLE, DRAW_DISABLE,
                      DRAW_ENABLE, rendererCore->gs.zBuffer.method),
          GS_REG_TEST);
    }

    if (texBuffers != nullptr) {
      rendererCore->texture.updateClutBuffer(texBuffers->clut);

      packet2_utils_gs_add_texbuff_clut(objectDataPacket, texBuffers->core,
                                        &rendererCore->texture.clut);
    }
  }
  packet2_utils_vu_close_unpack(objectDataPacket);

  packet2_utils_vu_add_end_tag(objectDataPacket);
  dma_channel_wait(DMA_CHANNEL_VIF1, 0);
  dma_channel_send_packet2(objectDataPacket, DMA_CHANNEL_VIF1, true);
}

void StaPipQBufferRenderer::setInfo(PipelineInfoBag* bag) {
  prim->antialiasing = bag->antiAliasingEnabled;
  prim->blending = bag->blendingEnabled;
  prim->shading = bag->shadingType;

  if (bag->textureMappingType == TyraLinear) {
    lod->mag_filter = LOD_MAG_LINEAR;
    lod->min_filter = LOD_MIN_LINEAR;
  } else {
    lod->mag_filter = LOD_MAG_NEAREST;
    lod->min_filter = LOD_MIN_NEAREST;
  }
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
  u16 startingAddr = VU1_STAPIP_LAST_ITEM_ADDR + 1;
  const u16 bufferMaxSize = 1000;
  bufferSize = (bufferMaxSize - startingAddr) / 2;

  path1->setDoubleBuffer(startingAddr, bufferSize);

  bufferSize -= 1;  // Because we don't want to upload anything from first
                    // buffer, to first addr of second buffer
}

StaPipQBuffer* StaPipQBufferRenderer::getBuffer() {
  currentBufferIndex = nextBufferIndex++;
  Verbose("Proposing buffer: ", currentBufferIndex);
  auto* result = buffers[currentBufferIndex];
  if (nextBufferIndex >= buffersCount) {
    Verbose("Rollup - clearing buffer indices. currentBufferIndex: ",
            currentBufferIndex, " (before)nextBufferIndex: ", nextBufferIndex);
    nextBufferIndex = 0;
  }
  return result;
}

u16 StaPipQBufferRenderer::getQBufferIndex(StaPipQBuffer* buffer) {
  for (u16 i = 0; i < buffersCount; i++) {
    if (buffers[i] == buffer) return i;
  }
  TYRA_TRAP("Buffer not found!");
  return 0;
}

bool StaPipQBufferRenderer::is1stDBufferFlushTime() {
  return nextBufferIndex == buffersCount / 2;
}

bool StaPipQBufferRenderer::is2ndDBufferFlushTime() {
  return nextBufferIndex == 0;
}

void StaPipQBufferRenderer::flushBuffers() {
  auto is1stDBuffer = is1stDBufferFlushTime();
  auto is2ndDBuffer = is2ndDBufferFlushTime();

  if (!is1stDBuffer && !is2ndDBuffer) {
    auto offset = currentBufferIndex >= buffersCount / 2 ? buffersCount / 2 : 0;
    auto size = (currentBufferIndex + 1) - offset;
    Verbose("-- End flush. from: ", offset, " to: ", offset + size);
    addBuffersDataToPacket(offset, offset + size);
    sendPacket();
  }

  currentBufferIndex = 0;
  nextBufferIndex = 0;

  Verbose("End flush - zeroing buffer indices.");
}

void StaPipQBufferRenderer::cull(StaPipQBuffer* buffer) {
  if (buffer->size == 0) {
    return;
  }

  dBufferPrograms[getQBufferIndex(buffer)] = getCullProgramByBag(buffer->bag);

  Verbose("Add cull[", getQBufferIndex(buffer), "]: ", buffer->size);

  auto is1stDBuffer = is1stDBufferFlushTime();
  auto is2ndDBuffer = is2ndDBufferFlushTime();

  if (is1stDBuffer || is2ndDBuffer) {
    auto from = is1stDBuffer ? 0 : buffersCount / 2;
    auto to = from + buffersCount / 2;

    Verbose("-- Half flush at ", getQBufferIndex(buffer), ". from: ", from,
            " to: ", to);

    addBuffersDataToPacket(from, to);
    sendPacket();
  }
}

void StaPipQBufferRenderer::clip(StaPipQBuffer* buffer) {
  if (buffer->size == 0) {
    return;
  }

  dBufferPrograms[getQBufferIndex(buffer)] = getAsIsProgramByBag(buffer->bag);

  clipper.clip(buffer);

  Verbose("Add clip[", getQBufferIndex(buffer), "]: ", buffer->size);

  auto is1stDBuffer = is1stDBufferFlushTime();
  auto is2ndDBuffer = is2ndDBufferFlushTime();

  if (is1stDBuffer || is2ndDBuffer) {
    auto from = is1stDBuffer ? 0 : buffersCount / 2;
    auto to = from + buffersCount / 2;

    Verbose("-- Half flush at ", getQBufferIndex(buffer), ". from: ", from,
            " to: ", to);

    addBuffersDataToPacket(from, to);
    sendPacket();
  }
}

void StaPipQBufferRenderer::clearLastProgramName() {
  lastProgramName = StaPipUndefinedProgram;
}

void StaPipQBufferRenderer::addBuffersDataToPacket(const u32& from,
                                                   const u32& to) {
  auto* currentPacket = packets[context];
  packet2_reset(currentPacket, false);

  for (u32 i = from; i < to; i++) {
    if (!buffers[i]->any()) continue;

    auto* program = dBufferPrograms[i];

    program->addBufferDataToPacket(currentPacket, buffers[i], prim);

    Verbose("Send ", program->getStringName(), "[", i, "]: ", buffers[i]->size);

    if (lastProgramName != program->getName()) {
      packet2_utils_vu_add_start_program(currentPacket,
                                         program->getDestinationAddress());
      lastProgramName = program->getName();
    } else {
      packet2_utils_vu_add_continue_program(currentPacket);
    }
  }

  packet2_utils_vu_add_end_tag(currentPacket);
}

void StaPipQBufferRenderer::sendPacket() {
  auto* currentPacket = packets[context];

  TYRA_ASSERT(packet2_get_qw_count(currentPacket) <= qbuffersPacketSize,
              "Packet is too big. Internal error");

  dma_channel_wait(DMA_CHANNEL_VIF1, 0);
  dma_channel_wait(DMA_CHANNEL_GIF, 0);  // Wait for texture. Issue #182.

  // dma_wait_fast(); // This have no impact on performance

  dma_channel_send_packet2(currentPacket, DMA_CHANNEL_VIF1, true);

  // Switch packet, so we can proceed during DMA transfer
  context = !context;
}

void StaPipQBufferRenderer::setMaxVertCount(const u32& count) {
  for (u32 i = 0; i < buffersCount; i++) {
    buffers[i]->setMaxVertCount(count);
  }
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
