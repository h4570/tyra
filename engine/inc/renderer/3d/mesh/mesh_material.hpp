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

#include "debug/debug.hpp"
#include "./mesh_material_frame.hpp"
#include "loaders/3d/builder/mesh_builder_data.hpp"
#include <optional>

namespace Tyra {

class MeshMaterial {
 public:
  explicit MeshMaterial(const MeshBuilderData& data, const u32& materialIndex);
  explicit MeshMaterial(const MeshMaterial& material);
  ~MeshMaterial();

  bool isMother, lightmapFlag;
  u32 id;
  std::string name;
  std::optional<std::string> textureName;
  Color ambient;

  std::vector<MeshMaterialFrame*> frames;

  const BBox& getBBox(const u32& frame) const;

  void print() const;
  void print(const char* name) const;
  void print(const std::string& name) const { print(name.c_str()); }
  std::string getPrint(const char* name = nullptr) const;
};

}  // namespace Tyra
