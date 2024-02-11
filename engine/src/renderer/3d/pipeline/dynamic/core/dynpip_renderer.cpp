/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/3d/pipeline/dynamic/core/dynpip_renderer.hpp"
#include "renderer/3d/pipeline/dynamic/core/programs/dynpip_vu1_shared_defines.h"
#include <dma.h>
#include <utility>
#include "packet2/packet2_tyra_utils.hpp"

namespace Tyra {

DynPipRenderer::DynPipRenderer() {
  context = 0;
  bufferSize = 0;
  lastProgramName = DynPipProgramName::DynPipUndefinedProgram;

  programsPacket = nullptr;
}

DynPipRenderer::~DynPipRenderer() {
  if (programsPacket) packet2_free(programsPacket);
}

void DynPipRenderer::allocateOnUse(const u32& t_packetSize) {
  staticDataPacket = packet2_create(3, P2_TYPE_NORMAL, P2_MODE_CHAIN, true);
  objectDataPacket = packet2_create(20, P2_TYPE_NORMAL, P2_MODE_CHAIN, true);

  packetSize = t_packetSize;

  for (u16 i = 0; i < 2; i++)
    packets[i] =
        packet2_create(t_packetSize, P2_TYPE_NORMAL, P2_MODE_CHAIN, true);

  sendStaticData();
}

void DynPipRenderer::deallocateOnUse() {
  packet2_free(staticDataPacket);
  packet2_free(objectDataPacket);

  for (u16 i = 0; i < 2; i++) packet2_free(packets[i]);
}

void DynPipRenderer::init(RendererCore* t_core,
                          DynPipProgramsRepository* t_programRepo,
                          prim_t* t_prim, lod_t* t_lod) {
  path1 = t_core->getPath1();
  prim = t_prim;
  lod = t_lod;
  rendererCore = t_core;
  programsRepo = t_programRepo;

  dma_channel_initialize(DMA_CHANNEL_VIF1, nullptr, 0);

  setProgramsCache();

  reinitVU1();

  TYRA_LOG("DynPipRenderer initialized");
}

void DynPipRenderer::reinitVU1() {
  uploadPrograms();
  setDoubleBuffer();
}

void DynPipRenderer::setProgramsCache() {
  VU1Program** programs = new VU1Program*[4];
  programs[0] = programsRepo->getProgram(DynPipProgramName::DynPipColor);
  programs[1] = programsRepo->getProgram(DynPipProgramName::DynPipDirLights);
  programs[2] = programsRepo->getProgram(DynPipProgramName::DynPipTextureColor);
  programs[3] =
      programsRepo->getProgram(DynPipProgramName::DynPipTextureDirLights);
  programsPacket = path1->createProgramsCache(programs, 4, 0);
  delete[] programs;
}

void DynPipRenderer::sendStaticData() const {
  packet2_reset(staticDataPacket, false);
  packet2_utils_vu_open_unpack(staticDataPacket, VU1_SET_GIFTAG_ADDR, false);
  { packet2_utils_gif_add_set(staticDataPacket, 1); }
  packet2_utils_vu_close_unpack(staticDataPacket);

  packet2_utils_vu_add_end_tag(staticDataPacket);
  dma_channel_wait(DMA_CHANNEL_VIF1, 0);
  dma_channel_send_packet2(staticDataPacket, DMA_CHANNEL_VIF1, true);
}

void DynPipRenderer::sendObjectData(
    DynPipBag* bag, M4x4* mvp, RendererCoreTextureBuffers* texBuffers) const {
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
                    singleColorEnabled);  // Single color enabled.
    packet2_add_float(objectDataPacket,
                      bag->interpolation);  // Interpolation value
    packet2_add_u32(objectDataPacket, 0);   // not used, padding
    packet2_add_u32(objectDataPacket, 0);   // not used, padding

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

void DynPipRenderer::render(DynPipBag** bags, const u32& count) {
  if (count <= 0) return;

  addBufferDataToPacket(bags, count);
  sendPacket();
}

void DynPipRenderer::addBufferDataToPacket(DynPipBag** bags, const u32& count) {
  currentPacket = packets[context];
  packet2_reset(currentPacket, false);

  for (u32 i = 0; i < count; i++) {
    if (bags[i]->count <= 0) continue;

    auto* program = programsRepo->getProgramByBag(bags[i]);

    program->addBufferDataToPacket(currentPacket, bags[i], prim);

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

void DynPipRenderer::sendPacket() {
  dma_channel_wait(DMA_CHANNEL_VIF1, 0);
  dma_channel_wait(DMA_CHANNEL_GIF, 0);  // Wait for texture. Issue #182.

  // dma_wait_fast(); // This have no impact on performance
  dma_channel_send_packet2(currentPacket, DMA_CHANNEL_VIF1, true);

  TYRA_ASSERT(packet2_get_qw_count(currentPacket) <= packetSize,
              "Packet is too big. Internal error.");

  // Switch packet, so we can proceed during DMA transfer
  context = !context;
}

void DynPipRenderer::clearLastProgramName() {
  lastProgramName = DynPipUndefinedProgram;
}

void DynPipRenderer::uploadPrograms() {
  dma_channel_wait(DMA_CHANNEL_VIF1, 0);
  dma_channel_send_packet2(programsPacket, DMA_CHANNEL_VIF1, true);
  dma_channel_wait(DMA_CHANNEL_VIF1, 0);
}

void DynPipRenderer::setDoubleBuffer() {
  u16 startingAddr = VU1_DYNPIP_LAST_ITEM_ADDR + 1;
  const u16 bufferMaxSize = 1000;
  bufferSize = (bufferMaxSize - startingAddr) / 2;

  path1->setDoubleBuffer(startingAddr, bufferSize);

  bufferSize -= 1;  // Because we don't want to upload anything from first
                    // buffer, to first addr of second buffer
}

}  // namespace Tyra
