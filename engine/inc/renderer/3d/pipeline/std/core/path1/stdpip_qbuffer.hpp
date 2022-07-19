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

#include <math3d.h>
#include <string>
#include <sstream>
#include "debug/debug.hpp"
#include "../bag/packaging/stdpip_bag_package.hpp"
#include "../bag/stdpip_bag.hpp"

namespace Tyra {

class StdpipQBuffer {
 public:
  StdpipQBuffer();
  ~StdpipQBuffer();

  void setMaxVertCount(const u32& count);

  /**
   * @brief Dont allocate any dynamic data in buffer.
   * Just copy pointers to input data.
   */
  void fillByPointer(const StdpipBagPackage& pkg);

  /**
   * @brief Allocate dynamic data in buffer
   * And copy input data to it.
   */
  void fillByCopyMax(const StdpipBagPackage& pkg1, const StdpipBagPackage& pkg2,
                     const StdpipBagPackage& pkg3);

  /**
   * @brief Allocate dynamic data in buffer
   * And copy input data to it.
   */
  void fillByCopy1By2(const StdpipBagPackage& pkg1,
                      const StdpipBagPackage& pkg2);

  /**
   * @brief Allocate dynamic data in buffer
   * And copy input data to it.
   */
  void fillByCopy1By3(const StdpipBagPackage& pkg);

  /**
   * @brief Deallocate dynamic data if it was allocated and allocate new data
   * specified by size.
   * @param size 48 is max
   */
  void reallocateManually(const u16& size);

  bool any() const;

  StdpipBag* bag;

  Vec4* vertices;
  Vec4* sts;
  Vec4* colors;
  Vec4* normals;
  u32 size;

  void print() const;
  void print(const char* name) const;
  void print(const std::string& name) const { print(name.c_str()); }
  std::string getPrint(const char* name = nullptr) const;

 private:
  u32 maxVertCount;
  void deallocateDynamicData();
  void allocateDynamicData(u16 size, StdpipBag* bag);
  u8 _isDynamicallyAllocated, _stAllocated, _colorAllocated, _normalAllocated;
};

}  // namespace Tyra
