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

// Similiar set is in PS2SDK, but for VU1 we have to switch ST with RGBAQ, because VU1 must know Q before sending RGBAQ
#define DRAW_R_STQ_REGLIST ((u64)GIF_REG_ST) << 0 | ((u64)GIF_REG_RGBAQ) << 4 | ((u64)GIF_REG_XYZ2) << 8
const u32 VU1_PACKAGE_VERTS_PER_BUFF = 96; // Remember to modify buffer size in vu1 also
const u32 VU1_PACKAGES_PER_PACKET = 6;

// ----
// Constructors/Destructors
// ----

VifSender::VifSender()
{
    PRINT_LOG("Initializing VifSender");
    PRINT_LOG("VifSender initialized!");

    dma_channel_initialize(DMA_CHANNEL_VIF1, NULL, 0);
    dma_channel_fast_waits(DMA_CHANNEL_VIF1);
}

VifSender::~VifSender() {}

// ----
// Methods
// ----

void VifSender::sendMatrices(const RenderData &t_renderData, const Vector3 &t_position, const Vector3 &t_rotation)
{
    vec3ToNative(position, t_position, 1.0F);
    vec3ToNative(rotation, t_rotation, 1.0F);
    create_local_world(localWorld, position, rotation);
    create_local_screen(localScreen, localWorld, t_renderData.worldView->data, t_renderData.perspective->data);
    vu1.sendSingleRefList(0, &localScreen, 8);
}

void VifSender::drawMesh(RenderData *t_renderData, Matrix t_perspective, u32 vertCount2, VECTOR *vertices, VECTOR *normals, VECTOR *coordinates, const Mesh &t_mesh, LightBulb *t_bulbs, u16 t_bulbsCount, texbuffer_t *textureBuffer)
{
    // we have to split 3D object into small parts, because of small memory of VU1
    for (u32 i = 0; i < vertCount2;)
    {
        vu1.createList();
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
        vu1.sendList();
    }
}

/** Draw using PATH1 */
void VifSender::drawVertices(const Mesh &t_mesh, u32 t_start, u32 t_end, VECTOR *t_vertices, VECTOR *t_coordinates, prim_t *t_prim, texbuffer_t *textureBuffer)
{
    const u32 vertCount = t_end - t_start;
    vu1.addListBeginning();

    // TODO get this via screensettings
    vu1.addFloat(2048.0F);                   // scale
    vu1.addFloat(2048.0F);                   // scale
    vu1.addFloat(((float)0xFFFFFF) / 32.0F); // scale
    vu1.add32(vertCount);                    // vertex count

    vu1.add128(GIF_SET_TAG(1, 0, 0, 0, GIF_FLG_PACKED, 1), GIF_REG_AD); // 1x set tag

    vu1.add128( // tex -> lod
        GS_SET_TEX1(
            t_mesh.lod.calculation,
            t_mesh.lod.max_level,
            t_mesh.lod.mag_filter,
            t_mesh.lod.min_filter,
            t_mesh.lod.mipmap_select,
            t_mesh.lod.l,
            (int)(t_mesh.lod.k * 16.0F)),
        GS_REG_TEX1);

    vu1.add128( // tex -> buff + clut
        GS_SET_TEX0(
            textureBuffer->address >> 6,
            textureBuffer->width >> 6,
            textureBuffer->psm,
            textureBuffer->info.width,
            textureBuffer->info.height,
            textureBuffer->info.components,
            textureBuffer->info.function,
            t_mesh.clut.address >> 6,
            t_mesh.clut.psm,
            t_mesh.clut.storage_mode,
            t_mesh.clut.start,
            t_mesh.clut.load_method),
        GS_REG_TEX0);

    vu1.add128(
        GS_GIFTAG(
            vertCount, // amount of loops
            1,
            1,
            GS_PRIM(
                t_prim->type,
                t_prim->shading,
                t_prim->mapping,
                t_prim->fogging,
                t_prim->blending,
                t_prim->antialiasing,
                t_prim->mapping_type,
                0, // context
                t_prim->colorfix),
            0,  // GIFTAG_PACKED
            3), // STQ + RGBA + XYZ
        DRAW_R_STQ_REGLIST);

    for (u8 j = 0; j < 4; j++)
        vu1.add32(128);

    //// Clipping tests start

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

    vu1.addFloat(0.0F);
    vu1.addFloat(0.0F);
    vu1.addFloat(0.0F);
    vu1.addFloat(0.0F);

    //// Clipping tests end

    vu1.addListEnding();
    vu1.addReferenceList(t_vertices + t_start, 2 * vertCount, 1);
    vu1.addReferenceList(t_coordinates + t_start, 2 * vertCount, 1);
    vu1.addStartProgram();
}
