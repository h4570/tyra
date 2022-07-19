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

#include "renderer/3d/mesh/mesh_material.hpp"
#include "loaders/3d/builder/mesh_builder_data.hpp"
#include "renderer/3d/bbox/bbox.hpp"

namespace Tyra {

class MeshFrame {
 public:
  explicit MeshFrame(const MeshBuilderData& data, const u32& index);
  explicit MeshFrame(const MeshFrame& frame);
  ~MeshFrame();

  const u32& getId() const { return id; }
  const u32& getVertexCount() const { return vertexCount; }
  const u32& getTextureCoordsCount() const { return textureCoordsCount; }
  const u32& getNormalsCount() const { return normalsCount; }
  const u32& getColorsCount() const { return colorsCount; }
  const u8& isMother() const { return _isMother; }
  const BBox& getBBox() const { return *bbox; }

  Vec4* getVertices() const { return vertices; }
  Vec4* getNormals() const { return normals; }
  Vec4* getTextureCoords() const { return textureCoords; }
  Color* getColors() const { return colors; }

  void print() const;
  void print(const char* name) const;
  void print(const std::string& name) const { print(name.c_str()); }
  std::string getPrint(const char* name = nullptr) const;

 private:
  u8 _isMother;
  BBox* bbox;
  u32 id, vertexCount, textureCoordsCount, normalsCount, colorsCount;
  Vec4 *vertices, *textureCoords, *normals;
  Color* colors;
};

}  // namespace Tyra
