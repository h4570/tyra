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
const u32 VU1_PACKAGES_PER_PACKET = 6;
const u32 VU1_PACKET_SIZE = 128;

// ----
// Constructors/Destructors
// ----

// VU1 micro program
extern u32 VU1Draw3D_CodeStart __attribute__((section(".vudata")));
extern u32 VU1Draw3D_CodeEnd __attribute__((section(".vudata")));
//

VifSender::VifSender()
{
    PRINT_LOG("Initializing VifSender");
    PRINT_LOG("VifSender initialized!");
    dma_channel_initialize(DMA_CHANNEL_VIF1, NULL, 0);
    dma_channel_fast_waits(DMA_CHANNEL_VIF1);
    uploadMicroProgram();
    packets[0] = packet2_create(VU1_PACKET_SIZE, P2_TYPE_NORMAL, P2_MODE_CHAIN, true);
    packets[1] = packet2_create(VU1_PACKET_SIZE, P2_TYPE_NORMAL, P2_MODE_CHAIN, true);
    matricesPacket = packet2_create(4, P2_TYPE_NORMAL, P2_MODE_CHAIN, true);
    context = 0;
    setDoubleBuffer();
}

VifSender::~VifSender()
{
    packet2_free(packets[0]);
    packet2_free(packets[1]);
    packet2_free(matricesPacket);
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

void VifSender::sendMatrices(const RenderData &t_renderData, const Vector3 &t_position, const Vector3 &t_rotation)
{
    vec3ToNative(position, t_position, 1.0F);
    vec3ToNative(rotation, t_rotation, 1.0F);
    create_local_world(localWorld, position, rotation);
    create_local_screen(localScreen, localWorld, t_renderData.worldView->data, t_renderData.perspective->data);
    packet2_reset(matricesPacket, false);
    packet2_utils_vu_add_unpack_data(matricesPacket, 0, &localScreen, 8, 0);
    packet2_utils_vu_add_end_tag(matricesPacket);
    dma_channel_wait(DMA_CHANNEL_VIF1, 0);
    dma_channel_send_packet2(matricesPacket, DMA_CHANNEL_VIF1, 1);
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
            drawVertices(t_mesh, i, endI, vertices, coordinates, t_renderData->prim, textureBuffer);
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

void VifSender::setDoubleBuffer()
{
    packet2_t *settings = packet2_create(2, P2_TYPE_NORMAL, P2_MODE_CHAIN, true);
    packet2_utils_vu_add_double_buffer(settings, 8, 496);
    packet2_utils_vu_add_end_tag(settings);
    dma_channel_send_packet2(settings, DMA_CHANNEL_VIF1, 1);
    dma_channel_wait(DMA_CHANNEL_VIF1, 0);
    packet2_free(settings);
}

/** Draw using PATH1 */
void VifSender::drawVertices(Mesh &t_mesh, u32 t_start, u32 t_end, VECTOR *t_vertices, VECTOR *t_coordinates, prim_t *t_prim, texbuffer_t *textureBuffer)
{
    const u32 vertCount = t_end - t_start;
    u32 vif_added_bytes = 0;
    packet2_utils_vu_open_unpack(currPacket, 0, 1);
    // TODO get this via screensettings
    packet2_add_float(currPacket, 2048.0F);                   // scale
    packet2_add_float(currPacket, 2048.0F);                   // scale
    packet2_add_float(currPacket, ((float)0xFFFFFF) / 32.0F); // scale
    packet2_add_u32(currPacket, vertCount);                   // vertex count
    packet2_utils_gif_add_set(currPacket, 1);
    packet2_utils_gs_add_lod(currPacket, &t_mesh.lod);
    packet2_utils_gs_add_texbuff_clut(currPacket, textureBuffer, &t_mesh.clut);
    packet2_utils_gs_add_prim_giftag(currPacket, t_prim, vertCount, DRAW_STQ2_REGLIST, 3, 0);

    for (u8 j = 0; j < 4; j++)
        packet2_add_u32(currPacket, 128);

    // Clipping tests start

    // // const float minZ = 1;
    // // const float maxZ = 65535;
    // // const int iGuardDimXY = 2048;

    // // vu1.addFloat(1.0F); // f_TODO clipping maybe there is problem?
    // // vu1.addFloat(1.0F);
    // // vu1.addFloat(1.0F);
    // // vu1.addFloat(1.0F);
    // //  float xClip = (float)2048.0f/(drawContext.GetFBWidth() * 0.5f * 2.0f);
    // //       packet += Math::Max( xClip, 1.0f );
    // //       float yClip = (float)2048.0f/(drawContext.GetFBHeight() * 0.5f * 2.0f);
    // //       packet += Math::Max( yClip, 1.0f );
    // //       float depthClip = 2048.0f / depthClipToGs;
    // //       // F_FIXME: maybe these 2048's should be 2047.5s...
    // //       depthClip *= 1.003f; // round up a bit for fp error (????)
    // //       packet += depthClip;
    // //       // enable/disable clipping
    // //       packet += (drawContext.GetDoClipping()) ? 1 : 0;

    // u32 depthBits = 24; // or 28(fog) or 16
    // float depthClipToGs = (float)((1 << depthBits) - 1) / 2.0f;
    // vu1.addFloat(2048.0f / (640.0F * 0.5f * 2.0f));
    // vu1.addFloat(2048.0f / (480.0F * 0.5f * 2.0f));
    // vu1.addFloat((2048.0f / depthClipToGs) * 1.003F);
    // // vu1.addFloat(2048.0F);                   // scale
    // // vu1.addFloat(2048.0F);                   // scale
    // // vu1.addFloat(((float)0xFFFFFF) / 32.0F); // scale
    // vu1.addFloat(0.0F);
    // // vu1.addFloat(0.5f * iGuardDimXY);
    // // vu1.addFloat(-0.5f * iGuardDimXY);
    // // vu1.addFloat(1.0F);
    // // vu1.addFloat(500.0F); // far

    packet2_add_float(currPacket, 0.0F);
    packet2_add_float(currPacket, 0.0F);
    packet2_add_float(currPacket, 0.0F);
    packet2_add_float(currPacket, 0.0F);

    //// Clipping tests end
    vif_added_bytes += packet2_utils_vu_close_unpack(currPacket);
    packet2_utils_vu_add_unpack_data(currPacket, vif_added_bytes, t_vertices + t_start, vertCount, 1);
    vif_added_bytes += vertCount;
    packet2_utils_vu_add_unpack_data(currPacket, vif_added_bytes, t_coordinates + t_start, vertCount, 1);
    vif_added_bytes += vertCount;
    packet2_utils_vu_add_start_program(currPacket, 0);
}
