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
#include <packet2.h>
#include "gif_sender.hpp"
#include "vif_sender.hpp"
#include "../models/math/plane.hpp"
#include "../models/sprite.hpp"
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
    u8 context; // TODO
    zbuffer_t zBuffer;
    prim_t prim;

    void enableVSync() { isVSyncEnabled = true; }
    void disableVSync() { isVSyncEnabled = false; }

    /// --- Draw: PATH3

    void draw(Sprite &t_sprite);

    /** 
     * Draw many meshes with lighting information.
     * Slowest way of rendering (PATH 3, using EE and GIF). 
     * NOTICE: Animation supported, lighting supported
     */
    void drawByPath3(Mesh *t_meshes, u16 t_amount, LightBulb *t_bulbs, u16 t_bulbsCount);

    /** 
     * Draw mesh with lighting information.
     * Slowest way of rendering (PATH 3, using EE and GIF). 
     * NOTICE: Animation supported, lighting supported
     */
    void drawByPath3(Mesh &t_mesh, LightBulb *t_bulbs, u16 t_bulbsCount);

    /** 
     * Draw many meshes without lighting information.
     * Slowest way of rendering (PATH 3, using EE and GIF). 
     * NOTICE: Animation supported, lighting supported
     */
    void drawByPath3(Mesh *t_meshes, u16 t_amount);

    /** 
     * Draw mesh without lighting information.
     * Slowest way of rendering (PATH 3, using EE and GIF). 
     * NOTICE: Animation supported, lighting supported
     */
    void drawByPath3(Mesh &t_mesh);

    /// --- Draw: PATH1

    /** 
     * Draw many meshes with lighting information.
     * Fastest way of rendering (PATH 1, using VU1). 
     * NOTICE: Animation supported, lighting NOT supported (at this moment)
     */
    void draw(Mesh *t_meshes, u16 t_amount, LightBulb *t_bulbs, u16 t_bulbsCount);

    /** 
     * Draw mesh with lighting information.
     * Fastest way of rendering (PATH 1, using VU1). 
     * NOTICE: Animation supported, lighting NOT supported (at this moment)
     */
    void draw(Mesh &t_mesh, LightBulb *t_bulbs, u16 t_bulbsCount);

    /** 
     * Draw many meshes without lighting information.
     * Fastest way of rendering (PATH 1, using VU1). 
     * NOTICE: Animation supported, lighting NOT supported (at this moment)
     */
    void draw(Mesh *t_meshes, u16 t_amount);

    /** 
     * Draw mesh without lighting information.
     * Fastest way of rendering (PATH 1, using VU1). 
     * NOTICE: Animation supported, lighting NOT supported (at this moment)
     */
    void draw(Mesh &t_mesh);

    void setCameraDefinitions(Matrix *t_worldView, Vector3 *t_cameraPos, Plane *t_planes);

    void endFrame(float fps);

    TextureRepository *getTextureRepository() { return &textureRepo; };

private:
    void changeTexture(Texture *t_tex);
    u32 lastTextureId;
    texbuffer_t textureBuffer;
    u8 isTextureVRAMAllocated, isVSyncEnabled;
    void allocateTextureBuffer(Texture *t_texture);
    void deallocateTextureBuffer();
    void flipBuffers();
    void beginFrameIfNeeded();
    u8 isFrameEmpty;
    Matrix perspective;
    RenderData renderData;
    TextureRepository textureRepo;
    ScreenSettings *screen;
    GifSender *gifSender;
    VifSender *vifSender;
    packet2_t *flipPacket;
    void allocateBuffers(float t_screenW, float t_screenH);
    void initDrawingEnv(float t_screenW, float t_screenH);
    void setPrim();
};

#endif
