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

/** Class responsible for loading & parsing custom Tyra obj files */
class ObjLoader : public Loader {
 public:
  ObjLoader();
  ~ObjLoader();

  /** Load multiple obj files (dynamic) */
  MeshBuilderData* load(const char* fullpath, const u16& count,
                        const float& scale, const bool& invertT);

  inline MeshBuilderData* load(const std::string& fullpath, const u16& count,
                               const float& scale, const bool& invertT) {
    return load(fullpath.c_str(), count, scale, invertT);
  }

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
