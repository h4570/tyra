/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "../include/modules/vif_sender.hpp"

#include <gs_gp.h>
#include <dma.h>
#include <gif_tags.h>
#include "../include/utils/math.hpp"
#include "../include/utils/debug.hpp"

const u32 VU1_PACKAGE_VERTS_PER_BUFF = 96; // Remember to modify buffer size in vu1 also
const u32 VU1_PACKAGES_PER_PACKET = 9;
const u32 VU1_PACKET_SIZE = 256; // should be 128, but 256 is more safe for future
const u8 VU1_PARAMS_ADDRESS = 4;
const u8 VU1_RGBA_ADDRESS = 8;

// ----
// Constructors/Destructors
// ----

// VU1 micro program
extern u32 VU1Draw3D_CodeStart __attribute__((section(".vudata")));
extern u32 VU1Draw3D_CodeEnd __attribute__((section(".vudata")));
//

VifSender::VifSender(Light *t_light)
{
    PRINT_LOG("Initializing VifSender");
    light = t_light;
    lastVertCount = 0;
    isDrawWaitEnabled = true;
    dma_channel_initialize(DMA_CHANNEL_VIF1, NULL, 0);
    dma_channel_fast_waits(DMA_CHANNEL_VIF1);
    uploadMicroProgram();
    packets[0] = packet2_create(VU1_PACKET_SIZE, P2_TYPE_NORMAL, P2_MODE_CHAIN, true);
    packets[1] = packet2_create(VU1_PACKET_SIZE, P2_TYPE_NORMAL, P2_MODE_CHAIN, true);
    context = 0;
    setDoubleBufferAddStaticData();
    PRINT_LOG("VifSender initialized!");
}

VifSender::~VifSender()
{
    packet2_free(packets[0]);
    packet2_free(packets[1]);
}

// ----
// Methods
// ----

void VifSender::uploadMicroProgram()
{
    packet2_t *packet2 = packet2_create(
        packet2_utils_get_packet_size_for_program(&VU1Draw3D_CodeStart, &VU1Draw3D_CodeEnd) + 1, // + end tag
        P2_TYPE_NORMAL,
        P2_MODE_CHAIN,
        1);
    packet2_vif_add_micro_program(packet2, 0, &VU1Draw3D_CodeStart, &VU1Draw3D_CodeEnd);
    packet2_utils_vu_add_end_tag(packet2);
    dma_channel_wait(DMA_CHANNEL_VIF1, 0);
    dma_channel_send_packet2(packet2, DMA_CHANNEL_VIF1, 1);
    packet2_free(packet2);
}

void VifSender::calcMatrix(const RenderData &t_renderData, const Vector3 &t_position, const Vector3 &t_rotation)
{
    model.identity();
    model.rotate(t_rotation);
    model.translate(t_position);

    modelViewProj.identity();
    modelViewProj = model * modelViewProj;
    modelViewProj = *t_renderData.view * modelViewProj;
    modelViewProj = *t_renderData.projection * modelViewProj;
}

void VifSender::drawMesh(RenderData *t_renderData, Matrix t_perspective, u32 vertCount2, VECTOR *vertices, VECTOR *normals, VECTOR *coordinates, Mesh &t_mesh, LightBulb *t_bulbs, u16 t_bulbsCount, texbuffer_t *textureBuffer)
{
    // we have to split 3D object into small parts, because of small memory of VU1

    for (u32 i = 0; i < vertCount2;)
    {
        currPacket = packets[context];
        packet2_reset(currPacket, false);
        for (u8 j = 0; j < VU1_PACKAGES_PER_PACKET; j++) // how many "packages" per one packet
        {
            if (i != 0) // we have to go back to avoid the visual artifacts
                i -= 3;

            const u32 endI = i + (VU1_PACKAGE_VERTS_PER_BUFF - 1) > vertCount2 ? vertCount2 : i + (VU1_PACKAGE_VERTS_PER_BUFF - 1);
            drawVertices(t_mesh, i, endI, vertices, coordinates, t_renderData->prim, textureBuffer, isDrawWaitEnabled ? endI == vertCount2 : false);
            if (endI == vertCount2) // if there are no more vertices to draw, break
            {
                i = vertCount2;
                break;
            }
            i += (VU1_PACKAGE_VERTS_PER_BUFF - 1);
            i++;
        }
        packet2_utils_vu_add_end_tag(currPacket);
        dma_channel_send_packet2(currPacket, DMA_CHANNEL_VIF1, 1);
        dma_channel_wait(DMA_CHANNEL_VIF1, 0);
        context = !context;
    }
}

void VifSender::setDoubleBufferAddStaticData()
{
    packet2_t *settings = packet2_create(10, P2_TYPE_NORMAL, P2_MODE_CHAIN, true);
    packet2_utils_vu_open_unpack(settings, 0, false);
    {
        packet2_utils_gs_add_draw_finish_giftag(settings);
        packet2_utils_gif_add_set(settings, 1);
    }
    packet2_utils_vu_close_unpack(settings);
    packet2_utils_vu_add_double_buffer(settings, 10, 498);
    packet2_utils_vu_add_end_tag(settings);
    dma_channel_send_packet2(settings, DMA_CHANNEL_VIF1, true);
    dma_channel_wait(DMA_CHANNEL_VIF1, 0);
    packet2_free(settings);
}

/** Draw using PATH1 */
void VifSender::drawVertices(Mesh &t_mesh, u32 t_start, u32 t_end, VECTOR *t_vertices, VECTOR *t_coordinates, prim_t *t_prim, texbuffer_t *t_texBuff, u8 t_addDrawWait)
{
    const u32 vertCount = t_end - t_start;
    lastVertCount = vertCount;
    packet2_utils_vu_open_unpack(currPacket, 0, true);
    packet2_add_data(currPacket, modelViewProj.data, 4);
    packet2_add_u32(currPacket, t_addDrawWait); // Draw finish?
    packet2_add_u32(currPacket, vertCount);     // Vertex count
    packet2_add_u32(currPacket, vertCount / 3); // Triangles count
    packet2_add_u32(currPacket, 0);             // Free
    packet2_utils_gs_add_lod(currPacket, &t_mesh.lod);
    packet2_utils_gs_add_texbuff_clut(currPacket, t_texBuff, &t_mesh.clut);
    packet2_utils_gs_add_prim_giftag(currPacket, t_prim, vertCount, DRAW_STQ2_REGLIST, 3, 0);
    packet2_add_u32(currPacket, t_mesh.color.r);
    packet2_add_u32(currPacket, t_mesh.color.g);
    packet2_add_u32(currPacket, t_mesh.color.b);
    packet2_add_u32(currPacket, t_mesh.color.a);
    u32 vif_added_bytes = packet2_utils_vu_close_unpack(currPacket);
    packet2_utils_vu_add_unpack_data(currPacket, vif_added_bytes, t_vertices + t_start, vertCount, true);
    vif_added_bytes += vertCount;
    packet2_utils_vu_add_unpack_data(currPacket, vif_added_bytes, t_coordinates + t_start, vertCount, true);
    vif_added_bytes += vertCount;
    packet2_utils_vu_add_start_program(currPacket, 0);
}

void VifSender::drawTheSameWithOtherMatrices(const RenderData &t_renderData, Mesh **t_meshes, const u32 &t_skip, const u32 &t_count)
{
    packet2_t *packet2 = packet2_create(300, P2_TYPE_NORMAL, P2_MODE_CHAIN, true);
    u8 switchCounter = 0;
    u32 i;
    for (i = t_skip; i < t_count; i++)
    {
        model.identity();
        model.rotate(t_meshes[i]->rotation);
        model.translate(t_meshes[i]->position);

        modelViewProj.identity();
        modelViewProj = model * modelViewProj;
        modelViewProj = *t_renderData.view * modelViewProj;
        modelViewProj = *t_renderData.projection * modelViewProj;

        packet2_utils_vu_open_unpack(packet2, 0, true);
        {
            packet2_add_data(packet2, modelViewProj.data, 4);
        }
        packet2_utils_vu_close_unpack(packet2);
        packet2_utils_vu_open_unpack(packet2, VU1_RGBA_ADDRESS, true);
        {
            packet2_add_u32(packet2, t_meshes[i]->color.r);
            packet2_add_u32(packet2, t_meshes[i]->color.g);
            packet2_add_u32(packet2, t_meshes[i]->color.b);
            packet2_add_u32(packet2, t_meshes[i]->color.a);
        }
        packet2_utils_vu_close_unpack(packet2);
        if (i != t_count - 1)
            packet2_utils_vu_add_start_program(packet2, 0);

        if (switchCounter++ >= 32)
        {
            switchCounter = 0;
            if (i == t_count - 1) // is last
            {
                packet2_utils_vu_open_unpack(packet2, VU1_PARAMS_ADDRESS, true);
                {
                    packet2_add_u32(packet2, true);              // Draw wait finish?
                    packet2_add_u32(packet2, lastVertCount);     // Vertex count
                    packet2_add_u32(packet2, lastVertCount / 3); // Triangles count
                    packet2_add_u32(packet2, 0);                 // Free
                }
                packet2_utils_vu_close_unpack(packet2);
                packet2_utils_vu_add_start_program(packet2, 0);
            }
            packet2_utils_vu_add_end_tag(packet2);
            dma_channel_wait(DMA_CHANNEL_VIF1, 0);
            dma_channel_send_packet2(packet2, DMA_CHANNEL_VIF1, 1);
            packet2_reset(packet2, false);
        }
    }
    if (switchCounter != 0)
    {
        packet2_utils_vu_open_unpack(packet2, VU1_PARAMS_ADDRESS, true);
        {
            packet2_add_u32(packet2, true);              // Draw wait finish?
            packet2_add_u32(packet2, lastVertCount);     // Vertex count
            packet2_add_u32(packet2, lastVertCount / 3); // Triangles count
            packet2_add_u32(packet2, 0);                 // Free
        }
        packet2_utils_vu_close_unpack(packet2);
        packet2_utils_vu_add_start_program(packet2, 0);
        packet2_utils_vu_add_end_tag(packet2);
        dma_channel_wait(DMA_CHANNEL_VIF1, 0);
        dma_channel_send_packet2(packet2, DMA_CHANNEL_VIF1, 1);
    }
    packet2_free(packet2);
}
