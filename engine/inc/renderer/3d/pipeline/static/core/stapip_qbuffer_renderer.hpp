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

class StapipQBufferRenderer {
 public:
  StapipQBufferRenderer();
  ~StapipQBufferRenderer();

  void init(RendererCore* t_core);

  void reinitVU1();

  void setClipperMVP(M4x4* mvp) { clipper.setMVP(mvp); }

  StapipQBuffer* getBuffer();

  void sendObjectData(StapipBag* bag, M4x4* mvp,
                      RendererCoreTextureBuffers* texBuffers) const;

  void setMaxVertCount(const u32& count);

  void setInfo(StapipInfoBag* bag);

  /** Fast render with culling */
  void cull(StapipQBuffer* buffer);

  /** Slower render with clipping */
  void clip(StapipQBuffer* buffer);

  void clearLastProgramName();

  StapipVU1Program* getCullProgramByBag(const StapipBag* bag);

  StapipVU1Program* getCullProgramByParams(const bool& isLightingEnabled,
                                           const bool& isTextureEnabled);

  const u16& getBufferSize() { return bufferSize; }

 private:
  void sendStaticData() const;
  void setProgramsCache();
  void uploadPrograms();
  void setDoubleBuffer();

  StapipVU1Program* getProgramByName(const StapipProgramName& name);
  void addBufferDataToPacket(StapipVU1Program* program, StapipQBuffer* buffer);
  void sendPacket();
  StapipVU1Program* getAsIsProgramByBag(const StapipBag* bag);
  StapipVU1Program* getCullProgramByType(const StapipProgramType& programType);
  StapipProgramType getDrawProgramTypeByBag(const StapipBag* bag) const;
  StapipProgramType getDrawProgramTypeByParams(
      const bool& isLightingEnabled, const bool& isTextureEnabled) const;
  packet2_t* packets[2];
  packet2_t* programsPacket;
  StapipQBuffer buffers[2];
  packet2_t* currentPacket;
  packet2_t* staticDataPacket;
  packet2_t* objectDataPacket;

  RendererCore* rendererCore;

  StapipProgramName lastProgramName;
  Path1* path1;
  StapipClipper clipper;
  StapipProgramsRepository repository;

  u16 bufferSize;
  u8 context;
};

}  // namespace Tyra
