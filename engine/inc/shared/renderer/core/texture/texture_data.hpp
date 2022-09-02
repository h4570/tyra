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

#include <string>
#include "shared/renderer/core/texture/models/texture_bpp.hpp"

namespace Tyra {

class TextureData {
 public:
  TextureData(unsigned char* data, const TextureBpp& bpp,
              const unsigned char& gsComponents, const int& width,
              const int& height);
  ~TextureData();

  int width, height;
  unsigned int psm;
  unsigned char* data;
  TextureBpp bpp;
  unsigned char components;

  void print() const;
  void print(const unsigned char* name) const;
  void print(const std::string& name) const { print(name.c_str()); }
  std::string getPrint(const unsigned char* name = nullptr) const;

 private:
  unsigned int getPsmByBpp(const TextureBpp& bpp);
};
}  // namespace Tyra
