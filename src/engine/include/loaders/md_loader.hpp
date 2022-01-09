/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020 - 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
# André Guilherme <andregui17@outlook.com>
*/

#ifndef TYRA_MD_LOADER_HPP
#define TYRA_MD_LOADER_HPP
#include "../models/mesh_frame.hpp"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <tamtypes.h>
/*
************************************************************************
** .MD2 Triangle model file format Definitions
** Used by Tyra engine.
************************************************************************
*/

// magic number "IDP2" or 844121161
#define MD2_IDENT (('2' << 24) + ('P' << 16) + ('D' << 8) + 'I')
// MD2 model version
#define MD2_VERSION 8

/*
************************************************************************
** .MD3 triangle model file format
** Definitions used by the MD3 loader.
************************************************************************
*/
// magic number "IDP3"
#define MD3_IDENT			(('3'<<24)+('P'<<16)+('D'<<8)+'I')
// MD Version
#define MD3_VERSION			15

// surface geometry should not exceed these limits
#define	SHADER_MAX_VERTEXES	1000
#define	SHADER_MAX_INDEXES	(6*SHADER_MAX_VERTEXES)

// limits
#define MD3_MAX_LODS		4
#define	MD3_MAX_TRIANGLES	8192	// per surface
#define MD3_MAX_VERTS		4096	// per surface
#define MD3_MAX_SHADERS		256		// per surface
#define MD3_MAX_FRAMES		1024	// per model
#define	MD3_MAX_SURFACES	32		// per model
#define MD3_MAX_TAGS		16		// per frame
#define MAX_MD3PATH			64		// from quake3

// vertex scales
#define	MD3_XYZ_SCALE		(1.0/64)

/*
************************************************************************
** .MD2 Triangle model file format whole structure
** Used by Tyra engine.
************************************************************************
*/
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
/*
************************************************************************
** .MD2 Triangle model file format end of the structure
**  Used by Tyra engine.
************************************************************************
*/

/*
************************************************************************
** .MD3 Triangle model file format start of the structure
************************************************************************
*/
typedef struct md3Frame_s 
{
	Vector3		bounds[2];
	Vector3		localOrigin;
	float		radius;
	char		name[16];
} md3Frame_t;

typedef struct md3Tag_s 
{
	char		name[MAX_MD3PATH];	// tag name
	Vector3		origin;
	Vector3		axis[3];
} md3Tag_t;

/*
** md3Surface_t
**
** CHUNK			SIZE
** header			sizeof( md3Surface_t )
** shaders			sizeof( md3Shader_t ) * numShaders
** triangles[0]		sizeof( md3Triangle_t ) * numTriangles
** st				sizeof( md3St_t ) * numVerts
** XyzNormals		sizeof( md3XyzNormal_t ) * numVerts * numFrames
*/

typedef struct md3Surface_s 
{
	int			ident;				// 

	char		name[MAX_MD3PATH];	// polyset name

	int			flags;
	int			numFrames;			// all surfaces in a model should have the same

	int			numShaders;			// all surfaces in a model should have the same
	int			numVerts;

	int			numTriangles;
	int			ofsTriangles;

	int			ofsShaders;			// offset from start of md3Surface_t
	int			ofsSt;				// texture coords are common for all frames
	int			ofsXyzNormals;		// numVerts * numFrames

	int			ofsEnd;				// next surface follows
} md3Surface_t;

typedef struct 
{
	char				name[MAX_MD3PATH];
	const MeshMaterial	*shader;			// for in-game use
} md3Shader_t;

typedef struct 
{
	int			indexes[3];
} md3Triangle_t;

typedef struct 
{
	float		st[2];
} md3St_t;

typedef struct 
{
	short		xyz[3];
	short		normal;
} md3XyzNormal_t;

typedef struct md3Header_s 
{
	int			ident;
	int			version;

	char		name[MAX_MD3PATH];	// model name

	int			flags;

	int			numFrames;
	int			numTags;			
	int			numSurfaces;

	int			numSkins;

	int			ofsFrames;			// offset for first frame
	int			ofsTags;			// numFrames * numTags
	int			ofsSurfaces;		// first surface, others follow

	int			ofsEnd;				// end of file
} md3Header_t;
/*
************************************************************************
** .MD3 Triangle model file format end of the structure
************************************************************************
*/

/** Class responsible for loading&parsing Quake's II ".md2" and Quake III ".md3" 3D files */
class MDLoader
{
public:
    MDLoader();
    ~MDLoader();

    MeshFrame *load_md2(u32 &o_framesCount, char *t_subpath, char *t_nameWithoutExtension, float t_scale, u8 t_invertT);
	MeshFrame *load_md3(u32 &o_framesCount, char *t_subpath, char *t_nameWithoutExtension, float t_scale, u8 t_invertT);
};

#endif //TYRA_MD_LOADER_HPP