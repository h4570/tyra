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

namespace Tyra {

struct TyraobjData {
  u32 vertexCount, stsCount, normalsCount, colorsCount, materialsCount,
      framesCount;
  u32* materialsFaces;
  float scale;
  bool invertT;
};

struct TyraobjReadInfo {
  u32 verticesI, coordsI, normalsI, colorsI, faceI;
  s16 materialsI;
};

/** Class responsible for loading & parsing custom Tyra obj files */
class TyrobjLoader : public Loader {
 public:
  TyrobjLoader();
  ~TyrobjLoader();

  /** Load multiple tyrobj files (dynamic) */
  MeshBuilderData* load(const char* fullpath, const u16& count,
                        const float& scale, const bool& invertT);

  inline MeshBuilderData* load(const std::string& fullpath, const u16& count,
                               const float& scale, const bool& invertT) {
    return load(fullpath.c_str(), count, scale, invertT);
  }

 private:
  TyraobjData scan(FILE* file, const u32& framesCount);
  void loadFile(FILE* file, const std::string& path, const u16& frameIndex,
                const TyraobjData& inputData, MeshBuilderData* outputData);

  void initReadInfo(TyraobjReadInfo* info);
  void readVertices(TyraobjReadInfo* info, FILE* file, const u16& frameIndex,
                    const TyraobjData& inputData, MeshBuilderData* outputData);
  void readTextureCoords(TyraobjReadInfo* info, FILE* file,
                         const u16& frameIndex, const TyraobjData& inputData,
                         MeshBuilderData* outputData);
  void readNormals(TyraobjReadInfo* info, FILE* file, const u16& frameIndex,
                   MeshBuilderData* outputData);
  void readColors(TyraobjReadInfo* info, FILE* file, const u16& frameIndex,
                  MeshBuilderData* outputData);
  void readMaterials(TyraobjReadInfo* info, FILE* file,
                     const TyraobjData& inputData, MeshBuilderData* outputData);
  void readFaces(TyraobjReadInfo* info, FILE* file,
                 MeshBuilderData* outputData);
};

}  // namespace Tyra
