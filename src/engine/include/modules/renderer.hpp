/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _TYRA_RENDERER_
#define _TYRA_RENDERER_

#include <draw_buffers.h>
#include <draw_primitives.h>
#include <packet.h>
#include "gif_sender.hpp"
#include "vif_sender.hpp"
#include "../models/math/plane.hpp"
#include "../models/screen_settings.hpp"
#include "../models/render_data.hpp"
#include "./texture_repository.hpp"

/** Class responsible for intializing draw env, textures and buffers */
class Renderer
{

public:
    Renderer(u32 t_packetSize, ScreenSettings *t_screen);
    ~Renderer();

    framebuffer_t frameBuffers[2];
    u8 context;
    zbuffer_t zBuffer;
    prim_t prim;

    /** PATH3 Many + lighting */
    void drawByPath3(Mesh **t_meshes, u16 t_amount, LightBulb *t_bulbs, u16 t_bulbsCount);
    /** PATH3 Single + lighting */
    void drawByPath3(Mesh *t_mesh, LightBulb *t_bulbs, u16 t_bulbsCount);
    /** PATH3 Many */
    void drawByPath3(Mesh **t_meshes, u16 t_amount);
    /** PATH3 Single */
    void drawByPath3(Mesh *t_mesh);

    /** PATH1 Many + lighting */
    void draw(Mesh **t_meshes, u16 t_amount, LightBulb *t_bulbs, u16 t_bulbsCount);
    /** PATH1 Single + lighting */
    void draw(Mesh *t_mesh, LightBulb *t_bulbs, u16 t_bulbsCount);
    /** PATH1 Many */
    void draw(Mesh **t_meshes, u16 t_amount);
    /** PATH1 Single */
    void draw(Mesh *t_mesh);

    void setCameraDefinitions(Matrix *t_worldView, Vector3 *t_cameraPos, Plane *t_planes);

    void endFrame(float fps);

    TextureRepository *getTextureRepository() { return &textureRepo; };

private:
    void changeTexture(Mesh *t_mesh, u8 t_textureIndex);
    u32 lastTextureId;
    texbuffer_t textureBuffer;
    u8 isTextureVRAMAllocated;
    void allocateTextureBuffer(u16 t_width, u16 t_height);
    void deallocateTextureBuffer();
    void flipBuffers();
    void beginFrameIfNeeded();
    u8 isFrameEmpty;
    Matrix perspective;
    RenderData renderData;
    TextureRepository textureRepo;
    GifSender *gifSender;
    VifSender *vifSender;
    packet_t *flipPacket;
    void allocateBuffers(float t_screenW, float t_screenH);
    void initDrawingEnv(float t_screenW, float t_screenH);
    void setPrim();
};

#endif
