/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020 - 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# André Guilherme <andregui17@outlook.com>
*/

#include "../models/mesh_frame.hpp"
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
#define	MAX_QPATH		64
typedef char material[MAX_QPATH];
// vertex scales
#define	MD3_XYZ_SCALE		(1.0/64)

typedef float vec3_t[3];

/*
************************************************************************
** .MD3 Triangle model file format start of the structure
************************************************************************
*/
typedef struct md3Frame_s 
{
	vec3_t	bounds[2];
	vec3_t		localOrigin;
	float		radius;
	char		name[16];
} md3Frame_t;

typedef struct md3Tag_s 
{
	char		name[MAX_MD3PATH];	// tag name
	vec3_t		origin;
	vec3_t		axis[3];
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
	const material	*shader;			// for in-game use
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