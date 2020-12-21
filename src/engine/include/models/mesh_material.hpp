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
     * Returns bounding box (AABB).
     * Total length: 8
     */
    Vector3 *getBoundingBox() { return boundingBoxObj->getVertices(); };

    /** 
     * Returns bounding box (AABB) vertex.
     * Total length: 8
     */
    Vector3 &getBoundingBoxVertex(const u8 &i) { return boundingBoxObj->getVertex(i); };

    /**
     * Returns bounding box (AABB) object pointer.
     */
    BoundingBox *getBoundingBoxP() { return boundingBoxObj; };

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

    // ----
    //  Other
    // ----

    const u8 &isNameSet() const { return _isNameSet; };
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
    BoundingBox *boundingBoxObj;
    u32 facesCount, id;
    u32 *vertexFaces, *stFaces, *normalFaces;
    u8 _isNameSet, _areFacesAllocated, _isBoundingBoxCalculated;
    char *name;
};

#endif
