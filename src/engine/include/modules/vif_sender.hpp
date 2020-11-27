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
#include <packet2_utils.h>
#include "../models/render_data.hpp"
#include "../models/light_bulb.hpp"
#include "../models/mesh.hpp"
#include "../models/math/matrix.hpp"
#include "../models/math/vector3.hpp"

/** Class responsible for sending 3D objects via VIF (PATH 1) */
class VifSender
{

public:
    VifSender();
    ~VifSender();

    // TODO refactor
    void drawMesh(RenderData *t_renderData, Matrix t_perspective, u32 vertCount2, VECTOR *vertices, VECTOR *normals, VECTOR *coordinates, Mesh &t_mesh, LightBulb *t_bulbs, u16 t_bulbsCount, texbuffer_t *textureBuffer);
    void sendMatrices(const RenderData &t_renderData, const Vector3 &t_position, const Vector3 &t_rotation);

private:
    void uploadMicroProgram();
    void setDoubleBuffer();
    void drawVertices(Mesh &t_mesh, u32 t_start, u32 t_end, VECTOR *t_vertices, VECTOR *t_coordinates, prim_t *t_prim, texbuffer_t *textureBuffer);
    packet2_t *packets[2] __attribute__((aligned(64)));
    packet2_t *currPacket;
    u8 context;
    packet2_t *matricesPacket __attribute__((aligned(64)));
    MATRIX localWorld, localScreen;
    VECTOR position, rotation;
    u32 vertCount;
};

#endif
