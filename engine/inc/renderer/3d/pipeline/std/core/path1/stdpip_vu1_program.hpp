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

#include "./stdpip_program_name.hpp"
#include "renderer/core/paths/path1/vu1_program.hpp"
#include "./stdpip_qbuffer.hpp"
#include "./programs/stdpip_vu1_shared_defines.h"

namespace Tyra {

class StdpipVU1Program : public VU1Program {
 public:
  StdpipVU1Program(const StdpipProgramName& name, u32* start, u32* end,
                   const u32& t_reglist, const u8& t_reglistCount,
                   const u8& t_elementsPerVertex);
  ~StdpipVU1Program();

  u32& getReglist();

  const StdpipProgramName& getName() const;

  u16 getMaxVertCount(const bool& singleColorEnabled,
                      const u16& vu1DBufferSize) const;

  void addBufferDataToPacket(packet2_t* packet, StdpipQBuffer* buffer,
                             prim_t* prim);

 protected:
  StdpipProgramName name;
  u8 reglistCount, elementsPerVertex;
  u32 destinationAddress, reglist;

  virtual void addProgramQBufferDataToPacket(packet2_t* packet,
                                             StdpipQBuffer* qbuffer) const = 0;

 private:
  void addStandardBufferDataToPacket(packet2_t* packet, StdpipQBuffer* buffer,
                                     prim_t* prim);
};

}  // namespace Tyra
