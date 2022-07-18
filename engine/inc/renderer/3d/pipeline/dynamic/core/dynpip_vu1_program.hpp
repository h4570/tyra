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

#include "./dynpip_program_name.hpp"
#include "renderer/core/paths/path1/vu1_program.hpp"
#include "./programs/dynpip_vu1_shared_defines.h"
#include "./dynpip_qbuffer.hpp"

namespace Tyra {

class DynPipVU1Program : public VU1Program {
 public:
  DynPipVU1Program(const DynPipProgramName& name, u32* start, u32* end,
                   const u32& t_reglist, const u8& t_reglistCount,
                   const u8& t_elementsPerVertex);
  ~DynPipVU1Program();

  u32& getReglist();

  const DynPipProgramName& getName() const;

  u16 getMaxVertCount(const bool& singleColorEnabled,
                      const u16& vu1DBufferSize) const;

  void addBufferDataToPacket(packet2_t* packet, DynPipQBuffer* buffer,
                             prim_t* prim);

 protected:
  DynPipProgramName name;
  u8 reglistCount, elementsPerVertex;
  u32 destinationAddress, reglist;

  virtual void addProgramQBufferDataToPacket(packet2_t* packet,
                                             DynPipQBuffer* qbuffer) const = 0;

 private:
  void addStandardBufferDataToPacket(packet2_t* packet, DynPipQBuffer* buffer,
                                     prim_t* prim);
};

}  // namespace Tyra
