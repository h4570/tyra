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

#include "./dynpip_program_name.hpp"
#include "shared/renderer/core/paths/path1/vu1_program.hpp"
#include "./programs/dynpip_vu1_shared_defines.h"
#include "./bag/dynpip_bag.hpp"

namespace Tyra {

class DynPipVU1Program : public VU1Program {
 public:
  DynPipVU1Program(const DynPipProgramName& name, unsigned int* start,
                   unsigned int* end, const unsigned int& t_reglist,
                   const unsigned char& t_reglistCount,
                   const unsigned char& t_elementsPerVertex);
  ~DynPipVU1Program();

  unsigned int& getReglist();

  const DynPipProgramName& getName() const;

  unsigned short getMaxVertCount(const unsigned short& vu1DBufferSize) const;

  void addBufferDataToPacket(packet2_t* packet, DynPipBag* bag, prim_t* prim);

 protected:
  DynPipProgramName name;
  unsigned char reglistCount, elementsPerVertex;
  unsigned int destinationAddress, reglist;

  virtual void addProgramQBufferDataToPacket(packet2_t* packet,
                                             DynPipBag* bag) const = 0;

 private:
  void addStandardBufferDataToPacket(packet2_t* packet, DynPipBag* bag,
                                     prim_t* prim);
};

}  // namespace Tyra
