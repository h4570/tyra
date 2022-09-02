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

#include "./mesh_material.hpp"
#include "loaders/3d/builder/mesh_builder_data.hpp"
#include "renderer/3d/bbox/bbox.hpp"

namespace Tyra {

class MeshFrame {
 public:
  explicit MeshFrame(const MeshBuilderData& data, const unsigned int& index);
  explicit MeshFrame(const MeshFrame& frame);
  ~MeshFrame();

  unsigned char isMother;

  unsigned int id;

  BBox* bbox;

  void print() const;
  void print(const char* name) const;
  void print(const std::string& name) const { print(name.c_str()); }
  std::string getPrint(const char* name = nullptr) const;
};

}  // namespace Tyra
