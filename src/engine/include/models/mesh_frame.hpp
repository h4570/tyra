/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _TYRA_MESH_FRAME_
#define _TYRA_MESH_FRAME_

#include <stddef.h>
#include <tamtypes.h>
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
     * Returns bounding box (AABB).
     * Total length: 8
     */
    Vector3 *getBoundingBox() { return boundingBox; };

    /** 
     * Returns bounding box (AABB) vertex.
     * Total length: 8
     */
    Vector3 &getBoundingBoxVertex(const u8 &i) { return boundingBox[i]; };

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

    const u8 &areSTsAllocated() const { return _areSTsAllocated; };
    const u8 &areVerticesAllocated() const { return _areVerticesAllocated; };
    const u8 &areNormalsAllocated() const { return _areNormalsAllocated; };
    const u8 &areMaterialsAllocated() const { return _areMaterialsAllocated; };
    const u8 &isBoundingBoxCalculated() const { return _isBoundingBoxCalculated; };

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
    Vector3 boundingBox[8];
    u8 _areSTsAllocated,
        _areVerticesAllocated,
        _areNormalsAllocated,
        _areMaterialsAllocated,
        _isBoundingBoxCalculated;
    u32 vertexCount, stsCount, normalsCount, materialsCount;
    MeshMaterial *materials;
    Point *sts __attribute__((aligned(16)));
    Vector3
        *vertices __attribute__((aligned(16))),
        *normals __attribute__((aligned(16)));
};

#endif
