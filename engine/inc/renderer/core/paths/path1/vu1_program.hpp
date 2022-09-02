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

#include <packet2_utils.h>
#include <string>

namespace Tyra {

class VU1Program {
 public:
  VU1Program(unsigned int* start, unsigned int* end);
  ~VU1Program();

  const unsigned int& getPacketSize() const;
  const unsigned int& getProgramSize() const;
  const unsigned int& getDestinationAddress() const;

  unsigned int* getStart() const;
  unsigned int* getEnd() const;

  virtual std::string getStringName() const = 0;

  void setDestinationAddress(const unsigned int& addr);

 protected:
  unsigned int packetSize, destinationAddress, programSize;
  unsigned int *start, *end;
  unsigned int calculateProgramSize() const;
};

}  // namespace Tyra
