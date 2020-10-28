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
#include "mesh_spec.hpp"
#include "obj_model.hpp"
#include "dff_model.hpp"
#include "md2_model.hpp"
#include <tamtypes.h>
#include <draw_types.h>

/** Class which have common types for all 3D objects */
class Mesh
{

public:
    Vector3 position, rotation;
    u8 shouldBeLighted, shouldBeBackfaceCulled, shouldBeFrustumCulled;

    MeshSpec *spec;
    MD2Model *md2;
    ObjModel *obj;
    DffModel *dff;
    float scale;
    color_t color;
    Vector3 boxVertices[8];

    Mesh();
    ~Mesh();

    void loadObj(char *t_subfolder, char *t_objFile, Vector3 &t_initPos, float t_scale);
    void setObj(Vector3 &t_initPos, ObjModel *t_objModel, MeshSpec *t_spec);
    void loadDff(char *t_subfolder, char *t_dffFile, Vector3 &t_initPos, float t_scale);
    void setDff(Vector3 &t_initPos, DffModel *t_dffModel, MeshSpec *t_spec);
    void loadMD2(char *t_subfolder, char *t_md2File, Vector3 &t_initPos, float t_scale);
    void getMinMax(Vector3 *t_min, Vector3 *t_max);
    void playAnimation(u32 t_startFrame, u32 t_endFrame);
    u32 getVertexCount();
    void setAnimSpeed(float t_value);
    u32 getDrawData(u32 splitIndex, VECTOR *t_vertices, VECTOR *t_normals, VECTOR *t_coordinates, VECTOR *t_colors, Vector3 &t_cameraPos);
    u8 isInFrustum(Plane *t_frustumPlanes);

    u8 isMd2Loaded, isObjLoaded, isDffLoaded, isSpecInitialized;

private:
    void setDefaultWrapSettings(texwrap_t &t_wrapSettings);
    u32 verticesCount;
    Vector3 *vertices;
    void loadTextures(char *t_subfolder, char *t_extension);
    void computeBoundingBox();
    void setVerticesReference(u32 t_verticesCount, Vector3 *t_verticesRef);
    void createSpecIfNotCreated();
    void setDefaultColor();
    void getFarthestVertex(Vector3 *o_result, int t_offset);
};

#endif
