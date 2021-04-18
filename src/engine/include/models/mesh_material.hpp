/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _TYRA_MESH_MATERIAL_
#define _TYRA_MESH_MATERIAL_

#include <tamtypes.h>
#include <draw_types.h>
#include "bounding_box.hpp"
#include "./math/vector3.hpp"
#include "./math/plane.hpp"

/** 
 * Class which contains draw instructions for part of mesh.
 * Mesh can have many materials.
 * For example, car mesh can have three materials:
 * body, tires and windows. 
 */
class MeshMaterial
{

public:
    MeshMaterial();
    ~MeshMaterial();

    color_t color;

    // ----
    // Getters
    // ----

    /** Material name. */
    char *getName() const { return name; };

    /** 
     * Auto generated unique Id. 
     * Core role of this variable is to select correct texture to draw
     */
    const u32 &getId() const { return id; };

    const u32 &getFacesCount() const { return facesCount; };

    /** Indexes of vertices. Each 3 faces will give you vertices of next triangle. */
    u32 &getVertexFace(const u32 &i) const { return vertexFaces[i]; };

    /** Indexes of texture coords. Each 3 faces will give you texture coords of next triangle. */
    u32 &getSTFace(const u32 &i) const { return stFaces[i]; };

    /** Indexes of normal vectors used for lighting. Each 3 faces will give you normal vectors of next triangle. */
    u32 &getNormalFace(const u32 &i) const { return normalFaces[i]; };

    /** Array of vertex faces. Size of getFacesCount() */
    u32 *getVertexFaces() const { return vertexFaces; };

    /** Array of texture coords faces. Size of getFacesCount() */
    u32 *getSTFaces() const { return stFaces; };

    /** Array of normal vector faces. Size of getFacesCount() */
    u32 *getNormalFaces() const { return normalFaces; };

    /** 
     * @returns bounding box (AABB).
     * Total length: 8
     */
    Vector3 *getBoundingBoxVertices() { return boundingBoxObj->getVertices(); };

    /** 
     * @returns bounding box (AABB) vertex.
     * Total length: 8
     */
    const Vector3 &getBoundingBoxVertex(const u8 &i) { return boundingBoxObj->getVertex(i); };

    /**
     * @returns bounding box (AABB) object pointer.
     */
    BoundingBox *getBoundingBox() { return boundingBoxObj; };

    // ----
    //  Setters
    // ----

    /** 
     * Do not call this method unless you know what you do.
     * Should be called by data loader. 
     */
    void setVertexFace(const u32 &t_index, const u32 &t_val) { vertexFaces[t_index] = t_val; }

    /** 
     * Do not call this method unless you know what you do.
     * Should be called by data loader. 
     */
    void setSTFace(const u32 &t_index, const u32 &t_val) { stFaces[t_index] = t_val; }

    /** 
     * Do not call this method unless you know what you do.
     * Should be called by data loader. 
     */
    void setNormalFace(const u32 &t_index, const u32 &t_val) { normalFaces[t_index] = t_val; }

    /** Set material name. */
    void setName(char *t_val);

    /** 
     * Do not call this method unless you know what you do.
     * Should be called by data loader. 
     */
    void setSTsPresent(const u8 &b) { _areSTsPresent = b; };

    /** 
     * Do not call this method unless you know what you do.
     * Should be called by data loader. 
     */
    void setNormalsPresent(const u8 &b) { _areNormalsPresent = b; };

    // ----
    //  Other
    // ----

    const u8 &areSTsPresent() const { return _areSTsPresent; };

    const u8 &areNormalsPresent() const { return _areNormalsPresent; };

    /** Create reference copy (non-mother) */
    void copyFrom(MeshMaterial *t_refCopy);

    const u8 &isNameSet() const { return _isNameSet; };

    /** 
     * Check if this object is mother. 
     * Mother = object loaded with loadObj(), loadDff().. 
     * Have all mother data: vertex faces, st faces, etc.. 
     * Non-mother = object loaded with loadFrom().  
     * Have reference copy to mother data: vertex faces, st faces, etc.. 
     *  
     * When you will destruct mother object, all faces data will be deleted.
     * When you will destruct non-mother object, all faces data will be NOT deleted.
     */
    const u8 &isMother() const { return _isMother; };

    const u8 &areFacesAllocated() const { return _areFacesAllocated; };

    /** Set faces count and allocate memory. */
    void allocateFaces(const u32 &t_val);

    /** 
     * Do not call this method unless you know what you do.
     * Calculates bounding box (AABB).
     * Called automatically in mesh frame class on data loading.
     */
    void calculateBoundingBox(Vector3 *t_vertices, u32 t_vertCount);

    /** True when mesh is in view frustum */
    u8 isInFrustum(Plane *t_frustumPlanes, const Vector3 &position);

private:
    void setDefaultColor();
    BoundingBox *boundingBoxObj;
    u32 facesCount, id;
    u32 *vertexFaces, *stFaces, *normalFaces;
    u8 _isMother,
        _isNameSet,
        _areFacesAllocated,
        _isBoundingBoxCalculated,
        _areSTsPresent,
        _areNormalsPresent;
    char *name;
};

#endif
