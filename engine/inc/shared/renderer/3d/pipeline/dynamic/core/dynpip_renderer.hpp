/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

#include "shared/renderer/core/renderer_core.hpp"
#include "./dynpip_programs_repository.hpp"

namespace Tyra {

class DynPipRenderer {
 public:
  DynPipRenderer();
  ~DynPipRenderer();

  void init(RendererCore* t_core, DynPipProgramsRepository* t_programRepo,
            prim_t* prim, lod_t* lod);

  void reinitVU1();

  void sendObjectData(DynPipBag* bag, M4x4* mvp,
                      RendererCoreTextureBuffers* texBuffers) const;

  void render(DynPipBag** bags, const unsigned int& count);

  void clearLastProgramName();

  const unsigned short& getBufferSize() { return bufferSize; }

  void allocateOnUse(const unsigned int& t_packetSize);
  void deallocateOnUse();

 private:
  void sendStaticData() const;
  void setProgramsCache();
  void uploadPrograms();
  void setDoubleBuffer();

  void addBufferDataToPacket(DynPipBag** bags, const unsigned int& count);
  void sendPacket();

  prim_t* prim;
  lod_t* lod;
  unsigned short packetSize;

  packet2_t* packets[2];
  packet2_t* programsPacket;
  packet2_t* currentPacket;
  packet2_t* staticDataPacket;
  packet2_t* objectDataPacket;

  DynPipProgramsRepository* programsRepo;
  RendererCore* rendererCore;
  Path1* path1;

  DynPipProgramName lastProgramName;

  unsigned short bufferSize;
  unsigned char context;
};

}  // namespace Tyra
