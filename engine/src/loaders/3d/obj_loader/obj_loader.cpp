/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include <cstring>
#include <stdio.h>
#include <string>
#include <vector>
#include "math/vec2.hpp"
#include "math/vec4.hpp"
#include "renderer/models/color.hpp"
#include "debug/debug.hpp"
#include "loaders/3d/obj_loader/obj_loader.hpp"

#define TINYOBJLOADER_USE_MAPBOX_EARCUT
#define TINYOBJLOADER_IMPLEMENTATION
#include "loaders/3d/obj_loader/tiny_obj_loader.hpp"

namespace Tyra {

ObjLoader::ObjLoader() {}

ObjLoader::~ObjLoader() {}

std::unique_ptr<MeshBuilderData> ObjLoader::load(const char* fullpath) {
  return load(fullpath, ObjLoaderOptions());
}

std::unique_ptr<MeshBuilderData> ObjLoader::load(const std::string& fullpath) {
  return load(fullpath.c_str(), ObjLoaderOptions());
}

std::unique_ptr<MeshBuilderData> ObjLoader::load(
    const std::string& fullpath, const ObjLoaderOptions& options) {
  return load(fullpath.c_str(), options);
}

std::unique_ptr<MeshBuilderData> ObjLoader::load(
    const char* fullpath, const ObjLoaderOptions& options) {
  std::string path = fullpath;
  std::string basePath = getPathFromFilename(path);

  TYRA_ASSERT(!path.empty(), "Provided path is empty!");

  auto rawFilename = getFilenameWithoutExtension(path);
  auto extension = getExtensionOfFilename(path);
  auto result = std::make_unique<MeshBuilderData>();

  tinyobj::ObjReaderConfig readerConfig;
  readerConfig.triangulate = options.animation.count == 1;
  readerConfig.triangulation_method =
      options.animation.count == 1 ? "earcut" : "simple";
  readerConfig.mtl_search_path = basePath;

  for (auto i = 1; i <= options.animation.count; i++) {
    int indexStringLength = std::to_string(i).length();
    auto numbersWithLeadingZeros =
        std::string(6 - std::min(6, indexStringLength), '0') +
        std::to_string(i);

    std::string filePath =
        rawFilename + "_" + numbersWithLeadingZeros + "." + extension;
    if (options.animation.count == 1) filePath = path;

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(filePath, readerConfig)) {
      if (!reader.Error().empty()) {
        TYRA_TRAP("TinyObjLoader: ", reader.Error());
      }
      TYRA_TRAP("Unknown TinyObjLoader error!");
    }

    if (!reader.Warning().empty()) {
      TYRA_WARN("TinyObjReader: ", reader.Warning());
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    TYRA_ASSERT(
        materials.size() > 0,
        "No material data found! Please add .mtl file(s) and assign them via "
        "mtlib in obj file");

    if (i == 1) {
      auto scanResult = scan(shapes, materials);
      addOutputMaterialsAndFrames(result.get(), attrib, shapes, materials,
                                  options.animation.count, scanResult);
    }

    importFrame(result.get(), attrib, shapes, materials, i - 1, options.scale,
                options.flipUVs, options.animation.count);
  }

  return result;
}

std::vector<MaterialVertexCount> ObjLoader::scan(
    const std::vector<tinyobj::shape_t>& shapes,
    const std::vector<tinyobj::material_t>& materials) {
  std::vector<MaterialVertexCount> result;

  for (size_t i = 0; i < materials.size(); i++) {
    MaterialVertexCount counter = {i, 0};
    result.push_back(counter);
  }

  for (size_t s = 0; s < shapes.size(); s++) {
    const auto& mesh = shapes[s].mesh;

    for (size_t f = 0; f < mesh.num_face_vertices.size(); f++) {
      const auto& materialId = mesh.material_ids[f];
      const auto& vertCountPerFace = size_t(mesh.num_face_vertices[f]);

      TYRA_ASSERT(vertCountPerFace == 3,
                  "TinyObjLoader should triangulate mesh, internal error!");

      result[materialId].count += vertCountPerFace;
    }
  }

  return result;
}

void ObjLoader::addOutputMaterialsAndFrames(
    MeshBuilderData* output, const tinyobj::attrib_t& attrib,
    const std::vector<tinyobj::shape_t>& shapes,
    const std::vector<tinyobj::material_t>& materials, const u16& framesCount,
    std::vector<MaterialVertexCount>& materialVertexCounts) {
  if (attrib.texcoords.size()) output->textureCoordsEnabled = true;

  if (attrib.normals.size()) output->normalsEnabled = true;

  TYRA_ASSERT(materials.size() > 0,
              "No material data found! Please add .mtl "
              "file(s) and assign them via mtlib in obj "
              "file");

  for (size_t i = 0; i < materials.size(); i++) {
    auto* material = new MeshBuilderMaterialData();

    material->name = materials[i].name;

    material->ambient.set(materials[i].diffuse[0] * 128.0F,
                          materials[i].diffuse[1] * 128.0F,
                          materials[i].diffuse[2] * 128.0F, 128.0F);

    for (size_t j = 0; j < framesCount; j++) {
      auto* frame = new MeshBuilderMaterialFrameData();

      const auto& counter = materialVertexCounts[i];

      frame->count = counter.count;
      frame->vertices = new Vec4[counter.count];

      if (output->textureCoordsEnabled)
        frame->textureCoords = new Vec4[counter.count];

      if (output->normalsEnabled) frame->normals = new Vec4[counter.count];

      material->frames.push_back(frame);
    }

    output->materials.push_back(material);
  }
}

void ObjLoader::importFrame(MeshBuilderData* output,
                            const tinyobj::attrib_t& attrib,
                            const std::vector<tinyobj::shape_t>& shapes,
                            const std::vector<tinyobj::material_t>& materials,
                            const u16& frameIndex, const float& scale,
                            const bool& invertY, const u16& count) {
  struct MaterialInsertControl {
    size_t materialId;
    int inserted;
  };

  std::vector<MaterialInsertControl> materialInsertControls;
  for (size_t i = 0; i < materials.size(); i++) {
    MaterialInsertControl control = {i, 0};
    materialInsertControls.push_back(control);
  }

  // Loop over shapes
  for (size_t s = 0; s < shapes.size(); s++) {
    const auto& mesh = shapes[s].mesh;

    size_t index_offset = 0;
    // Loop over faces
    for (size_t f = 0; f < mesh.num_face_vertices.size(); f++) {
      auto materialId = mesh.material_ids[f];
      auto& materialInsertControl = materialInsertControls[materialId];
      size_t vertCountPerFace = size_t(mesh.num_face_vertices[f]);

      auto* outFrame = output->materials[materialId]->frames[frameIndex];

      TYRA_ASSERT(count == 1 || vertCountPerFace == 3,
                  "Please triangulate obj files if you are animating!",
                  "Recommended Blender options: ",
                  "- Obj exporting: triangulate off, keep vertex order",
                  "- Object modifiers: triangulate (as first!), then other "
                  "modifiers");

      // Loop over vertices in the face.
      for (size_t v = 0; v < vertCountPerFace; v++) {
        // access to vertex
        tinyobj::index_t idx = mesh.indices[index_offset + v];
        tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
        tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
        tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

        outFrame->vertices[materialInsertControl.inserted].set(vx, vy, vz,
                                                               1.0F);
        outFrame->vertices[materialInsertControl.inserted] *= scale;

        // Check if `normal_index` is zero or positive. negative = no normal
        // data
        if (idx.normal_index >= 0) {
          tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
          tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
          tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];

          outFrame->normals[materialInsertControl.inserted].set(nx, ny, nz,
                                                                1.0F);
        }

        // Check if `texcoord_index` is zero or positive. negative =
        // notexcoord data
        if (idx.texcoord_index >= 0) {
          tinyobj::real_t tx =
              attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
          tinyobj::real_t ty =
              attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];

          auto finalY = invertY ? 1.0F - ty : ty;
          outFrame->textureCoords[materialInsertControl.inserted].set(
              tx, finalY, 1.0F, 0.0F);
        }

        materialInsertControl.inserted++;
      }

      index_offset += vertCountPerFace;
    }
  }
}

}  // namespace Tyra
