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

namespace Tyra {

class DynPipQBufferRenderer {
 public:
  DynPipQBufferRenderer();
  ~DynPipQBufferRenderer();

  // void init(RendererCore* t_core);

  // void reinitVU1();

  // DynPipQBuffer* getBuffer();

  // void sendObjectData(DynPipBag* bag, M4x4* mvp,
  //                     RendererCoreTextureBuffers* texBuffers) const;

  // void setMaxVertCount(const u32& count);

  // void setInfo(PipelineInfoBag* bag);

  // void render(DynPipQBuffer* buffer);

  // void clearLastProgramName();

  const u16& getBufferSize() { return bufferSize; }

 private:
  // void sendStaticData() const;
  // void setProgramsCache();
  // void uploadPrograms();
  // void setDoubleBuffer();

  // void addBufferDataToPacket(DynPipVU1Program* program, DynPipQBuffer*
  // buffer); void sendPacket();

  // packet2_t* packets[2];
  // packet2_t* programsPacket;
  // DynPipQBuffer buffers[2];
  // packet2_t* currentPacket;
  // packet2_t* staticDataPacket;
  // packet2_t* objectDataPacket;

  // RendererCore* rendererCore;

  // DynPipProgramName lastProgramName;
  // Path1* path1;
  // DynPipClipper clipper;
  // DynPipProgramsRepository repository;

  u16 bufferSize;
  // u8 context;
};

}  // namespace Tyra
