/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include <stdio.h>
#include <string>
#include "debug/debug.hpp"
#include "loaders/3d/builder/mesh_builder_data.hpp"
#include "loaders/3d/md2/anorms.hpp"
#include "loaders/loader.hpp"
#include "loaders/3d/md2/md2_loader.hpp"

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
  s16 index_xyz[3];  // indexes to triangle's vertices
  s16 index_st[3];   // indexes to vertices' texture coorinates
} triangle_t;

typedef struct {
  s16 s;
  s16 t;
} texCoord_t;

MD2Loader::MD2Loader() {}

MD2Loader::~MD2Loader() {}

MeshBuilderData* MD2Loader::load(const char* fullpath, const float& scale,
                                 const bool& invertT) {
  std::string path = fullpath;
  TYRA_ASSERT(!path.empty(), "Provided path is empty!");

  auto filename = getFilenameFromPath(path);

  FILE* file = fopen(fullpath, "rb");
  TYRA_ASSERT(file != NULL, "Failed to load: ", filename);
  md2_t header;

  fread(reinterpret_cast<char*>(&header), sizeof(md2_t), 1, file);

  TYRA_ASSERT((header.ident == MD2_IDENT) && (header.version == MD2_VERSION),
              "This MD2 file is not in correct format!");

  u32 framesCount = header.num_frames;
  u32 vertexCount = header.num_xyz;
  u32 stsCount = header.num_st;
  u32 trianglesCount = header.num_tris;

  auto framesBuffer = new char[framesCount * header.framesize];
  fseek(file, header.ofs_frames, SEEK_SET);
  fread(framesBuffer, framesCount * header.framesize, 1, file);

  auto stsBuffer = new char[stsCount * sizeof(texCoord_t)];
  fseek(file, header.ofs_st, SEEK_SET);
  fread(stsBuffer, stsCount * sizeof(texCoord_t), 1, file);

  auto trianglesBuffer = new char[trianglesCount * sizeof(triangle_t)];
  fseek(file, header.ofs_tris, SEEK_SET);
  fread(trianglesBuffer, trianglesCount * sizeof(triangle_t), 1, file);

  fclose(file);

  auto result = new MeshBuilderData();
  result->allocate(framesCount, 1);
  result->normalsEnabled = true;
  result->textureCoordsEnabled = true;
  result->manyColorsEnabled = false;

  frame_t* frame;
  Vec4 temp(0.0F, 0.0F, 0.0F, 1.0F);
  for (u32 j = 0; j < framesCount; j++) {
    result->frames[j]->allocateVertices(vertexCount);
    result->frames[j]->allocateNormals(vertexCount);
    result->frames[j]->allocateTextureCoords(stsCount);

    result->materials[0]->allocateFaces(trianglesCount * 3);
    result->materials[0]->name = getFilenameWithoutExtension(filename);

    frame = reinterpret_cast<frame_t*>(&framesBuffer[header.framesize * j]);

    for (u32 i = 0; i < vertexCount; i++) {
      temp.set(
          ((frame->verts[i].v[0] * frame->scale[0]) + frame->translate[0]) *
              scale,
          ((frame->verts[i].v[1] * frame->scale[1]) + frame->translate[1]) *
              scale,
          ((frame->verts[i].v[2] * frame->scale[2]) + frame->translate[2]) *
              scale);

      result->frames[j]->vertices[i].set(temp);

      temp.set(ANORMS[frame->verts[i].lightnormalindex][0],
               ANORMS[frame->verts[i].lightnormalindex][1],
               ANORMS[frame->verts[i].lightnormalindex][2]);

      result->frames[j]->normals[i].set(temp);
    }
  }

  texCoord_t* texCoord;

  TYRA_LOG("Skin width: ", header.skinwidth,
           " Skin height: ", header.skinheight);

  for (u32 i = 0; i < stsCount; i++) {
    texCoord =
        reinterpret_cast<texCoord_t*>(&stsBuffer[sizeof(texCoord_t) * i]);

    temp.set(static_cast<float>(texCoord->s) / header.skinwidth,
             static_cast<float>(texCoord->t) / header.skinheight, 1.0F, 0.0F);

    if (invertT) temp.y = 1.0F - temp.y;

    for (u32 j = 0; j < framesCount; j++)
      result->frames[j]->textureCoords[i].set(temp);
  }

  triangle_t* triangle;
  for (u32 i = 0; i < trianglesCount; i++) {
    triangle =
        reinterpret_cast<triangle_t*>(&trianglesBuffer[sizeof(triangle_t) * i]);

    for (u8 j = 0; j < 3; j++) {
      for (u32 x = 0; x < framesCount; x++) {
        result->materials[0]->vertexFaces[(i * 3) + j] = triangle->index_xyz[j];

        result->materials[0]->textureCoordFaces[(i * 3) + j] =
            triangle->index_st[j];

        result->materials[0]->normalFaces[(i * 3) + j] = triangle->index_xyz[j];
      }
    }
  }

  TYRA_LOG("MD2 file \"", filename, "\" loaded!");

  delete[] framesBuffer;
  delete[] stsBuffer;
  delete[] trianglesBuffer;

  return result;
}

}  // namespace Tyra
