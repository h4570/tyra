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

#include "shared/debug/debug.hpp"
#include "./mesh_material_frame.hpp"
#include "shared/loaders/3d/builder/mesh_builder_data.hpp"
#include <optional>

namespace Tyra {

class MeshMaterial {
 public:
  explicit MeshMaterial(const MeshBuilderData& data,
                        const unsigned int& materialIndex);
  explicit MeshMaterial(const MeshMaterial& material);
  ~MeshMaterial();

  bool isMother, lightmapFlag;
  unsigned int id;
  std::string name;
  std::optional<std::string> textureName;
  Color ambient;

  std::vector<MeshMaterialFrame*> frames;

  const BBox& getBBox(const unsigned int& frame) const;

  void print() const;
  void print(const char* name) const;
  void print(const std::string& name) const { print(name.c_str()); }
  std::string getPrint(const char* name = nullptr) const;
};

}  // namespace Tyra
