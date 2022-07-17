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
#include "./stdpip_qbuffer.hpp"
#include "./stdpip_program_type.hpp"
#include "./stdpip_program_name.hpp"
#include "./stdpip_programs_repository.hpp"
#include "./stdpip_clipper.hpp"
#include "renderer/core/paths/path1/path1.hpp"
#include "renderer/core/renderer_core.hpp"
#include "renderer/core/texture/renderer_core_texture_buffers.hpp"

namespace Tyra {

class StdpipQBufferRenderer {
 public:
  StdpipQBufferRenderer();
  ~StdpipQBufferRenderer();

  void init(RendererCore* t_core);

  void reinitVU1();

  void setClipperMVP(M4x4* mvp) { clipper.setMVP(mvp); }

  StdpipQBuffer* getBuffer();

  void sendObjectData(StdpipBag* bag, M4x4* mvp,
                      RendererCoreTextureBuffers* texBuffers) const;

  void setMaxVertCount(const u32& count);

  void setInfo(StdpipInfoBag* bag);

  /** Fast render with culling */
  void cull(StdpipQBuffer* buffer);

  /** Slower render with clipping */
  void clip(StdpipQBuffer* buffer);

  void clearLastProgramName();

  StdpipVU1Program* getCullProgramByBag(const StdpipBag* bag);

  StdpipVU1Program* getCullProgramByParams(const bool& isLightingEnabled,
                                           const bool& isTextureEnabled);

  const u16& getBufferSize() { return bufferSize; }

 private:
  void sendStaticData() const;
  void setProgramsCache();
  void uploadPrograms();
  void setDoubleBuffer();

  StdpipVU1Program* getProgramByName(const StdpipProgramName& name);
  void addBufferDataToPacket(StdpipVU1Program* program, StdpipQBuffer* buffer);
  void sendPacket();
  StdpipVU1Program* getAsIsProgramByBag(const StdpipBag* bag);
  StdpipVU1Program* getCullProgramByType(const StdpipProgramType& programType);
  StdpipProgramType getDrawProgramTypeByBag(const StdpipBag* bag) const;
  StdpipProgramType getDrawProgramTypeByParams(
      const bool& isLightingEnabled, const bool& isTextureEnabled) const;
  packet2_t* packets[2];
  packet2_t* programsPacket;
  StdpipQBuffer buffers[2];
  packet2_t* currentPacket;
  packet2_t* staticDataPacket;
  packet2_t* objectDataPacket;

  RendererCore* rendererCore;

  StdpipProgramName lastProgramName;
  Path1* path1;
  StdpipClipper clipper;
  StdpipProgramsRepository repository;

  u16 bufferSize;
  u8 context;
};

}  // namespace Tyra
