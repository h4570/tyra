/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "../include/modules/gif_sender.hpp"

#include "../include/utils/math.hpp"
#include "../include/utils/debug.hpp"
#include "../include/modules/light.hpp"
#include <dma.h>
#include <draw.h>
#include <stdio.h>
#include <malloc.h>
#include <dma_tags.h>
#include <gs_psm.h>

// ----
// Constructors/Destructors
// ----

/** Initializes vars and creates data transfer packets
 * @param packetSize Size of data packet, should be increased when more data will be rendered
 */
GifSender::GifSender(u32 t_packetSize, ScreenSettings *t_screen) : screen(t_screen)
{
    PRINT_LOG("Initializing GifSender");
    packetSize = t_packetSize;
    packets[0] = packet_init(t_packetSize, PACKET_NORMAL);
    packets[1] = packet_init(t_packetSize, PACKET_NORMAL);
    PRINT_LOG("GifSender initialized!");
}

/** Releases packets memory */
GifSender::~GifSender()
{
    packet_free(packets[0]);
    packet_free(packets[1]);
}

// ----
// Methods
// ----

#include <gif_tags.h>
#include <gs_gp.h>

/** Send texture via GIF */
void GifSender::sendTexture(MeshTexture &texture, texbuffer_t *t_texBuffer)
{
    const u16 packetSize = 40;
    packet_t *packet = packet_init(packetSize, PACKET_NORMAL);
    qword_t *q = packet->data;
    q = draw_texture_transfer(q, texture.getData(), texture.getWidth(), texture.getHeight(), GS_PSM_24, t_texBuffer->address, t_texBuffer->width);
    DMATAG_CNT(q, 2, 0, 0, 0);
    q++;
    q = draw_texture_wrapping(q, 0, texture.getWrapSettings());
    q = draw_texture_flush(q);
    dma_channel_send_chain(DMA_CHANNEL_GIF, packet->data, q - packet->data, 0, 0);
    dma_wait_fast();
    packet_free(packet);
}

void GifSender::sendClear(zbuffer_t *t_zBuffer)
{
    packet_t *packet = packet_init(100, PACKET_NORMAL);
    qword_t *q = packet->data;
    q++;
    q = draw_disable_tests(q, 0, t_zBuffer);
    q = draw_clear(q, 0,
                   2048.0F - (screen->width / 2), 2048.0F - (screen->height / 2),
                   screen->width, screen->height,
                   0x10, 0x10, 0x10);
    q = draw_enable_tests(q, 0, t_zBuffer);
    q = draw_finish(q);
    DMATAG_END(packet->data, q - packet->data - 1, 0, 0, 0);
    dma_channel_send_chain(DMA_CHANNEL_GIF, packet->data, q - packet->data, 0, 0);
    dma_wait_fast();
    packet_free(packet);
}

/** Used in game loop.
 * Switches current packet to next one
 */
void GifSender::initPacket(u8 t_context)
{
    currentPacket = packets[t_context];
    if (currentPacket->qwords > (u32)packetSize || currentPacket->qwords < 0)
        PRINT_ERR("GifSender packet size error. Please consider to change packet size!\n");
    q = currentPacket->data;
    dmatag = q;
    q++;
    isAnyObjectAdded = 0;
}

/** Sends packet to GIF. */
void GifSender::sendPacket()
{
    if (isAnyObjectAdded)
    {
        dmatag = q;
        q++;
    }
    q = draw_finish(q);
    DMATAG_END(dmatag, q - dmatag - 1, 0, 0, 0);
    dma_wait_fast();
    dma_channel_send_chain(DMA_CHANNEL_GIF, currentPacket->data, q - currentPacket->data, 0, 0);
}

/** Adds clear screen to current packet */
void GifSender::addClear(zbuffer_t *t_zBuffer)
{
    q = draw_disable_tests(q, 0, t_zBuffer);
    q = draw_clear(q, 0,
                   2048.0F - (screen->width / 2), 2048.0F - (screen->height / 2),
                   screen->width, screen->height,
                   0x10, 0x10, 0x10);
    q = draw_enable_tests(q, 0, t_zBuffer);
}

/** Adds 3D objects to current packet
 * @param worldView Matrix
 * @param perspective Matrix with .setPerpsective() used [clone of gluPerspective]
 * @param objects3D Array of 3D objects pointers
 * @param amount Amount of 3D objects
 */
void GifSender::addObjects(RenderData *t_renderData, Mesh **t_objects3D, u32 t_amount, LightBulb *t_bulbs, u16 t_bulbsCount, texbuffer_t *textureBuffer)
{
    if (!isAnyObjectAdded)
    {
        isAnyObjectAdded = true;
        DMATAG_CNT(dmatag, q - dmatag - 1, 0, 0, 0); // init tag (before first 3d obj)
    }
    qword_t *tempDMATag;
    tempDMATag = q;
    q++;

    q = draw_texture_sampling(q, 0, &t_objects3D[0]->lod);
    q = draw_texturebuffer(q, 0, textureBuffer, &t_objects3D[0]->clut);
    dw = (u64 *)draw_prim_start(q, 0, t_renderData->prim, &t_objects3D[0]->color);

    for (u32 i = 0; i < t_amount; i++)
        if (t_objects3D[i]->shouldBeFrustumCulled == 0 || t_objects3D[i]->isInFrustum(t_renderData->frustumPlanes))
        {
            u32 vertexCount = calc3DObject(*t_renderData->perspective, *t_objects3D[i], t_renderData, t_bulbs, t_bulbsCount);
            addCurrentCalcs(vertexCount);
            delete[] xyz;
            delete[] rgbaq;
            delete[] st;
        }

    if ((u32)dw % 16) // if we are in the middle of qw, switch packet
        *dw++ = 0;

    q = draw_prim_end((qword_t *)dw, 3, DRAW_STQ_REGLIST);

    DMATAG_CNT(tempDMATag, q - tempDMATag - 1, 0, 0, 0);
}

/** Calculates 3D object data into xyz, rgbq, st
 * After it addCurrentSTQ() can be done
 * @param worldView Matrix
 * @param perspective Matrix with .setPerpsective() used [clone of gluPerspective]
 * @param mesh 3D object
 * @returns Vertex count
 */
u32 GifSender::calc3DObject(Matrix t_perspective, Mesh &t_mesh, RenderData *t_renderData, LightBulb *t_bulbs, u16 t_bulbsCount)
{
    u32 vertexCount = t_mesh.getVertexCount();

    VECTOR *vertices = new VECTOR[vertexCount];
    VECTOR *normals = new VECTOR[vertexCount];
    VECTOR *coordinates = new VECTOR[vertexCount];
    VECTOR *colors = new VECTOR[vertexCount];
    vertexCount = t_mesh.getDrawData(0, vertices, normals, coordinates, *t_renderData->cameraPosition);

    xyz = new xyz_t[vertexCount];
    rgbaq = new color_t[vertexCount];
    st = new texel_t[vertexCount];

    VECTOR position, rotation;

    vec3ToNative(position, t_mesh.position, 1.0F);
    vec3ToNative(rotation, t_mesh.rotation, 1.0F);

    create_local_world(localWorld, position, rotation);

    const u8 SHOULD_BE_LIGHTED = t_bulbs != NULL && t_mesh.shouldBeLighted;

    if (SHOULD_BE_LIGHTED)
        create_local_light(localLight, rotation);

    // I cant put perspective from renderData here. ee-gcc bug?
    create_local_screen(localScreen, localWorld, t_renderData->worldView->data, t_perspective.data);

    if (SHOULD_BE_LIGHTED)
    {
        const u16 lightsCount = Light::getLightsCount(t_bulbsCount);
        VECTOR *lightDirections = new VECTOR[lightsCount];
        VECTOR *lightColors = new VECTOR[lightsCount];
        int *lightTypes = new int[lightsCount];

        VECTOR *lights = new VECTOR[vertexCount];
        calculate_normals(normals, vertexCount, normals, localLight);

        Light::calculateLight(lightDirections, lightColors, lightTypes, t_bulbs, t_bulbsCount, t_mesh.position);

        calculate_lights(lights, vertexCount, normals, lightDirections, lightColors, lightTypes, lightsCount);

        for (u32 i = 0; i < vertexCount; i++)
        {
            // Apply the light value to the colour.
            colors[i][0] = (t_mesh.color.r * lights[i][0]);
            colors[i][1] = (t_mesh.color.g * lights[i][1]);
            colors[i][2] = (t_mesh.color.b * lights[i][2]);
            vector_clamp(colors[i], colors[i], 0.00f, 1.99f);
        }

        delete[] lightDirections;
        delete[] lightColors;
        delete[] lightTypes;
        delete[] lights;
    }
    else
        for (u32 i = 0; i < vertexCount; i++)
        {
            colors[i][0] = t_mesh.color.r / 128.0F;
            colors[i][1] = t_mesh.color.g / 128.0F;
            colors[i][2] = t_mesh.color.b / 128.0F;
        }

    calculate_vertices(vertices, vertexCount, vertices, localScreen);

    convertCalcs(vertexCount, vertices, colors, coordinates, t_mesh.color);

    delete[] vertices;
    delete[] normals;
    delete[] colors;
    delete[] coordinates;

    return vertexCount;
}

void GifSender::convertCalcs(u32 t_vertexCount, VECTOR *t_vertices, VECTOR *t_colors, VECTOR *t_sts, color_t &t_color)
{
    // TODO get this via screensettings
    const s32 centerX = ftoi4(2048);
    const s32 centerY = ftoi4(2048);
    const u32 maxZ = ftoi4(((float)0xFFFFFF) / 32.0F);
    float q = 1.00F;
    for (u32 i = 0; i < t_vertexCount; i++)
    {
        xyz[i].x = (u16)((t_vertices[i][0] + 1.0F) * centerX);
        xyz[i].y = (u16)((t_vertices[i][1] + 1.0F) * centerY);
        xyz[i].z = (u32)((t_vertices[i][2] + 1.0F) * maxZ);

        if (t_vertices[i][3])
            q = 1 / t_vertices[i][3];

        st[i].s = t_sts[i][0] * q;
        st[i].t = t_sts[i][1] * q;

        rgbaq[i].r = (u8)(t_colors[i][0] * 128.0F);
        rgbaq[i].g = (u8)(t_colors[i][1] * 128.0F);
        rgbaq[i].b = (u8)(t_colors[i][2] * 128.0F);
        rgbaq[i].a = t_color.a;
        rgbaq[i].q = q;
    }
}

/** Update q's double words. It may look strange, but this workaround
 * use's a 64-bit pointer to simplify adding data to the packet.
*/
void GifSender::addCurrentCalcs(u32 &t_vertexCount)
{
    for (u32 i = 0; i < t_vertexCount; i++)
    {
        *dw++ = rgbaq[i].rgbaq;
        *dw++ = st[i].uv;
        *dw++ = xyz[i].xyz;
    }
}
