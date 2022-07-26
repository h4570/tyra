/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Michał Mostowik <mostek3pl@gmail.com>
*/

#include <cstring>
#include <stdio.h>
#include <string>
#include "math/vec2.hpp"
#include "math/vec4.hpp"
#include "renderer/models/color.hpp"
#include "debug/debug.hpp"
#include "loaders/3d/tyrobj/tyrobj_loader.hpp"

namespace Tyra {

TyrobjLoader::TyrobjLoader() {}

TyrobjLoader::~TyrobjLoader() {}

MeshBuilderData* TyrobjLoader::load(const char* fullpath, const u16& count,
                                    const float& scale, const bool& invertT) {
  std::string path = fullpath;
  TYRA_ASSERT(!path.empty(), "Provided path is empty!");

  auto rawFilename = getFilenameWithoutExtension(path);
  auto extension = getExtensionOfFilename(path);

  std::string firstFile = path;
  if (count > 1) firstFile = rawFilename + "1." + extension;

  FILE* firstFileHandler = fopen(firstFile.c_str(), "rb");
  TYRA_ASSERT(firstFileHandler != nullptr, "Failed to load: ", firstFile);
  auto data = scan(firstFileHandler, count);
  data.scale = scale;
  data.invertT = invertT;
  fclose(firstFileHandler);

  auto result = new MeshBuilderData();
  result->allocate(data.framesCount, data.materialsCount);
  result->normalsEnabled = data.normalsCount > 0;
  result->textureCoordsEnabled = data.stsCount > 0;
  result->manyColorsEnabled = data.colorsCount > 0;

  for (u16 i = 1; i <= count; i++) {
    std::string filePath = rawFilename + std::to_string(i) + "." + extension;
    if (count == 1) filePath = path;

    FILE* fileHandler = fopen(filePath.c_str(), "rb");
    TYRA_ASSERT(fileHandler != nullptr, "Failed to load: ", filePath);
    loadFile(fileHandler, filePath, i - 1, data, result);
    fclose(fileHandler);
  }

  delete data.materialsFaces;
  return result;
}

TyraobjData TyrobjLoader::scan(FILE* file, const u32& framesCount) {
  TyraobjData result;
  result.vertexCount = 0;
  result.stsCount = 0;
  result.normalsCount = 0;
  result.colorsCount = 0;
  result.materialsCount = 0;
  result.framesCount = framesCount;

  while (1) {
    char lineHeader[128];
    int res = fscanf(file, "%s", lineHeader);
    if (res != EOF) {
      if (strcmp(lineHeader, "v") == 0)
        result.vertexCount += 1;
      else if (strcmp(lineHeader, "vt") == 0)
        result.stsCount += 1;
      else if (strcmp(lineHeader, "vn") == 0)
        result.normalsCount += 1;
      else if (strcmp(lineHeader, "vc") == 0)
        result.colorsCount += 1;
      else if (strcmp(lineHeader, "usemtl") == 0)
        result.materialsCount += 1;
    } else
      break;
  }

  result.materialsFaces = new u32[result.materialsCount];
  s16 currentMatI = -1;

  fseek(file, 0, SEEK_SET);
  u32 facesCounter = 0;
  while (1) {
    char lineHeader[128];
    int res = fscanf(file, "%s", lineHeader);
    if (res != EOF) {
      if (strcmp(lineHeader, "usemtl") == 0) {
        if (currentMatI >= 0)  // Skip -1
        {
          result.materialsFaces[currentMatI] = facesCounter;
          facesCounter = 0;
        }
        currentMatI++;
      } else if (strcmp(lineHeader, "f") == 0)
        facesCounter += 3;
    } else
      break;
  }

  result.materialsFaces[currentMatI] = facesCounter;  // Allocate last one

  return result;
}

void TyrobjLoader::loadFile(FILE* file, const std::string& path,
                            const u16& frameIndex, const TyraobjData& inputData,
                            MeshBuilderData* outputData) {
  outputData->frames[frameIndex]->allocateVertices(inputData.vertexCount);
  outputData->frames[frameIndex]->allocateNormals(inputData.normalsCount);
  outputData->frames[frameIndex]->allocateTextureCoords(inputData.stsCount);
  outputData->frames[frameIndex]->allocateColors(inputData.colorsCount);

  TyraobjReadInfo readInfo;
  initReadInfo(&readInfo);

  int res = 0;
  char* lineHeader = new char[128];

  while (res != EOF) {
    if (strcmp(lineHeader, "v") == 0) {
      readVertices(&readInfo, file, frameIndex, inputData, outputData);
    } else if (strcmp(lineHeader, "vt") == 0) {
      readTextureCoords(&readInfo, file, frameIndex, inputData, outputData);
    } else if (strcmp(lineHeader, "vn") == 0) {
      readNormals(&readInfo, file, frameIndex, inputData, outputData);
    } else if (strcmp(lineHeader, "vc") == 0) {
      readColors(&readInfo, file, frameIndex, inputData, outputData);
    } else if (strcmp(lineHeader, "usemtl") == 0) {
      readMaterials(&readInfo, file, frameIndex, inputData, outputData);
    } else if (strcmp(lineHeader, "f") == 0) {
      readFaces(&readInfo, file, frameIndex, inputData, outputData);
    }

    res = fscanf(file, "%s", lineHeader);
  }

  delete[] lineHeader;
}

void TyrobjLoader::initReadInfo(TyraobjReadInfo* info) {
  info->verticesI = 0;
  info->coordsI = 0;
  info->normalsI = 0;
  info->colorsI = 0;
  info->faceI = 0;
  info->materialsI = -1;
}

void TyrobjLoader::readVertices(TyraobjReadInfo* info, FILE* file,
                                const u16& frameIndex,
                                const TyraobjData& inputData,
                                MeshBuilderData* outputData) {
  Vec4 vector(0.0F, 0.0F, 0.0F, 1.0F);
  fscanf(file, "%f %f %f\n", &vector.x, &vector.y, &vector.z);
  outputData->frames[frameIndex]->vertices[info->verticesI++].set(
      vector * inputData.scale);
}

void TyrobjLoader::readTextureCoords(TyraobjReadInfo* info, FILE* file,
                                     const u16& frameIndex,
                                     const TyraobjData& inputData,
                                     MeshBuilderData* outputData) {
  Vec2 point(0.0F, 0.0F);
  fscanf(file, "%f %f\n", &point.x, &point.y);
  if (inputData.invertT) point.y = 1.0F - point.y;
  outputData->frames[frameIndex]->textureCoords[info->coordsI++].set(
      point.x, point.y, 1.0F, 0.0F);
}

void TyrobjLoader::readNormals(TyraobjReadInfo* info, FILE* file,
                               const u16& frameIndex,
                               const TyraobjData& inputData,
                               MeshBuilderData* outputData) {
  Vec4 vector(0.0F, 0.0F, 0.0F, 1.0F);
  fscanf(file, "%f %f %f\n", &vector.x, &vector.y, &vector.z);
  outputData->frames[frameIndex]->normals[info->normalsI++].set(vector);
}

void TyrobjLoader::readColors(TyraobjReadInfo* info, FILE* file,
                              const u16& frameIndex,
                              const TyraobjData& inputData,
                              MeshBuilderData* outputData) {
  Color color(128.0F, 128.0F, 128.0F, 128.0F);
  fscanf(file, "%f %f %f %f\n", &color.r, &color.g, &color.b, &color.a);
  outputData->frames[frameIndex]->colors[info->colorsI++].set(color);
}

void TyrobjLoader::readMaterials(TyraobjReadInfo* info, FILE* file,
                                 const u16& frameIndex,
                                 const TyraobjData& inputData,
                                 MeshBuilderData* outputData) {
  char temp[30];
  fscanf(file, "%s\n", temp);
  info->materialsI++;
  outputData->materials[info->materialsI]->allocateFaces(
      inputData.materialsFaces[info->materialsI]);
  outputData->materials[info->materialsI]->name = temp;
  info->faceI = 0;
}

int test123 = 0;

void TyrobjLoader::readFaces(TyraobjReadInfo* info, FILE* file,
                             const u16& frameIndex,
                             const TyraobjData& inputData,
                             MeshBuilderData* outputData) {
  int* x = new int[9];
  fpos_t start;
  fgetpos(file, &start);
  int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &x[0], &x[1],
                       &x[2], &x[3], &x[4], &x[5], &x[6], &x[7], &x[8]);
  fsetpos(file, &start);

  // TODO: No support of vc!
  u32 vertexIndex[3], coordIndex[3], normalIndex[3];  // ,colorIndex[3];
  for (u16 i = 0; i < 3; i++) {
    vertexIndex[i] = 0;
    coordIndex[i] = 0;
    normalIndex[i] = 0;
    // colorIndex[i] = 0;
  }

  int newerMatches = 0;

  switch (matches) {
    /** Vs, VTs and VNs all set */
    case 9: {
      fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0],
             &coordIndex[0], &normalIndex[0], &vertexIndex[1], &coordIndex[1],
             &normalIndex[1], &vertexIndex[2], &coordIndex[2], &normalIndex[2]);
    } break;

    /** Loaded only two digits (V, VT) succesfuly. Not setting
    VN. */
    case 3: {
      fscanf(file, "%d/%d/ %d/%d/ %d/%d/\n", &vertexIndex[0], &coordIndex[0],
             &vertexIndex[1], &coordIndex[1], &vertexIndex[2], &coordIndex[2]);
    } break;

    /** Only V set. Checking for existance of VT or VN. */
    case 1: {
      /** Check for existance of V/// configuration.. */
      newerMatches = fscanf(file, "%d// %d// %d//\n", &x[0], &x[1], &x[2]);
      fsetpos(file, &start);
      if (newerMatches == 3) {
        /** Configuration confirmed. */
        fscanf(file, "%d// %d// %d//\n", &vertexIndex[0], &vertexIndex[1],
               &vertexIndex[2]);
      } else {
        /** Failed, checking configuration V//VN */
        newerMatches = fscanf(file, "%d//%d %d//%d %d//%d", &x[0], &x[1], &x[2],
                              &x[3], &x[4], &x[5]);
        fsetpos(file, &start);
        TYRA_ASSERT(newerMatches == 6, "Unknown .obj face for .obj file!");
        /** Configuration confirmed. */
        newerMatches = fscanf(file, "%d//%d %d//%d %d//%d", &vertexIndex[0],
                              &normalIndex[0], &vertexIndex[1], &normalIndex[1],
                              &vertexIndex[2], &normalIndex[2]);
      }
      break;
    } break;
    default: {
      TYRA_TRAP("Unknown faces format in .obj file!");
      break;
    }
  }

  // Set vertex indices
  if (matches == 9 || matches == 2 || matches == 1) {
    outputData->materials[info->materialsI]->vertexFaces[info->faceI] =
        vertexIndex[0] - 1;
    outputData->materials[info->materialsI]->vertexFaces[info->faceI + 1] =
        vertexIndex[1] - 1;
    outputData->materials[info->materialsI]->vertexFaces[info->faceI + 2] =
        vertexIndex[2] - 1;
  }

  // Set texture coord indices
  if (matches == 9 || matches == 2) {
    outputData->materials[info->materialsI]->textureCoordFaces[info->faceI] =
        coordIndex[0] - 1;
    outputData->materials[info->materialsI]
        ->textureCoordFaces[info->faceI + 1] = coordIndex[1] - 1;
    outputData->materials[info->materialsI]
        ->textureCoordFaces[info->faceI + 2] = coordIndex[2] - 1;
  }

  // Set normal indices
  if (matches == 9) {
    outputData->materials[info->materialsI]->normalFaces[info->faceI] =
        normalIndex[0] - 1;
    outputData->materials[info->materialsI]->normalFaces[info->faceI + 1] =
        normalIndex[1] - 1;
    outputData->materials[info->materialsI]->normalFaces[info->faceI + 2] =
        normalIndex[2] - 1;
  } else if (matches == 1 && newerMatches == 6) {
    outputData->materials[info->materialsI]->normalFaces[info->faceI] =
        normalIndex[0] - 1;
    outputData->materials[info->materialsI]->normalFaces[info->faceI + 1] =
        normalIndex[1] - 1;
    outputData->materials[info->materialsI]->normalFaces[info->faceI + 2] =
        normalIndex[2] - 1;
  }

  // Push index
  info->faceI += 3;

  delete[] x;
}

}  // namespace Tyra
