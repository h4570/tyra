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

#include "loaders/3d/builder/mesh_builder_data.hpp"
#include "./renderer/3d/bbox/bbox.hpp"

namespace Tyra {

class MeshMaterialFrame {
 public:
  explicit MeshMaterialFrame(const MeshBuilderData& data, const u32& frameIndex,
                             const u32& materialIndex);
  explicit MeshMaterialFrame(const MeshMaterialFrame& frame);
  ~MeshMaterialFrame();

  const u32& getId() const { return id; }
  const u8& isMother() const { return _isMother; }
  const BBox& getBBox() const { return *bbox; }

 private:
  BBox* bbox;

  u8 _isMother;
  u32 id;
};

}  // namespace Tyra
