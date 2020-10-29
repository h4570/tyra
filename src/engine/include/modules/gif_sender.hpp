/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _TYRA_GIF_SENDER_
#define _TYRA_GIF_SENDER_

#include <tamtypes.h>
#include <draw_buffers.h>
#include <draw_types.h>
#include <draw_primitives.h>
#include <math3d.h>
#include <packet.h>
#include "../models/mesh.hpp"
#include "../models/math/matrix.hpp"
#include "../models/screen_settings.hpp"
#include "../models/light_bulb.hpp"
#include "../models/render_data.hpp"
#include "../models/mesh_texture.hpp"

/** Class responsible for sending data packets via GIF (PATH3) */
class GifSender
{

public:
    GifSender(u32 t_packetSize, ScreenSettings *t_screen);
    ~GifSender();

    void initPacket(u8 context);
    void addObjects(RenderData *t_renderData, Mesh **t_objects3D, u32 t_amount, LightBulb *t_bulbs, u16 t_bulbsCount, texbuffer_t *textureBuffer);
    void addClear(zbuffer_t *t_zBuffer);
    void sendPacket();
    void sendClear(zbuffer_t *t_zBuffer);
    static void sendTexture(MeshTexture &texture, texbuffer_t *t_texBuffer);

private:
    xyz_t *xyz;
    color_t *rgbaq;
    texel_t *st;
    u8 isAnyObjectAdded;
    ScreenSettings *screen;
    u64 *dw;
    qword_t *q, *dmatag;
    packet_t *packets[2];
    packet_t *currentPacket;
    u8 packetsCount;
    int packetSize;
    float halfScreenW, halfScreenH;
    MATRIX localWorld, localScreen, localLight;

    u32 calc3DObject(Matrix t_perspective, Mesh &t_mesh, RenderData *t_renderData, LightBulb *t_bulbs, u16 t_bulbsCount);
    void convertCalcs(u32 t_vertCount, VECTOR *t_vertices, VECTOR *t_colors, VECTOR *t_sts, color_t &t_color);
    void addCurrentCalcs(u32 &t_vertexCount);
};

#endif
