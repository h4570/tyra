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

#include "./bag/dynpip_bag.hpp"

namespace Tyra {

class DynPipQBuffer {
 public:
  DynPipQBuffer();
  ~DynPipQBuffer();

  void setMaxVertCount(const u32& count);

  DynPipBag* bag;

  Vec4* verticesFrom;
  Vec4* verticesTo;

  Vec4* stsFrom;
  Vec4* stsTo;

  Vec4* normalsFrom;
  Vec4* normalsTo;

  /** Size per frame */
  u32 size;

  void print() const;
  void print(const char* name) const;
  void print(const std::string& name) const { print(name.c_str()); }
  std::string getPrint(const char* name = nullptr) const;

 private:
  u32 maxVertCount;
};

}  // namespace Tyra
