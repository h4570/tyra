/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

#include <dma.h>
#include <packet2_utils.h>
#include <vector>
#include "debug/debug.hpp"
#include "math/m4x4.hpp"
#include "renderer/renderer_settings.hpp"
#include "renderer/models/color.hpp"
#include "./stapip_qbuffer.hpp"
#include "./stapip_program_type.hpp"
#include "./stapip_program_name.hpp"
#include "./stapip_programs_repository.hpp"
#include "./stapip_clipper.hpp"
#include "renderer/core/paths/path1/path1.hpp"
#include "renderer/core/renderer_core.hpp"
#include "renderer/core/texture/renderer_core_texture_buffers.hpp"

namespace Tyra {

class StaPipQBufferRenderer {
 public:
  StaPipQBufferRenderer();
  ~StaPipQBufferRenderer();

  void init(RendererCore* t_core);

  void reinitVU1();

  void setClipperMVP(M4x4* mvp) { clipper.setMVP(mvp); }

  StaPipQBuffer* getBuffer();

  void sendObjectData(StaPipBag* bag, M4x4* mvp,
                      RendererCoreTextureBuffers* texBuffers) const;

  void setMaxVertCount(const u32& count);

  void setInfo(PipelineInfoBag* bag);

  /** Fast render with culling */
  void cull(StaPipQBuffer* buffer);

  /** Slower render with clipping */
  void clip(StaPipQBuffer* buffer);

  void flushBuffers();

  void clearLastProgramName();

  StaPipVU1Program* getCullProgramByBag(const StaPipBag* bag);

  StaPipVU1Program* getCullProgramByParams(const bool& isLightingEnabled,
                                           const bool& isTextureEnabled);

  const u16& getBufferSize() { return bufferSize; }

  void allocateOnUse();
  void deallocateOnUse();

 private:
  bool is1stDBufferFlushTime();
  bool is2ndDBufferFlushTime();

  void sendStaticData() const;
  void setProgramsCache();
  void uploadPrograms();
  void setDoubleBuffer();
  u16 getQBufferIndex(StaPipQBuffer* buffer);
  u16 qbuffersPacketSize;

  static const u16 buffersCount;

  StaPipVU1Program* getProgramByName(const StaPipProgramName& name);
  void addBufferDataToPacket(StaPipQBuffer** buffers, const u32& count);
  void sendPacket();
  StaPipVU1Program* getAsIsProgramByBag(const StaPipBag* bag);
  StaPipVU1Program* getCullProgramByType(const StaPipProgramType& programType);
  StaPipProgramType getDrawProgramTypeByBag(const StaPipBag* bag) const;
  StaPipProgramType getDrawProgramTypeByParams(
      const bool& isLightingEnabled, const bool& isTextureEnabled) const;
  packet2_t* programsPacket;

  packet2_t** packets;
  StaPipVU1Program** dBufferPrograms;
  StaPipQBuffer** buffers;
  packet2_t* currentPacket;
  packet2_t* staticDataPacket;
  packet2_t* objectDataPacket;

  RendererCore* rendererCore;

  StaPipProgramName lastProgramName;
  Path1* path1;
  StaPipClipper clipper;
  StaPipProgramsRepository repository;

  u16 bufferSize, nextBufferIndex, currentBufferIndex;
  u8 context;
};

}  // namespace Tyra
