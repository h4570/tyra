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

#include "../../loader.hpp"
#include "../builder/mesh_builder_data.hpp"
#include <string>
#include "renderer/models/color.hpp"
#include "loaders/3d/obj_loader/tiny_obj_loader.hpp"

namespace Tyra {

struct ObjLoaderAnimationOptions {
  u16 count = 1;
  u16 startingIndex = 1;
};

struct ObjLoaderOptions {
  bool flipUVs = false;
  float scale = 1.0F;
  ObjLoaderAnimationOptions animation;
};

/** Class responsible for loading & parsing obj files */
class ObjLoader : public Loader {
 public:
  ObjLoader();
  ~ObjLoader();

  MeshBuilderData* load(const char* fullpath);
  MeshBuilderData* load(const char* fullpath, const ObjLoaderOptions& options);
  MeshBuilderData* load(const std::string& fullpath);
  MeshBuilderData* load(const std::string& fullpath,
                        const ObjLoaderOptions& options);

 private:
  void addOutputMaterialsAndFrames(
      MeshBuilderData* output, const tinyobj::attrib_t& attrib,
      const std::vector<tinyobj::shape_t>& shapes,
      const std::vector<tinyobj::material_t>& materials,
      const u16& framesCount);

  void scan(MeshBuilderData* output, const tinyobj::attrib_t& attrib,
            const std::vector<tinyobj::shape_t>& shapes,
            const std::vector<tinyobj::material_t>& materials,
            const u16& frameIndex);

  void importFrame(MeshBuilderData* output, const tinyobj::attrib_t& attrib,
                   const std::vector<tinyobj::shape_t>& shapes,
                   const std::vector<tinyobj::material_t>& materials,
                   const u16& frameIndex, const float& scale,
                   const bool& invertY, const u16& count);
};

}  // namespace Tyra
