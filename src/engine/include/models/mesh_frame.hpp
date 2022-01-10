/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020 - 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _TYRA_MESH_FRAME_
#define _TYRA_MESH_FRAME_

#include <stddef.h>
#include <tamtypes.h>
#include "bounding_box.hpp"
#include "math/point.hpp"
#include "math/vector3.hpp"
#include "./mesh_material.hpp"

/**
 * Class which contains core mesh data 
 * which are needed for drawing.
 * Static object (not animated), will have 
 * only one instance of this class. 
 */
class MeshFrame
{

public:
    MeshFrame();
    ~MeshFrame();

    // ----
    // Getters
    // ----

    /** 
     * Auto generated unique Id. 
     * Core role of this variable is to select correct texture to draw
     */
    const u32 &getId() const { return id; };

    const u32 &getVertexCount() const { return vertexCount; };
    const u32 &getSTsCount() const { return stsCount; };
    const u32 &getNormalsCount() const { return normalsCount; };
    const u32 &getMaterialsCount() const { return materialsCount; };

    /** Returns vertex. 3 vertices = 1 triangle. */
    Vector3 &getVertex(const u32 &i) const { return vertices[i]; };

    /** Returns texture coordinate. */
    Point &getST(const u32 &i) const { return sts[i]; };

    /** Returns normal vector. Used for lighting. */
    Vector3 &getNormal(const u32 &i) const { return normals[i]; };

    /** Returns material, which is a mesh "subgroup". */
    MeshMaterial &getMaterial(const u32 &i) const { return materials[i]; };

    /** 
     * Returns material, which is a mesh "subgroup".
     * NULL if not found.
     */
    MeshMaterial *getMaterialById(const u32 &t_id) const
    {
        for (u32 i = 0; i < materialsCount; i++)
            if (materials[i].getId() == t_id)
                return &materials[i];
        return NULL;
    }

    /** Array of vertices. Size of getVertexCount() */
    Vector3 *getVertices() const { return vertices; };

    /** Array of vertices. Size of getSTsCount() */
    Point *getSTs() const { return sts; };

    /** Array of vertices. Size of getNormalsCount() */
    Vector3 *getNormals() const { return normals; };

    /** Array of materials. Size of getMaterialsCount() */
    MeshMaterial *getMaterials() const { return materials; };

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
    // Setters
    // ----

    /** 
     * Do not call this method unless you know what you do.
     * Should be called by data loader. 
     */
    void setVertex(const u32 &t_index, const Vector3 &t_val) { vertices[t_index].set(t_val); }

    /** 
     * Do not call this method unless you know what you do.
     * Should be called by data loader. 
     */
    void setNormal(const u32 &t_index, const Vector3 &t_val) { normals[t_index].set(t_val); }

    /** 
     * Do not call this method unless you know what you do.
     * Should be called by data loader. 
     */
    void setST(const u32 &t_index, const Point &t_val) { sts[t_index].set(t_val); }

    // ----
    // Other
    // ----

    /** 
     * Check if this object is mother. 
     * Mother = object loaded with loadObj(), loadDff().. 
     * Have all mother data: vertex, st, etc.. 
     * Non-mother = object loaded with loadFrom().  
     * Have reference copy to mother data: vertex, st, etc.. 
     *  
     * When you will destruct mother object, all vertex, st data will be deleted.
     * When you will destruct non-mother object, all vertex, st data will be NOT deleted.
     */
    const u8 &isMother() const { return _isMother; };

    const u8 &areSTsAllocated() const { return _areSTsAllocated; };
    const u8 &areVerticesAllocated() const { return _areVerticesAllocated; };
    const u8 &areNormalsAllocated() const { return _areNormalsAllocated; };
    const u8 &areMaterialsAllocated() const { return _areMaterialsAllocated; };
    const u8 &isBoundingBoxCalculated() const { return _isBoundingBoxCalculated; };

    /** Create reference copy (non-mother) */
    void copyFrom(MeshFrame *t_refCopy);

    /** Set STs count and allocate memory. */
    void allocateSTs(const u32 &t_val);

    /** Set vertex count and allocate memory. */
    void allocateVertices(const u32 &t_val);

    /** Set normals count and allocate memory. */
    void allocateNormals(const u32 &t_val);

    /** Set materials count and allocate memory. */
    void allocateMaterials(const u32 &t_val);

    /** 
     * Calculates bounding box (AABB) for frame and for materiaals.
     * Should be called by data loader,
     * so there is no need to run it again.
     */
    void calculateBoundingBoxes();
    
private:
    BoundingBox *boundingBoxObj;
    u8 _isMother,
        _areSTsAllocated,
        _areVerticesAllocated,
        _areNormalsAllocated,
        _areMaterialsAllocated,
        _isBoundingBoxCalculated;
    u32 vertexCount, stsCount, normalsCount, materialsCount, id;
    MeshMaterial *materials;
    Point __attribute__((aligned(16))) * sts;
    Vector3 __attribute__((aligned(16))) * vertices, *normals;
};

#endif
