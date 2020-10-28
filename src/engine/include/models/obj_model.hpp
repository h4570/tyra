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

/** Class which have common types for all 3D objects */
class ObjModel
{

public:
    u32 verticesCount, coordinatesCount, normalsCount, facesCount;
    Vector3 *vertices __attribute__((aligned(16))),
        *coordinates __attribute__((aligned(16))),
        *normals __attribute__((aligned(16)));
    u32 *verticeFaces, *coordinateFaces, *normalFaces;

    /** File name without extension */
    char *filename;

    ObjModel(char *t_objFile);
    ~ObjModel();
    u32 getDrawData(VECTOR *o_vertices, VECTOR *o_normals, VECTOR *o_coordinates, VECTOR *o_colors, Vector3 &t_cameraPos, float t_scale, u8 t_shouldBeBackfaceCulled);
    void allocateMemory();

private:
    void fillNextFace(VECTOR *o_vertices, VECTOR *o_normals, VECTOR *o_coordinates, VECTOR *o_colors, u32 t_getI, u32 t_setI);
    u8 isMemoryAllocated;
};

#endif
