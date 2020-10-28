/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _TYRA_OBJ_MODEL_
#define _TYRA_OBJ_MODEL_

#include <tamtypes.h>
#include <math3d.h>
#include "math/vector3.hpp"

struct ObjMaterial
{
    char *materialName;
    u32 facesCount;
    u32 *verticeFaces, *coordinateFaces, *normalFaces;
};

/** Class which have common types for all 3D objects */
class ObjModel
{

public:
    u32 verticesCount, coordinatesCount, normalsCount, materialsCount;
    Vector3 *vertices __attribute__((aligned(16))),
        *coordinates __attribute__((aligned(16))),
        *normals __attribute__((aligned(16)));

    /** File name without extension */
    char *filename;
    ObjMaterial *materials;

    ObjModel(char *t_objFile);
    ~ObjModel();
    u32 getDrawData(u32 t_materialIndex, VECTOR *o_vertices, VECTOR *o_normals, VECTOR *o_coordinates, VECTOR *o_colors, Vector3 &t_cameraPos, float t_scale, u8 t_shouldBeBackfaceCulled);
    u32 getFacesCount();
    u8 isMemoryAllocated;

private:
    void fillNextFace(VECTOR *o_vertices, VECTOR *o_normals, VECTOR *o_coordinates, VECTOR *o_colors, u32 t_materialI, u32 t_getI, u32 t_setI);
};

#endif
