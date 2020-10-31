/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "../include/modules/renderer.hpp"

#include <dma.h>
#include <graph.h>
#include <packet.h>
#include <draw.h>
#include <gs_psm.h>
#include "../include/utils/debug.hpp"
#include "../include/utils/math.hpp"

// ----
// Constructors/Destructors
// ----

/** Initialize DMA<->GIF channel
 * Allocate buffers
 * Initialize screen
 * Initialize drawing environment
 * Load/setup textures
 * @param screenW Half of screen width
 * @param screenH Half of screen height
 */
Renderer::Renderer(u32 t_packetSize, ScreenSettings *t_screen)
{
    PRINT_LOG("Initializing renderer");
    dma_channel_initialize(DMA_CHANNEL_GIF, NULL, 0); // Initialize DMA to enable data transfer
    dma_channel_fast_waits(DMA_CHANNEL_GIF);
    context = 0;
    isTextureVRAMAllocated = false;
    isFrameEmpty = false;
    lastTextureId = 0;
    flipPacket = packet_init(3, PACKET_UCAB); // Uncached accelerated
    allocateBuffers(t_screen->width, t_screen->height);
    initDrawingEnv(t_screen->width, t_screen->height);
    setPrim();
    gifSender = new GifSender(t_packetSize, t_screen);
    vifSender = new VifSender();
    perspective.setPerspective(*t_screen);
    renderData.perspective = &perspective;
    PRINT_LOG("Renderer initialized!");
}

Renderer::~Renderer() {}

// ----
// Methods
// ----

/** Configure and allocate vRAM for texture buffer */
void Renderer::allocateTextureBuffer(u16 t_width, u16 t_height)
{
    textureBuffer.width = t_width;
    textureBuffer.psm = GS_PSM_24;
    textureBuffer.address = graph_vram_allocate(t_width, t_height, GS_PSM_24, GRAPH_ALIGN_BLOCK);
    if (textureBuffer.address <= 1)
        PRINT_ERR("Texture buffer allocation error. No memory!");
    textureBuffer.info.width = draw_log2(t_width);
    textureBuffer.info.height = draw_log2(t_height);
    textureBuffer.info.components = TEXTURE_COMPONENTS_RGB;
    textureBuffer.info.function = TEXTURE_FUNCTION_MODULATE;
    isTextureVRAMAllocated = true;
}

/** Configure and allocate vRAM for texture buffer */
void Renderer::deallocateTextureBuffer()
{
    if (isTextureVRAMAllocated)
    {
        graph_vram_free(textureBuffer.address);
        isTextureVRAMAllocated = false;
    }
}

void Renderer::changeTexture(Mesh *t_mesh, u32 t_materialId)
{

    MeshTexture *tex = textureRepo.getByMesh(t_mesh->id, t_materialId);
    if (tex != NULL)
    {
        if (tex->getId() != lastTextureId)
        {
            lastTextureId = tex->getId();
            deallocateTextureBuffer();
            allocateTextureBuffer(tex->getWidth(), tex->getHeight());
            GifSender::sendTexture(*tex, &textureBuffer);
        }
    }
    else
        PRINT_ERR("Texture was not found in texture repository!");
}

/** Initializes drawing environment (1st app packet) */
void Renderer::initDrawingEnv(float t_screenW, float t_screenH)
{
    PRINT_LOG("Initializing drawing environment");
    packet_t *packet = packet_init(20, PACKET_NORMAL);
    u16 halfW = (u16)t_screenW / 2;
    u16 halfH = (u16)t_screenH / 2;
    qword_t *q = packet->data; // Generic qword pointer.
    q = draw_setup_environment(q, 0, frameBuffers, &(zBuffer));
    q = draw_primitive_xyoffset(q, 0, (2048 - halfW), (2048 - halfH));
    q = draw_finish(q);
    // Now send the packet, no need to wait since it's the first.
    dma_channel_send_normal(DMA_CHANNEL_GIF, packet->data, q - packet->data, 0, 0);
    dma_wait_fast();
    packet_free(packet);
    PRINT_LOG("Drawing environment initialized!");
}

/** Sets drawing prim for all 3D objects */
void Renderer::setPrim()
{
    prim.type = PRIM_TRIANGLE;
    prim.shading = PRIM_SHADE_FLAT;
    prim.mapping = DRAW_ENABLE;
    prim.fogging = DRAW_DISABLE;
    prim.blending = DRAW_ENABLE;
    prim.antialiasing = DRAW_DISABLE;
    prim.mapping_type = PRIM_MAP_ST;
    prim.colorfix = PRIM_UNFIXED;
    renderData.prim = &prim;
    PRINT_LOG("Prim set!");
}

/** Defines and allocates framebuffers and zbuffer */
void Renderer::allocateBuffers(float t_screenW, float t_screenH)
{
    frameBuffers[0].width = (u16)t_screenW;
    frameBuffers[0].height = (u16)t_screenH;
    frameBuffers[0].mask = 0;
    frameBuffers[0].psm = GS_PSM_32;
    frameBuffers[0].address = graph_vram_allocate((u16)t_screenW, (u16)t_screenH, frameBuffers[0].psm, GRAPH_ALIGN_PAGE);

    frameBuffers[1].width = (u16)t_screenW;
    frameBuffers[1].height = (u16)t_screenH;
    frameBuffers[1].mask = 0;
    frameBuffers[1].psm = GS_PSM_32;
    frameBuffers[1].address = graph_vram_allocate((u16)t_screenW, (u16)t_screenH, frameBuffers[1].psm, GRAPH_ALIGN_PAGE);

    zBuffer.enable = DRAW_ENABLE;
    zBuffer.mask = 0;
    zBuffer.method = ZTEST_METHOD_GREATER_EQUAL;
    zBuffer.zsm = GS_ZBUF_32;
    zBuffer.address = graph_vram_allocate((u16)t_screenW, (u16)t_screenH, zBuffer.zsm, GRAPH_ALIGN_PAGE);
    PRINT_LOG("Framebuffers, zBuffer set and allocated!");

    // Initialize the screen and tie the first framebuffer to the read circuits.
    graph_initialize(frameBuffers[1].address, frameBuffers[1].width, frameBuffers[1].height, frameBuffers[1].psm, 0, 0);
}

/// --- Draw: PATH3

/** PATH3 Many + lighting */
void Renderer::drawByPath3(Mesh **t_meshes, u16 t_amount, LightBulb *t_bulbs, u16 t_bulbsCount)
{
    beginFrameIfNeeded();
    gifSender->initPacket(context);
    // TODO
    changeTexture(t_meshes[0], 0);
    gifSender->addObjects(&renderData, t_meshes, t_amount, t_bulbs, t_bulbsCount, &textureBuffer);
    gifSender->sendPacket();
    draw_wait_finish();
}

/** PATH3 Single + lighting */
void Renderer::drawByPath3(Mesh *t_mesh, LightBulb *t_bulbs, u16 t_bulbsCount)
{
    beginFrameIfNeeded();
    changeTexture(t_mesh, 0);
    gifSender->initPacket(context);
    // TODO
    gifSender->addObjects(&renderData, &t_mesh, 1, t_bulbs, t_bulbsCount, &textureBuffer);
    gifSender->sendPacket();
    draw_wait_finish();
}

/** PATH3 Many */
void Renderer::drawByPath3(Mesh **t_meshes, u16 t_amount) { drawByPath3(t_meshes, t_amount, NULL, 0); }

/** PATH3 Single */
void Renderer::drawByPath3(Mesh *t_mesh) { drawByPath3(t_mesh, NULL, 0); }

/// --- Draw: PATH1

/** PATH1 Many + lighting */
void Renderer::draw(Mesh **t_meshes, u16 t_amount, LightBulb *t_bulbs, u16 t_bulbsCount)
{
    // TODO
    beginFrameIfNeeded();
    for (u16 i = 0; i < t_amount; i++)
        draw(t_meshes[i], t_bulbs, t_bulbsCount);
}

/** PATH1 Single + lighting */
void Renderer::draw(Mesh *t_mesh, LightBulb *t_bulbs, u16 t_bulbsCount)
{
    beginFrameIfNeeded();
    // TODO VU1 send single list here
    if (!t_mesh->isObjLoaded && !t_mesh->isDffLoaded && !t_mesh->isMd2Loaded)
        return;
    u32 vertCount = t_mesh->getVertexCount();
    VECTOR *vertices = new VECTOR[vertCount];
    VECTOR *normals = new VECTOR[vertCount];
    VECTOR *coordinates = new VECTOR[vertCount];
    if (t_mesh->isObjLoaded)
    {
        t_mesh->obj->animate();
        for (u32 i = 0; i < t_mesh->getMaterialsCount(); i++)
        {
            changeTexture(t_mesh, t_mesh->getMaterial(i).getId());
            vertCount = t_mesh->getDrawData(i, vertices, normals, coordinates, *renderData.cameraPosition);
            vifSender->drawMesh(&renderData, perspective, vertCount, vertices, normals, coordinates, t_mesh, t_bulbs, t_bulbsCount, &textureBuffer);
        }
    }
    else if (t_mesh->isMd2Loaded)
    {
        changeTexture(t_mesh, 0);
        vertCount = t_mesh->getDrawData(0, vertices, normals, coordinates, *renderData.cameraPosition);
        vifSender->drawMesh(&renderData, perspective, vertCount, vertices, normals, coordinates, t_mesh, t_bulbs, t_bulbsCount, &textureBuffer);
    }
    else if (t_mesh->isDffLoaded)
        for (u32 i = 0; i < t_mesh->dff->clump.geometryList.geometries[0].extension.materialSplit.header.splitCount; i++)
        {
            const u32 currentTexI = t_mesh->dff->clump.geometryList.geometries[0].extension.materialSplit.splitInformation[i].materialIndex;
            changeTexture(t_mesh, currentTexI);
            vertCount = t_mesh->getDrawData(i, vertices, normals, coordinates, *renderData.cameraPosition);
            vifSender->drawMesh(&renderData, perspective, vertCount, vertices, normals, coordinates, t_mesh, t_bulbs, t_bulbsCount, &textureBuffer);
        }
    delete[] vertices;
    delete[] normals;
    delete[] coordinates;
}

/** PATH1 Many */
void Renderer::draw(Mesh **t_objects3D, u16 t_amount) { draw(t_objects3D, t_amount, NULL, 0); }

/** PATH1 Single */
void Renderer::draw(Mesh *t_mesh) { draw(t_mesh, NULL, 0); }

/// ---

void Renderer::setCameraDefinitions(Matrix *t_worldView, Vector3 *t_cameraPos, Plane *t_planes)
{
    renderData.worldView = t_worldView;
    renderData.cameraPosition = t_cameraPos;
    renderData.frustumPlanes = t_planes;
}

void Renderer::beginFrameIfNeeded()
{
    if (isFrameEmpty)
    {
        isFrameEmpty = false;
        gifSender->sendClear(&zBuffer);
    }
}

void Renderer::endFrame(float fps)
{
    if (!isFrameEmpty)
    {
        if (fps > 49.0F)
            graph_wait_vsync();
        flipBuffers();
    }
}

/** We need to flip buffers outside of the chain, for some reason,
 * so we use a separate small packet
 * Do not use this method. This is called via packetManager
 */
void Renderer::flipBuffers()
{
    graph_set_framebuffer_filtered(
        frameBuffers[context].address,
        frameBuffers[context].width,
        frameBuffers[context].psm,
        0,
        0);
    context ^= 1;
    isFrameEmpty = 1;
    qword_t *q = flipPacket->data;
    q = draw_framebuffer(q, 0, &frameBuffers[context]);
    q = draw_finish(q);
    dma_wait_fast();
    dma_channel_send_normal_ucab(DMA_CHANNEL_GIF, flipPacket->data, q - flipPacket->data, 0);
    draw_wait_finish();
}
