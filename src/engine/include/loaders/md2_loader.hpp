/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _TYRA_MD2_LOADER_
#define _TYRA_MD2_LOADER_

#include "../models/mesh_frame.hpp"
#include <stdio.h>

// magic number "IDP2" or 844121161
#define MD2_IDENT (('2' << 24) + ('P' << 16) + ('D' << 8) + 'I')

// model version
#define MD2_VERSION 8

typedef struct
{
    int ident;   // magic number. must be equal to "IDP2"
    int version; // md2 version. must be equal to 8

    int skinwidth;  // width of the texture
    int skinheight; // height of the texture
    int framesize;  // size of one frame in bytes

    int num_skins;  // number of textures
    int num_xyz;    // number of vertices
    int num_st;     // number of texture coordinates
    int num_tris;   // number of triangles
    int num_glcmds; // number of opengl commands
    int num_frames; // total number of frames

    int ofs_skins;  // offset to skin names (64 bytes each)
    int ofs_st;     // offset to s-t texture coordinates
    int ofs_tris;   // offset to triangles
    int ofs_frames; // offset to frame data
    int ofs_glcmds; // offset to opengl commands
    int ofs_end;    // offset to end of file

} md2_t;

typedef struct
{
    unsigned char v[3];             // compressed vertex (x, y, z) coordinates
    unsigned char lightnormalindex; // index to a normal vector for the lighting

} mvertex_t;

typedef struct
{
    float scale[3];     // scale values
    float translate[3]; // translation vector
    char name[16];      // frame name
    mvertex_t verts[1]; // first vertex of this frame

} frame_t;

typedef float vec3_t[3];

typedef struct
{
    short index_xyz[3]; // indexes to triangle's vertices
    short index_st[3];  // indexes to vertices' texture coorinates
} triangle_t;

typedef struct
{
    short s;
    short t;
} texCoord_t;

/** Class responsible for loading&parsing Quake's II ".md2" 3D files */
class MD2Loader
{

public:
    MD2Loader();
    ~MD2Loader();

    MeshFrame *load(u32 &o_framesCount, char *t_subpath, char *t_nameWithoutExtension, float t_scale, u8 t_invertT);

private:
};

#endif
