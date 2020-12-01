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
#include <packet2_chain.h>
#include <kernel.h>
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
    packets[0] = packet2_create(t_packetSize, P2_TYPE_NORMAL, P2_MODE_CHAIN, false);
    packets[1] = packet2_create(t_packetSize, P2_TYPE_NORMAL, P2_MODE_CHAIN, false);
    PRINT_LOG("GifSender initialized!");
}

/** Releases packets memory */
GifSender::~GifSender()
{
    packet2_free(packets[0]);
    packet2_free(packets[1]);
}

// ----
// Methods
// ----

#include <gif_tags.h>
#include <gs_gp.h>

/** Send texture via GIF */
void GifSender::sendTexture(Texture &texture, texbuffer_t *t_texBuffer)
{
    packet2_t *packet2 = packet2_create(15, P2_TYPE_NORMAL, P2_MODE_CHAIN, false);
    packet2_update(
        packet2,
        draw_texture_transfer(
            packet2->base,
            texture.getData(),
            texture.getWidth(),
            texture.getHeight(),
            texture.getType(),
            t_texBuffer->address,
            t_texBuffer->width));
    packet2_chain_open_cnt(packet2, 0, 0, 0);
    packet2_update(packet2, draw_texture_wrapping(packet2->next, 0, texture.getWrapSettings()));
    packet2_chain_close_tag(packet2);
    packet2_update(packet2, draw_texture_flush(packet2->next));
    dma_channel_wait(DMA_CHANNEL_GIF, 0);
    dma_channel_send_packet2(packet2, DMA_CHANNEL_GIF, true);
    dma_channel_wait(DMA_CHANNEL_GIF, 0);
    packet2_free(packet2);
}

void GifSender::sendClear(zbuffer_t *t_zBuffer)
{
    packet2_t *packet2 = packet2_create(36, P2_TYPE_NORMAL, P2_MODE_CHAIN, false);
    packet2_chain_open_end(packet2, 0, 0);
    packet2_update(packet2, draw_disable_tests(packet2->next, 0, t_zBuffer));
    packet2_update(packet2, draw_clear(packet2->next, 0,
                                       2048.0F - (screen->width / 2), 2048.0F - (screen->height / 2),
                                       screen->width, screen->height,
                                       0x10, 0x10, 0x10));
    packet2_update(packet2, draw_enable_tests(packet2->next, 0, t_zBuffer));
    packet2_update(packet2, draw_finish(packet2->next));
    packet2_chain_close_tag(packet2);
    dma_channel_wait(DMA_CHANNEL_GIF, 0);
    dma_channel_send_packet2(packet2, DMA_CHANNEL_GIF, true);
    dma_channel_wait(DMA_CHANNEL_GIF, 0);
    packet2_free(packet2);
}

/** Used in game loop.
 * Switches current packet to next one
 */
void GifSender::initPacket(u8 t_context)
{
    currentPacket = packets[t_context];
    packet2_reset(currentPacket, false);
    isAnyObjectAdded = false;
}

/** Sends packet to GIF. */
void GifSender::sendPacket()
{
    if (isAnyObjectAdded)
    {
        packet2_chain_open_end(currentPacket, 0, 0);
        packet2_update(currentPacket, draw_finish(currentPacket->next));
        packet2_chain_close_tag(currentPacket);
    }
    dma_channel_wait(DMA_CHANNEL_GIF, 0);
    dma_channel_send_packet2(currentPacket, DMA_CHANNEL_GIF, true);
}

/** Adds clear screen to current packet */
void GifSender::addClear(zbuffer_t *t_zBuffer)
{
    packet2_chain_open_cnt(currentPacket, 0, 0, 0);
    packet2_update(currentPacket, draw_disable_tests(currentPacket->next, 0, t_zBuffer));
    packet2_update(currentPacket, draw_clear(currentPacket->next, 0,
                                             2048.0F - (screen->width / 2), 2048.0F - (screen->height / 2),
                                             screen->width, screen->height,
                                             0x10, 0x10, 0x10));
    packet2_update(currentPacket, draw_enable_tests(currentPacket->next, 0, t_zBuffer));
    packet2_chain_close_tag(currentPacket);
}

/** Adds 3D objects to current packet
 * @param worldView Matrix
 * @param perspective Matrix with .setPerpsective() used [clone of gluPerspective]
 * @param objects3D Array of 3D objects pointers
 * @param amount Amount of 3D objects
 */
void GifSender::addObject(RenderData *t_renderData, Mesh &t_mesh, u32 vertexCount, VECTOR *vertices, VECTOR *normals, VECTOR *coordinates, LightBulb *t_bulbs, u16 t_bulbsCount, texbuffer_t *textureBuffer)
{
    packet2_chain_open_cnt(currentPacket, 0, 0, 0);
    packet2_update(currentPacket, draw_texture_sampling(currentPacket->next, 0, &t_mesh.lod));
    packet2_update(currentPacket, draw_texturebuffer(currentPacket->next, 0, textureBuffer, &t_mesh.clut));
    packet2_update(currentPacket, draw_prim_start(currentPacket->next, 0, t_renderData->prim, &t_mesh.color));
    xyz = new xyz_t[vertexCount];
    rgbaq = new color_t[vertexCount];
    st = new texel_t[vertexCount];
    calc3DObject(*t_renderData->perspective, t_mesh, vertexCount, vertices, normals, coordinates, t_renderData, t_bulbs, t_bulbsCount);
    addCurrentCalcs(vertexCount);
    delete[] xyz;
    delete[] rgbaq;
    delete[] st;
    packet2_update(currentPacket, draw_prim_end(currentPacket->next, 3, DRAW_STQ_REGLIST));
    packet2_chain_close_tag(currentPacket);
}

/** Calculates 3D object data into xyz, rgbq, st
 * After it addCurrentSTQ() can be done
 * @param worldView Matrix
 * @param perspective Matrix with .setPerpsective() used [clone of gluPerspective]
 * @param mesh 3D object
 */
void GifSender::calc3DObject(Matrix t_perspective, Mesh &t_mesh, u32 vertexCount, VECTOR *vertices, VECTOR *normals, VECTOR *coordinates, RenderData *t_renderData, LightBulb *t_bulbs, u16 t_bulbsCount)
{
    VECTOR *colors = new VECTOR[vertexCount];
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
            colors[i][0] = (t_mesh.color.r * lights[i][0] / 128.0F);
            colors[i][1] = (t_mesh.color.g * lights[i][1] / 128.0F);
            colors[i][2] = (t_mesh.color.b * lights[i][2] / 128.0F);
            vector_clamp(colors[i], colors[i], 0.00F, 1.99F);
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

    delete[] colors;
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
        packet2_add_u64(currentPacket, rgbaq[i].rgbaq);
        packet2_add_u64(currentPacket, st[i].uv);
        packet2_add_u64(currentPacket, xyz[i].xyz);
    }
    packet2_pad128(currentPacket, 0);
}
