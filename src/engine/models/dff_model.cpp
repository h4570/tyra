/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "../include/models/dff_model.hpp"

#include "../include/utils/debug.hpp"

// ----
// Constructors/Destructors
// ----

DffModel::DffModel() {}

DffModel::~DffModel() {}

// ----
// Methods
// ----

u32 DffModel::getDrawData(u32 splitIndex, VECTOR *o_vertices, VECTOR *o_normals, VECTOR *o_coordinates, VECTOR *o_colors, Vector3 &t_cameraPos, float t_scale, u8 t_shouldBeBackfaceCulled)
{
    u32 result = 0;
    for (u32 i = 0; i < clump.geometryList.geometries[0].extension.materialSplit.splitInformation[splitIndex].faceIndex; i++)
        fillNextFace(o_vertices, o_normals, o_coordinates, o_colors, 0,
                     clump.geometryList.geometries[0].extension.materialSplit.splitInformation[splitIndex].vertexInformation[i].vertex1,
                     result++);

    return result;
}

void DffModel::fillNextFace(VECTOR *o_vertices, VECTOR *o_normals, VECTOR *o_coordinates, VECTOR *o_colors, u8 geometry, u32 t_getI, u32 t_setI)
{
    o_vertices[t_setI][0] = clump.geometryList.geometries[geometry].data.vertexInformation[t_getI].x;
    o_vertices[t_setI][1] = clump.geometryList.geometries[geometry].data.vertexInformation[t_getI].y;
    o_vertices[t_setI][2] = clump.geometryList.geometries[geometry].data.vertexInformation[t_getI].z;
    o_vertices[t_setI][3] = 1.0F;

    o_normals[t_setI][0] = clump.geometryList.geometries[geometry].data.normalInformation[t_getI].x;
    o_normals[t_setI][1] = clump.geometryList.geometries[geometry].data.normalInformation[t_getI].y;
    o_normals[t_setI][2] = clump.geometryList.geometries[geometry].data.normalInformation[t_getI].z;
    o_normals[t_setI][3] = 1.0F;

    o_coordinates[t_setI][0] = clump.geometryList.geometries[geometry].data.textureMappingInformation[t_getI].u;
    o_coordinates[t_setI][1] = 1.0F - clump.geometryList.geometries[geometry].data.textureMappingInformation[t_getI].v;
    o_coordinates[t_setI][2] = 1.0F;
    o_coordinates[t_setI][3] = 1.0F;

    o_colors[t_setI][0] = 1.0F;
    o_colors[t_setI][1] = 1.0F;
    o_colors[t_setI][2] = 1.0F;
    o_colors[t_setI][3] = 1.0F;
}
