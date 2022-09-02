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

#include <math3d.h>
#include <string>
#include "shared/debug/debug.hpp"
#include "./bag/packaging/stapip_bag_package.hpp"
#include "./bag/stapip_bag.hpp"

namespace Tyra {

class StaPipQBuffer {
 public:
  StaPipQBuffer();
  ~StaPipQBuffer();

  void setMaxVertCount(const unsigned int& count);

  /**
   * @brief Dont allocate any dynamic data in buffer.
   * Just copy pointers to input data.
   */
  void fillByPointer(const StaPipBagPackage& pkg);

  /**
   * @brief Allocate dynamic data in buffer
   * And copy input data to it.
   */
  void fillByCopyMax(const StaPipBagPackage& pkg1, const StaPipBagPackage& pkg2,
                     const StaPipBagPackage& pkg3);

  /**
   * @brief Allocate dynamic data in buffer
   * And copy input data to it.
   */
  void fillByCopy1By2(const StaPipBagPackage& pkg1,
                      const StaPipBagPackage& pkg2);

  /**
   * @brief Allocate dynamic data in buffer
   * And copy input data to it.
   */
  void fillByCopy1By3(const StaPipBagPackage& pkg);

  /**
   * @brief Deallocate dynamic data if it was allocated and allocate new data
   * specified by size.
   * @param size 48 is max
   */
  void reallocateManually(const unsigned short& size);

  bool any() const;

  StaPipBag* bag;

  Vec4* vertices;
  Vec4* sts;
  Vec4* colors;
  Vec4* normals;
  unsigned int size;

  void print() const;
  void print(const char* name) const;
  void print(const std::string& name) const { print(name.c_str()); }
  std::string getPrint(const char* name = nullptr) const;

 private:
  unsigned int maxVertCount;
  void deallocateDynamicData();
  void allocateDynamicData(unsigned short size, StaPipBag* bag);
  unsigned char _isDynamicallyAllocated, _stAllocated, _colorAllocated,
      _normalAllocated;
};

}  // namespace Tyra
