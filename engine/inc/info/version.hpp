/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# André Guilherme <andregui17@outlook.com>
*/

#pragma once

#include <string>

namespace Tyra {

class Version {
 public:
  Version() {}
  ~Version() {}

  static const unsigned char major = 2;
  static const unsigned char minor = 1;
  static const unsigned char patch = 0;

  static std::string toString() {
    return std::to_string(major) + "." + std::to_string(minor) + "." +
           std::to_string(patch);
  }
};

}  // namespace Tyra
