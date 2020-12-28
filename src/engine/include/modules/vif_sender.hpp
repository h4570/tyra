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
#include "./light.hpp"
#include "../models/light_bulb.hpp"
#include "../models/mesh.hpp"
#include "../models/math/matrix.hpp"
#include "../models/math/vector3.hpp"

/** Class responsible for sending 3D objects via VIF (PATH 1) */
class VifSender
{

public:
    VifSender(Light *t_light);
    ~VifSender();

    // TODO refactor
    void drawMesh(RenderData *t_renderData, Matrix t_perspective, u32 vertCount2, VECTOR *vertices, VECTOR *normals, VECTOR *coordinates, Mesh &t_mesh, LightBulb *t_bulbs, u16 t_bulbsCount, texbuffer_t *textureBuffer, color_t *t_color, u8 t_rgbaOnly);
    void calcMatrix(const RenderData &t_renderData, const Vector3 &t_position, const Vector3 &t_rotation);
    void drawTheSameWithOtherMatrices(const RenderData &t_renderData, Mesh **t_meshes, const u32 &t_skip, const u32 &t_count);
    void enableWait() { isDrawWaitEnabled = true; }
    void disableWait() { isDrawWaitEnabled = false; }

private:
    u32 lastVertCount; // needed for drawTheSameWithOtherMatrices()
    u8 isLastRGBAOnly; // needed for drawTheSameWithOtherMatrices()
    u8 isDrawWaitEnabled;
    Light *light;
    void uploadMicroProgram();
    void setDoubleBufferAddStaticData();
    void drawVertices(Mesh &t_mesh, u32 t_start, u32 t_end, VECTOR *t_vertices, VECTOR *t_coordinates, prim_t *t_prim, texbuffer_t *t_textureBuffer, u8 t_addDrawWait, color_t *t_color, u8 t_rgbaOnly);
    packet2_t *packets[2] __attribute__((aligned(64)));
    packet2_t *currPacket;
    /** 
     * OpenGL name: Model
     * Sony name: local world 
     */
    Matrix model;
    /** 
     * OpenGL name: ModelViewProjection
     * Sony name: local screen
     */
    Matrix modelViewProj;
    u8 context;
    VECTOR position, rotation;
    u32 vertCount;
};

#endif
