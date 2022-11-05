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

#include "./stapip_program_name.hpp"
#include "renderer/core/paths/path1/vu1_program.hpp"
#include "./stapip_qbuffer.hpp"
#include "./programs/stapip_vu1_shared_defines.h"

namespace Tyra {

class StaPipVU1Program : public VU1Program {
 public:
  StaPipVU1Program(const StaPipProgramName& name, u32* start, u32* end,
                   const u32& t_reglist, const u8& t_reglistCount,
                   const u8& t_elementsPerVertex);
  ~StaPipVU1Program();

  u32& getReglist();

  const StaPipProgramName& getName() const;

  u16 getMaxVertCount(const bool& singleColorEnabled,
                      const u16& vu1DBufferSize) const;

  void addBufferDataToPacket(packet2_t* packet, StaPipQBuffer* buffer,
                             prim_t* prim);

 protected:
  StaPipProgramName name;
  u8 reglistCount, elementsPerVertex;
  u32 destinationAddress, reglist;

  virtual void addProgramQBufferDataToPacket(packet2_t* packet,
                                             StaPipQBuffer* qbuffer) const = 0;

 private:
  void addStandardBufferDataToPacket(packet2_t* packet, StaPipQBuffer* buffer,
                                     prim_t* prim);
};

}  // namespace Tyra
