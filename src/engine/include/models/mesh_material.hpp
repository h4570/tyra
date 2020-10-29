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

class MeshMaterial
{

public:
    MeshMaterial();
    ~MeshMaterial();

    /** Material name. */
    char *getName() const { return name; };
    const u32 &getFacesCount() const { return facesCount; };
    /** Indexes of vertices. Each 3 faces will give you vertices of next triangle. */
    const u32 &getVertexFace(const u32 &i) const { return vertexFaces[i]; };
    /** Indexes of texture coords. Each 3 faces will give you texture coords of next triangle. */
    const u32 &getStFace(const u32 &i) const { return stFaces[i]; };
    /** Indexes of normal vectors used for lighting. Each 3 faces will give you normal vectors of next triangle. */
    const u32 &getNormalFace(const u32 &i) const { return normalFaces[i]; };
    /** Array of vertex faces. Size of getFacesCount() */
    const u32 *getVertexFaces() const { return vertexFaces; };
    /** Array of texture coords faces. Size of getFacesCount() */
    const u32 *getStFaces() const { return stFaces; };
    /** Array of normal vector faces. Size of getFacesCount() */
    const u32 *getNormalFaces() const { return normalFaces; };

    /** Set faces count and allocate faces memory. */
    void setFacesCount(const u32 &t_val);

    /** Set vertex face. Do not call this method until you know what you do.
     * Should be called by data loader. */
    void setVertexFace(const u32 &t_index, const u32 &t_val) { vertexFaces[t_index] = t_val; }

    /** Set st face. Do not call this method until you know what you do.
     * Should be called by data loader. */
    void setStFace(const u32 &t_index, const u32 &t_val) { stFaces[t_index] = t_val; }

    /** Set vertex face. Do not call this method until you know what you do.
     * Should be called by data loader. */
    void setNormalFace(const u32 &t_index, const u32 &t_val) { normalFaces[t_index] = t_val; }

    /** Set material name. */
    void setName(char *t_val);

private:
    u32 facesCount;
    u32 *vertexFaces, *stFaces, *normalFaces;
    u8 isNameAllocated, areFacesAllocated;
    char *name;
};

#endif
