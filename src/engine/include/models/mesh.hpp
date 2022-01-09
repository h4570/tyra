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
#include "./texture.hpp"
#include "./mesh_frame.hpp"
#include <tamtypes.h>
#include <draw_buffers.h>
#include <draw_sampling.h>
#include "./anim_state.hpp"

/** 
 * Class which have contain 3D object data.
 * External data can be loaded via loadXXX() methods.
 */
class Mesh
{

public:
    Mesh();
    ~Mesh();

    Vector3 position, rotation;
    float scale;
    u8 shouldBeLighted;
    /** 
     * When true, invisible triangles of mesh materials are not drawn.
     */
    u8 shouldBeBackfaceCulled;
    /** 
     * When true, mesh materials are not drawn when they are outside of view frustum.
     */
    u8 shouldBeFrustumCulled;
    clutbuffer_t clut;
    lod_t lod;

    // ----
    // Getters
    // ----

    /** Auto generated unique Id. */
    const u32 &getId() const { return id; };

    /** Array of materials. Size of getMaterialsCount() */
    MeshMaterial *getMaterials() const { return frames[0].getMaterials(); };

    const u32 &getMaterialsCount() const { return frames[0].getMaterialsCount(); };

    /** Returns material, which is a mesh "subgroup". */
    MeshMaterial &getMaterial(const u32 &i) const { return frames[0].getMaterial(i); };

    /** Count of frames. Static object (not animated) will have only 1 frame. */
    const u32 &getFramesCount() const { return framesCount; };

    /** Count of vertices.  */
    u32 getVertexCount() { return frames[0].getVertexCount(); };

    /** Returns single frame. */
    MeshFrame &getFrame(const u32 &i) const { return frames[i]; };

    /** Array of frames. Size of getFramesCount() */
    MeshFrame *getFrames() const { return frames; };

    const u32 &getCurrentAnimationFrame() const { return animState.currentFrame; };

    const u32 &getNextAnimationFrame() const { return animState.nextFrame; };

    const u32 &getStartAnimationFrame() const { return animState.startFrame; };

    const u32 &getEndAnimationFrame() const { return animState.endFrame; };

    const u32 &getStayAnimationFrame() const { return animState.stayFrame; };

    /** 
     * Returns material, which is a mesh "subgroup".
     * NULL if not found.
     */
    MeshMaterial *getMaterialById(const u32 &t_id) const { return frames[0].getMaterialById(t_id); }

    /** 
     * Returns bounding vertex of current frame.
     * @param i 0-7. Because, bounding box have 8 corners
     */
    const Vector3 &getCurrentBoundingBoxVertex(const u8 &i) { return frames[animState.currentFrame].getBoundingBoxVertex(i); };

    /** @returns bounding box vertex array of current frame. Size: 8 */
    const Vector3 *getCurrentBoundingBoxVertices() const { return frames[animState.currentFrame].getBoundingBoxVertices(); };

    /** @returns bounding box object of current frame. */
    const BoundingBox *getCurrentBoundingBox() const { return frames[animState.currentFrame].getBoundingBox(); };

    // ----
    //  Setters
    // ----

    // ----
    //  Other
    // ----

    /** 
     * Load mesh data from .obj files. Animation supported.
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
     * Load mesh data from .obj file. Animation not supported.
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
     * @param t_md2File File name without extension. Example "sand"
     * @param t_scale Scale. For 1:1, type 1.0F
     * @param t_invertT Sometimes textures are in UV format, in this case, invertT should be set to true.
     * Converts Y coordinate texture from Y to 1.0F - Y
     */
    void loadMD2(char *t_subfolder, char *t_md2File, const float &t_scale, const u8 &t_invertT);
    
    void loadMD3(char *t_subfolder, char *t_md3File, const float &t_scale, const u8 &t_invertT);

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

    void setAnimSpeed(const float &t_value) { animState.speed = t_value; }

    const u8 &areFramesAllocated() const { return _areFramesAllocated; };

    /** 
     * Check if this class loaded mesh data first.
     * Meshes which use loadFrom() method have false there.
    */
    const u8 &isMother() const { return _isMother; };

    /** 
     * Check if this class loaded mesh data first.
     * Meshes which use loadFrom() method have false there.
    */
    const u8 &isStayAnimationSet() const { return animState.isStayFrameSet; };

    /** True when mesh is in view frustum */
    u8 isInFrustum(Plane *t_frustumPlanes);

    /** 
     * Do not call this method unless you know what you do.
     * Should be called by renderer. 
     */
    void animate();

    /** 
     * Do not call this method unless you know what you do.
     * Should be called by renderer. 
     */
    u32 getDrawData(u32 t_materialIndex, VECTOR *o_vertices, VECTOR *o_normals, VECTOR *o_coordinates, Vector3 &t_cameraPos);

private:
    AnimState animState;
    MeshFrame *frames;
    u32 id, framesCount;
    u8 _isMother, _areFramesAllocated;
    Vector3 calc3Vectors[3];
    void setDefaultLODAndClut();
};

#endif
