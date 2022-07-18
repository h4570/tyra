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

#include "./renderer/3d/bbox/bbox.hpp"

namespace Tyra {

class DynamicMeshMaterialFrame {
 public:
  explicit DynamicMeshMaterialFrame(const MeshBuilderData& data,
                                    const u32& frameIndex,
                                    const u32& materialIndex);
  explicit DynamicMeshMaterialFrame(const DynamicMeshMaterialFrame& frame);
  ~DynamicMeshMaterialFrame();

  const u32& getId() const { return id; }
  const u8& isMother() const { return _isMother; }
  const BBox& getBBox() const { return *bbox; }

 private:
  BBox* bbox;

  u8 _isMother;
  u32 id;
};

}  // namespace Tyra
