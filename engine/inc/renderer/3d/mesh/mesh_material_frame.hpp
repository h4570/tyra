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

#include "loaders/3d/builder2/mesh_builder2_data.hpp"
#include "./renderer/3d/bbox/bbox.hpp"

namespace Tyra {

class MeshMaterialFrame {
 public:
  explicit MeshMaterialFrame(const MeshBuilder2Data& data,
                             const u32& frameIndex, const u32& materialIndex);
  explicit MeshMaterialFrame(const MeshMaterialFrame& frame);
  ~MeshMaterialFrame();

  const u32& getId() const { return id; }
  const u8& isMother() const { return _isMother; }
  const BBox& getBBox() const { return *bbox; }

  const u32& getVertexCount() const { return count; }
  Vec4* getVertices() const { return vertices; }
  Vec4* getNormals() const { return normals; }
  Vec4* getTextureCoords() const { return textureCoords; }
  Color* getColors() const { return colors; }

  void print() const;
  void print(const char* name) const;
  void print(const std::string& name) const { print(name.c_str()); }
  std::string getPrint(const char* name = nullptr) const;

 private:
  BBox* bbox;

  u8 _isMother;
  u32 id, count;
  Vec4 *vertices, *textureCoords, *normals;
  Color* colors;
};

}  // namespace Tyra
