/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "shared/loaders/3d/md2_loader/md2_loader.hpp"
#include <stdio.h>
#include <string>
#include "shared/debug/debug.hpp"
#include "shared/loaders/3d/md2_loader/anorms.hpp"
#include "shared/file/file_utils.hpp"

namespace Tyra {

// magic number "IDP2" or 844121161
#define MD2_IDENT (('2' << 24) + ('P' << 16) + ('D' << 8) + 'I')

// model version
#define MD2_VERSION 8

typedef struct {
  int ident;    // magic number. must be equal to "IDP2"
  int version;  // md2 version. must be equal to 8

  int skinwidth;   // width of the texture
  int skinheight;  // height of the texture
  int framesize;   // size of one frame in bytes

  int num_skins;   // number of textures
  int num_xyz;     // number of vertices
  int num_st;      // number of texture coordinates
  int num_tris;    // number of triangles
  int num_glcmds;  // number of opengl commands
  int num_frames;  // total number of frames

  int ofs_skins;   // offset to skin names (64 bytes each)
  int ofs_st;      // offset to s-t texture coordinates
  int ofs_tris;    // offset to triangles
  int ofs_frames;  // offset to frame data
  int ofs_glcmds;  // offset to opengl commands
  int ofs_end;     // offset to end of file
} md2_t;

typedef struct {
  unsigned char v[3];              // compressed vertex (x, y, z) coordinates
  unsigned char lightnormalindex;  // index to a normal vector for the lighting
} mvertex_t;

typedef struct {
  float scale[3];      // scale values
  float translate[3];  // translation vector
  char name[16];       // frame name
  mvertex_t verts[1];  // first vertex of this frame
} frame_t;

typedef float vec3_t[3];

typedef struct {
  short index_xyz[3];  // indexes to triangle's vertices
  short index_st[3];   // indexes to vertices' texture coorinates
} triangle_t;

typedef struct {
  short s;
  short t;
} texCoord_t;

std::unique_ptr<MeshBuilderData> MD2Loader::load(const char* fullpath) {
  return load(fullpath, MD2LoaderOptions());
}

std::unique_ptr<MeshBuilderData> MD2Loader::load(const std::string& fullpath) {
  return load(fullpath.c_str(), MD2LoaderOptions());
}

std::unique_ptr<MeshBuilderData> MD2Loader::load(const std::string& fullpath,
                                                 MD2LoaderOptions options) {
  return load(fullpath.c_str(), options);
}

std::unique_ptr<MeshBuilderData> MD2Loader::load(const char* fullpath,
                                                 MD2LoaderOptions options) {
  std::string path = fullpath;
  TYRA_ASSERT(!path.empty(), "Provided path is empty!");

  auto filename = FileUtils::getFilenameFromPath(path);

  FILE* file = fopen(fullpath, "rb");
  TYRA_ASSERT(file != nullptr, "Failed to load: ", filename);
  md2_t header;

  fread(reinterpret_cast<char*>(&header), sizeof(md2_t), 1, file);

  TYRA_ASSERT((header.ident == MD2_IDENT) && (header.version == MD2_VERSION),
              "This MD2 file is not in correct format!");

  unsigned int framesCount = header.num_frames;
  unsigned int vertexCount = header.num_xyz;
  unsigned int stsCount = header.num_st;
  unsigned int trianglesCount = header.num_tris;

  auto framesBufferSize = framesCount * header.framesize;
  auto framesBuffer = new char[framesBufferSize];
  fseek(file, header.ofs_frames, SEEK_SET);
  fread(framesBuffer, framesBufferSize, 1, file);

  auto stsBuffer = new char[stsCount * sizeof(texCoord_t)];
  fseek(file, header.ofs_st, SEEK_SET);
  fread(stsBuffer, stsCount * sizeof(texCoord_t), 1, file);

  auto trianglesBuffer = new char[trianglesCount * sizeof(triangle_t)];
  fseek(file, header.ofs_tris, SEEK_SET);
  fread(trianglesBuffer, trianglesCount * sizeof(triangle_t), 1, file);

  fclose(file);

  auto result = std::make_unique<MeshBuilderData>();

  auto* material = new MeshBuilderMaterialData();
  material->name = FileUtils::getFilenameWithoutExtension(filename);
  material->texturePath = material->name;
  material->texturePath.value().append(".png");

  result->materials.push_back(material);
  result->loadNormals = true;
  result->loadLightmap = false;

  Vec4** tempVertices = new Vec4*[framesCount];
  Vec4** tempNormals = new Vec4*[framesCount];
  Vec4** tempTexCoords = new Vec4*[framesCount];

  for (unsigned int i = 0; i < framesCount; i++) {
    auto* outputFrame = new MeshBuilderMaterialFrameData();
    material->frames.push_back(outputFrame);

    tempVertices[i] = new Vec4[vertexCount];
    tempNormals[i] = new Vec4[vertexCount];
    tempTexCoords[i] = new Vec4[stsCount];
  }

  Vec4 temp(0.0F, 0.0F, 0.0F, 1.0F);

  for (unsigned int frameIndex = 0; frameIndex < framesCount; frameIndex++) {
    auto* frame = reinterpret_cast<frame_t*>(
        &framesBuffer[header.framesize * frameIndex]);

    for (unsigned int vertexIndex = 0; vertexIndex < vertexCount;
         vertexIndex++) {
      temp.set(((frame->verts[vertexIndex].v[0] * frame->scale[0]) +
                frame->translate[0]) *
                   options.scale,
               ((frame->verts[vertexIndex].v[1] * frame->scale[1]) +
                frame->translate[1]) *
                   options.scale,
               ((frame->verts[vertexIndex].v[2] * frame->scale[2]) +
                frame->translate[2]) *
                   options.scale);

      tempVertices[frameIndex][vertexIndex].set(temp);

      temp.set(ANORMS[frame->verts[vertexIndex].lightnormalindex][0],
               ANORMS[frame->verts[vertexIndex].lightnormalindex][1],
               ANORMS[frame->verts[vertexIndex].lightnormalindex][2]);

      tempNormals[frameIndex][vertexIndex].set(temp);
    }
  }

  for (unsigned int i = 0; i < stsCount; i++) {
    auto* texCoord =
        reinterpret_cast<texCoord_t*>(&stsBuffer[sizeof(texCoord_t) * i]);

    temp.set(static_cast<float>(texCoord->s) / header.skinwidth,
             static_cast<float>(texCoord->t) / header.skinheight, 1.0F, 0.0F);

    if (options.flipUVs) temp.y = 1.0F - temp.y;

    for (unsigned int j = 0; j < framesCount; j++)
      tempTexCoords[j][i].set(temp);
  }

  for (unsigned int x = 0; x < framesCount; x++) {
    material->frames[x]->count = trianglesCount * 3;
    material->frames[x]->vertices = new Vec4[trianglesCount * 3];
    material->frames[x]->normals = new Vec4[trianglesCount * 3];
    material->frames[x]->textureCoords = new Vec4[trianglesCount * 3];
  }

  triangle_t* triangle;
  for (unsigned int i = 0; i < trianglesCount; i++) {
    triangle =
        reinterpret_cast<triangle_t*>(&trianglesBuffer[sizeof(triangle_t) * i]);

    for (unsigned int x = 0; x < framesCount; x++) {
      auto* workFrame = material->frames[x];

      for (unsigned char j = 0; j < 3; j++) {
        workFrame->vertices[i * 3 + j] =
            tempVertices[x][triangle->index_xyz[j]];

        workFrame->normals[i * 3 + j] = tempNormals[x][triangle->index_xyz[j]];

        workFrame->textureCoords[i * 3 + j] =
            tempTexCoords[x][triangle->index_st[j]];
      }
    }
  }

  for (unsigned int i = 0; i < framesCount; i++) {
    delete[] tempVertices[i];
    delete[] tempNormals[i];
    delete[] tempTexCoords[i];
  }

  delete[] tempVertices;
  delete[] tempNormals;
  delete[] tempTexCoords;

  delete[] framesBuffer;
  delete[] stsBuffer;
  delete[] trianglesBuffer;

  return result;
}

}  // namespace Tyra
