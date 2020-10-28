/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _TYRA_DFF_MODEL_
#define _TYRA_DFF_MODEL_

#include <tamtypes.h>
#include <math3d.h>
#include "../loaders/dff_structure.hpp"
#include "./math/vector3.hpp"

/** Class which have common types for all 3D objects */
class DffModel
{

public:
    DffModel();
    ~DffModel();

    u32 getDrawData(u32 splitIndex, VECTOR *o_vertices, VECTOR *o_normals, VECTOR *o_coordinates, VECTOR *o_colors, Vector3 &t_cameraPos, float t_scale, u8 t_shouldBeBackfaceCulled);
    RwClump clump;

private:
    void fillNextFace(VECTOR *o_vertices, VECTOR *o_normals, VECTOR *o_coordinates, VECTOR *o_colors, u8 geometry, u32 t_getI, u32 t_setI);
    u8 isMemoryAllocated;
};

#endif
