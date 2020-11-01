/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _TYRA_OBJECT3D_
#define _TYRA_OBJECT3D_

#include "math/vector3.hpp"
#include "math/plane.hpp"
#include "dff_model.hpp"
#include "md2_model.hpp"
#include "./mesh_texture.hpp"
#include "./mesh_frame.hpp"
#include <tamtypes.h>
#include <draw_types.h>
#include <draw_buffers.h>
#include <draw_sampling.h>

/** Class which have common types for all 3D objects */
class Mesh
{

public:
    Vector3 position, rotation;
    u8 shouldBeLighted, shouldBeBackfaceCulled, shouldBeFrustumCulled;

    MD2Model *md2;
    DffModel *dff;
    float scale;
    color_t color;

    Mesh();
    ~Mesh();

    void loadObj(char *t_subfolder, char *t_objFile, Vector3 &t_initPos, float t_scale, u16 t_framesCount);
    // void setObj(Vector3 &t_initPos); // TODO
    void loadDff(char *t_subfolder, char *t_dffFile, Vector3 &t_initPos, float t_scale);
    void setDff(Vector3 &t_initPos, DffModel *t_dffModel);
    void loadMD2(char *t_subfolder, char *t_md2File, Vector3 &t_initPos, float t_scale);
    void getMinMax(Vector3 *t_min, Vector3 *t_max);
    void playAnimation(u32 t_startFrame, u32 t_endFrame);
    u32 getVertexCount();
    void setAnimSpeed(float t_value);
    u32 getDrawData(u32 splitIndex, VECTOR *t_vertices, VECTOR *t_normals, VECTOR *t_coordinates, Vector3 &t_cameraPos);
    u8 isInFrustum(Plane *t_frustumPlanes);

    u8 isMd2Loaded, isObjLoaded, isDffLoaded, isSpecInitialized;
    clutbuffer_t clut;
    lod_t lod;
    u32 id;
    MeshTexture *textures;

    // NOWE TODO

    u16 framesCount;
    MeshFrame *frames;
    AnimState animState;
    void animate();
    u32 getDrawData(u32 t_materialIndex, VECTOR *o_vertices, VECTOR *o_normals, VECTOR *o_coordinates, Vector3 &t_cameraPos, float t_scale, u8 t_shouldBeBackfaceCulled);
    u32 getFacesCount();
    u8 isMemoryAllocated;

    /** Array of materials. Size of getMaterialsCount() */
    MeshMaterial *getMaterials() const { return frames[0].getMaterials(); };

    const u32 &getMaterialsCount() const { return frames[0].getMaterialsCount(); };

    /** Returns material, which is a mesh "subgroup". */
    MeshMaterial &getMaterial(const u32 &i) const { return frames[0].getMaterial(i); };

    /** 
     * Returns material, which is a mesh "subgroup".
     * NULL if not found.
     */
    MeshMaterial *getMaterialById(const u32 &t_id) const { return frames[0].getMaterialById(t_id); }

private:
    Vector3 calc3Vectors[3];
    void setupLodAndClut();
    void setDefaultWrapSettings(texwrap_t &t_wrapSettings);
    u32 verticesCount;
    Vector3 *vertices;
    void loadTextures(char *t_subfolder, char *t_extension);
    void setVerticesReference(u32 t_verticesCount, Vector3 *t_verticesRef);
    void createSpecIfNotCreated();
    void setDefaultColor();
    void getFarthestVertex(Vector3 *o_result, int t_offset);
};

#endif
