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
  StaPipVU1Program(const StaPipProgramName& name, unsigned int* start,
                   unsigned int* end, const unsigned int& t_reglist,
                   const unsigned char& t_reglistCount,
                   const unsigned char& t_elementsPerVertex);
  ~StaPipVU1Program();

  unsigned int& getReglist();

  const StaPipProgramName& getName() const;

  unsigned short getMaxVertCount(const bool& singleColorEnabled,
                                 const unsigned short& vu1DBufferSize) const;

  void addBufferDataToPacket(packet2_t* packet, StaPipQBuffer* buffer,
                             prim_t* prim);

 protected:
  StaPipProgramName name;
  unsigned char reglistCount, elementsPerVertex;
  unsigned int destinationAddress, reglist;

  virtual void addProgramQBufferDataToPacket(packet2_t* packet,
                                             StaPipQBuffer* qbuffer) const = 0;

 private:
  void addStandardBufferDataToPacket(packet2_t* packet, StaPipQBuffer* buffer,
                                     prim_t* prim);
};

}  // namespace Tyra
