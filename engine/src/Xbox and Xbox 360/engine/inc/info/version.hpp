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

#ifdef OG_XBOX
typedef unsigned char u8;
typedef unsigned int u32;
#elif XBOX_360
#include <stdint.h>
#else
#include <tamtypes.h>
#endif
#include <string>
#ifdef OG_XBOX
#include <iostream>
#include <sstream>
#define nullptr NULL
#endif
namespace Tyra {

class Version {
 public:
  Version() {}
  ~Version() {}

#ifndef XBOX_360
  static const u8 major = 2;
  static const u8 minor = 1;
  static const u8 patch = 0;
#else
  static const _ULonglong major = 2;
  static const _ULonglong minor = 1;
  static const _ULonglong patch = 0;
#endif
  static std::string toString() {
#ifdef OG_XBOX
	  std::stringstream stream1;
	  std::stringstream stream2;
	  std::stringstream stream3;
	  stream1 << major;
	  stream2 << minor;
	  stream3 << patch;
	  std::string str1, str2, str3;
	  stream1 >> str1;
      stream2 >> str2;
	  stream3 >> str3;
	  std::cout << str1 << "." << str2 << "." << str3 << ".";
#else
    return std::to_string(major) + "." + std::to_string(minor) + "." +
           std::to_string(patch);
#endif  
  }
};

}  // namespace Tyra
