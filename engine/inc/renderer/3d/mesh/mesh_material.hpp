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

#include "debug/debug.hpp"
#include "loaders/3d/builder/mesh_builder_data.hpp"
#include "./mesh_material_frame.hpp"

namespace Tyra {

class MeshMaterial {
 public:
  explicit MeshMaterial(const MeshBuilderData& data, const u32& materialIndex);
  explicit MeshMaterial(const MeshMaterial& material);
  ~MeshMaterial();

  Color color;

  const u32& getId() const { return id; }
  const std::string& getName() const { return _name; }
  const u8& isMother() const { return _isMother; }
  const u32& getFramesCount() const { return framesCount; }
  const u8& isSingleColorActivated() const {
    return singleColorFlag;
  }  // TODO: colormode -> color / lightmap

  MeshMaterialFrame** getFrames() const { return frames; }
  MeshMaterialFrame* getFrame(const u32& i) const { return frames[i]; }

  const BBox& getBBox(const u32& frame) const;
  void setSingleColorFlag(const u8& flag);

  void print() const;
  void print(const char* name) const;
  void print(const std::string& name) const { print(name.c_str()); }
  std::string getPrint(const char* name = nullptr) const;

 private:
  MeshMaterialFrame** frames;

  std::string _name;
  u8 _isMother, singleColorFlag;
  u32 id, framesCount;
};

}  // namespace Tyra
