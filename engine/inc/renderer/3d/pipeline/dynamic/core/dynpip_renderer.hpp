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

#include <tamtypes.h>
#include "renderer/core/renderer_core.hpp"
#include "./dynpip_programs_repository.hpp"

namespace Tyra {

class DynPipRenderer {
 public:
  DynPipRenderer();
  ~DynPipRenderer();

  void init(RendererCore* t_core, DynPipProgramsRepository* t_programRepo);

  void reinitVU1();

  // DynPipQBuffer* getBuffer();

  void sendObjectData(DynPipBag* bag, M4x4* mvp,
                      RendererCoreTextureBuffers* texBuffers) const;

  // void setMaxVertCount(const u32& count);

  // void setInfo(PipelineInfoBag* bag);

  // void render(DynPipQBuffer* buffer);

  void clearLastProgramName();

  const u16& getBufferSize() { return bufferSize; }

 private:
  void sendStaticData() const;
  void setProgramsCache();
  void uploadPrograms();
  void setDoubleBuffer();

  // void addBufferDataToPacket(DynPipVU1Program* program, DynPipQBuffer*
  // buffer); void sendPacket();
  // DynPipQBuffer buffers[2];

  packet2_t* packets[2];
  packet2_t* programsPacket;
  packet2_t* currentPacket;
  packet2_t* staticDataPacket;
  packet2_t* objectDataPacket;

  DynPipProgramsRepository* programsRepo;
  RendererCore* rendererCore;
  Path1* path1;

  DynPipProgramName lastProgramName;

  u16 bufferSize;
  u8 context;
};

}  // namespace Tyra
