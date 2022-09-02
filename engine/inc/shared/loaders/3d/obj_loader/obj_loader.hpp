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

#include "../builder/mesh_builder_data.hpp"
#include <string>
#include "shared/renderer/models/color.hpp"
#include "shared/loaders/3d/obj_loader/tiny_obj_loader.hpp"
#include <memory.h>
#include <optional>

namespace Tyra {

struct ObjLoaderAnimationOptions {
  unsigned short count = 1;
  unsigned short startingIndex = 1;
};

struct ObjLoaderOptions {
  bool flipUVs = false;
  float scale = 1.0F;
  ObjLoaderAnimationOptions animation;
};

struct MaterialVertexCount {
  size_t materialId;
  int count;
};

/** Class responsible for loading & parsing obj files */
class ObjLoader {
 public:
  static std::unique_ptr<MeshBuilderData> load(const char* fullpath);
  static std::unique_ptr<MeshBuilderData> load(const char* fullpath,
                                               const ObjLoaderOptions& options);
  static std::unique_ptr<MeshBuilderData> load(const std::string& fullpath);
  static std::unique_ptr<MeshBuilderData> load(const std::string& fullpath,
                                               const ObjLoaderOptions& options);

 private:
  static void addOutputMaterialsAndFrames(
      MeshBuilderData* output, const tinyobj::attrib_t& attrib,
      const std::vector<tinyobj::shape_t>& shapes,
      const std::vector<tinyobj::material_t>& materials,
      const unsigned short& framesCount,
      std::vector<MaterialVertexCount>& materialVertexCounts);

  static std::vector<MaterialVertexCount> scan(
      const std::vector<tinyobj::shape_t>& shapes,
      const std::vector<tinyobj::material_t>& materials);

  static void importFrame(MeshBuilderData* output,
                          const tinyobj::attrib_t& attrib,
                          const std::vector<tinyobj::shape_t>& shapes,
                          const std::vector<tinyobj::material_t>& materials,
                          const unsigned short& frameIndex, const float& scale,
                          const bool& invertY, const unsigned short& count);
};

}  // namespace Tyra
