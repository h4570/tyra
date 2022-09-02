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

#include "shared/loaders/3d/builder/mesh_builder_data.hpp"
#include "./shared/renderer/3d/bbox/bbox.hpp"

namespace Tyra {

class MeshMaterialFrame {
 public:
  explicit MeshMaterialFrame(const MeshBuilderData& data,
                             const unsigned int& frameIndex,
                             const unsigned int& materialIndex);
  explicit MeshMaterialFrame(const MeshMaterialFrame& frame);
  ~MeshMaterialFrame();

  unsigned char isMother;

  unsigned int id, count;

  Vec4 *vertices, *textureCoords, *normals;

  Color* colors;

  BBox* bbox;

  void print() const;
  void print(const char* name) const;
  void print(const std::string& name) const { print(name.c_str()); }
  std::string getPrint(const char* name = nullptr) const;
};

}  // namespace Tyra
