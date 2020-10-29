/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _TYRA_VIF_SENDER_
#define _TYRA_VIF_SENDER_

#include <tamtypes.h>
#include "../models/render_data.hpp"
#include "../models/light_bulb.hpp"
#include "../models/mesh.hpp"
#include "../models/math/matrix.hpp"
#include "../models/math/vector3.hpp"
#include "vu1.hpp"

/** Class responsible for sending 3D objects via VIF (PATH 1) */
class VifSender
{

public:
    VifSender();
    ~VifSender();

    // TODO refactor
    void drawMesh(RenderData *t_renderData, Matrix t_perspective, u32 vertCount2, VECTOR *vertices, VECTOR *normals, VECTOR *coordinates, Mesh *t_mesh, LightBulb *t_bulbs, u16 t_bulbsCount);

private:
    void drawVertices(Mesh *t_mesh, u32 t_start, u32 t_end, VECTOR *t_vertices, VECTOR *t_coordinates, prim_t *t_prim);

    MATRIX localWorld, localScreen;
    VECTOR position, rotation;
    u32 vertCount;
    VU1 vu1;
};

#endif
