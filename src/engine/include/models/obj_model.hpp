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
#include "utils/debug.hpp"
#include "./anim_state.hpp"
#include "./mesh_material.hpp"

class Frame2
{
public:
    Frame2() { printf("Frame2 constructor\n"); };
    ~Frame2() { printf("Frame2 destructor\n"); };
    u16 number;
    u32 verticesCount, coordinatesCount, normalsCount, materialsCount;
    Vector3 *vertices __attribute__((aligned(16))),
        *coordinates __attribute__((aligned(16))),
        *normals __attribute__((aligned(16)));
    MeshMaterial *materials;
};

struct Frame
{
    u16 number;
    u32 verticesCount, coordinatesCount, normalsCount, materialsCount;
    Vector3 *vertices __attribute__((aligned(16))),
        *coordinates __attribute__((aligned(16))),
        *normals __attribute__((aligned(16)));
    MeshMaterial *materials;
};

/** Class which have common types for all 3D objects */
class ObjModel
{

public:
    /** File name without extension */
    char *filename;
    u16 frameCount;
    Frame *frames;
    AnimState animState;

    ObjModel(char *t_objFile);
    ~ObjModel();
    void animate();
    u32 getDrawData(u32 t_materialIndex, VECTOR *o_vertices, VECTOR *o_normals, VECTOR *o_coordinates, Vector3 &t_cameraPos, float t_scale, u8 t_shouldBeBackfaceCulled);
    u32 getFacesCount();
    u8 isMemoryAllocated;

private:
    Vector3 calc3Vectors[3];
};

#endif
