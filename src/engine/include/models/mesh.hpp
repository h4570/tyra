/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _TYRA_MESH_
#define _TYRA_MESH_

#include "math/vector3.hpp"
#include "math/plane.hpp"
#include "./mesh_texture.hpp"
#include "./mesh_frame.hpp"
#include <tamtypes.h>
#include <draw_types.h>
#include <draw_buffers.h>
#include <draw_sampling.h>
#include "./anim_state.hpp"

/** Class which have common types for all 3D objects */
class Mesh
{

public:
    Mesh();
    ~Mesh();

    Vector3 position, rotation;
    float scale;
    u8 shouldBeLighted, shouldBeBackfaceCulled, shouldBeFrustumCulled;
    color_t color;

    // ----
    // Getters
    // ----

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

    /** Returns bounding box vertex at given index with added mesh position.
     * @param o_result Result
     * @param offset 0-7. Because, bounding box have 8 corners
     */
    void getCurrentBoundingBoxVertex(Vector3 &o_result, const u32 &t_index);

    // ----
    //  Setters
    // ----

    // ----
    //  Other
    // ----

    /** 
     * Load mesh data from .obj files. Animation not supported.
     * Be aware that texture names will be grabbed from material name.
     * So no .mtl file is needed. Please check if material names in .obj file
     * are equal to texture file names.
     * @param t_subfolder Relative path. Example "meshes/blocks"
     * @param t_objFile File name without extension. Example "sand"
     * @param t_scale Scale. For 1:1, type 1.0F
     * @param t_framesCount Amount of frames. 
     * Example: 2.
     * Searched files: sand_000001.obj, sand_000002.obj
     * @param t_invertT Sometimes textures are in UV format, in this case, invertT should be set to true.
     * Converts Y coordinate texture from Y to 1.0F - Y
     */
    void loadObj(char *t_subfolder, char *t_objFile, const float &t_scale, const u32 &t_framesCount, const u8 &t_invertT);

    /** 
     * Load mesh data from .obj file. Animation supported.
     * Be aware that texture names will be grabbed from material name.
     * So no .mtl file is needed. Please check if material names in .obj file
     * are equal to texture file names.
     * @param t_subfolder Relative path. Example "meshes/blocks"
     * @param t_objFile File name without extension. Example "sand"
     * @param t_scale Scale. For 1:1, type 1.0F
     * @param t_invertT Sometimes textures are in UV format, in this case, invertT should be set to true.
     * Converts Y coordinate texture from Y to 1.0F - Y
     */
    void loadObj(char *t_subfolder, char *t_objFile, const float &t_scale, const u8 &t_invertT);

    /** 
     * Load mesh data from RenderWare .dff file. Animation not supported
     * @param t_subfolder Relative path. Example "meshes/blocks"
     * @param t_objFile File name without extension. Example "sand"
     * @param t_scale Scale. For 1:1, type 1.0F
     * @param t_invertT Sometimes textures are in UV format, in this case, invertT should be set to true.
     * Converts Y coordinate texture from Y to 1.0F - Y
     */
    void loadDff(char *t_subfolder, char *t_dffFile, const float &t_scale, const u8 &t_invertT);

    /** 
     * Load mesh data from Quake II .md2 file. Animation supported.
     * @param t_subfolder Relative path. Example "meshes/blocks"
     * @param t_objFile File name without extension. Example "sand"
     * @param t_scale Scale. For 1:1, type 1.0F
     * @param t_invertT Sometimes textures are in UV format, in this case, invertT should be set to true.
     * Converts Y coordinate texture from Y to 1.0F - Y
     */
    void loadMD2(char *t_subfolder, char *t_md2File, const float &t_scale, const u8 &t_invertT);

    /** Copy by reference mesh data from other mesh */
    void loadFrom(const Mesh &t_mesh);

    /** Check if are there any frames */
    u8 isDataLoaded() const { return framesCount > 0; };

    /** Loop in one frame */
    void playAnimation(const u32 &t_frame) { playAnimation(t_frame, t_frame); }

    /** Play animation in loop from startFrame to endFrame */
    void playAnimation(const u32 &t_startFrame, const u32 &t_endFrame);

    /** Play animation from startFrame to endFrame and after loop in stayFrame */
    void playAnimation(const u32 &t_startFrame, const u32 &t_endFrame, const u32 &t_stayFrame);

    /** 
     * Check if this class loaded mesh data first.
     * Meshes which use loadFrom() method have false there.
    */
    const u8 &isMother() const { return _isMother; };

    // TODO

    // void getMinMax(Vector3 *t_min, Vector3 *t_max);
    u32 getVertexCount();
    void setAnimSpeed(float t_value);
    u8 isInFrustum(Plane *t_frustumPlanes);

    clutbuffer_t clut;
    lod_t lod;
    u32 id;

    // NOWE TODO

    u32 framesCount;
    MeshFrame *frames;
    AnimState animState;
    void animate();
    u32 getDrawData(u32 t_materialIndex, VECTOR *o_vertices, VECTOR *o_normals, VECTOR *o_coordinates, Vector3 &t_cameraPos);
    u32 getFacesCount();
    u8 isMemoryAllocated;

private:
    u8 _isMother;
    Vector3 calc3Vectors[3];
    void setupLodAndClut();
    void setDefaultColor();
};

#endif
