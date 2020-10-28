/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "../include/models/obj_model.hpp"

#include "../include/utils/debug.hpp"
#include "../include/utils/string.hpp"

// ----
// Constructors/Destructors
// ----

ObjModel::ObjModel(char *t_objFile)
{
    animState.startFrame = 0;
    animState.endFrame = 0;
    animState.interpolation = 0.0F;
    animState.animType = 0;
    animState.currentFrame = 0;
    animState.nextFrame = 0;
    animState.speed = 0.1F;
    filename = String::createWithoutExtension(t_objFile);
}

ObjModel::~ObjModel()
{
    if (isMemoryAllocated == true)
    {
        // TODO !!!!
        // delete[] vertices;
        // delete[] coordinates;
        // delete[] normals;
        // for (u16 i = 0; i < materialsCount; i++)
        // {
        // delete[] materials[i].verticeFaces;
        // delete[] materials[i].coordinateFaces;
        // delete[] materials[i].normalFaces;
        // delete[] materials[i].materialName;
        // }
        // delete[] materials;
    }
}

// ----
// Methods
// ----

u32 ObjModel::getFacesCount()
{
    u32 result = 0; // TODO
    for (u16 i = 0; i < frames[0].materialsCount; i++)
        result += frames[0].materials[i].facesCount;
    return result;
}

u32 ObjModel::getDrawData(u32 t_materialIndex, VECTOR *o_vertices, VECTOR *o_normals, VECTOR *o_coordinates, VECTOR *o_colors, Vector3 &t_cameraPos, float t_scale, u8 t_shouldBeBackfaceCulled)
{
    u32 addedFaces = 0;

    animState.interpolation += animState.speed; // TODO move it out
    if (animState.interpolation >= 1.0F)
    {
        animState.interpolation = 0.0F;
        animState.currentFrame = animState.nextFrame;
        if (++animState.nextFrame > animState.endFrame)
            animState.nextFrame = animState.startFrame;
    }

    for (u32 j = 0; j < frames[animState.currentFrame].materials[t_materialIndex].facesCount; j += 3)
    {
        for (u32 iVert = 0; iVert < 3; iVert++)
        {
            const ObjMaterial CURR_MAT = frames[animState.currentFrame].materials[t_materialIndex];
            const Vector3 CURR_VERT = frames[animState.currentFrame].vertices[CURR_MAT.verticeFaces[j + iVert]];
            if (animState.startFrame == animState.endFrame)
            {
                calc3Vectors[iVert].x = CURR_VERT.x * t_scale;
                calc3Vectors[iVert].y = CURR_VERT.y * t_scale;
                calc3Vectors[iVert].z = CURR_VERT.z * t_scale;
            }
            else
            {
                const Vector3 NEXT_VERT = frames[animState.nextFrame].vertices[CURR_MAT.verticeFaces[j + iVert]];
                calcVector.setByLerp(CURR_VERT, NEXT_VERT, animState.interpolation);
                calc3Vectors[iVert].x = calcVector.x * t_scale;
                calc3Vectors[iVert].y = calcVector.y * t_scale;
                calc3Vectors[iVert].z = calcVector.z * t_scale;
            }
        }
        if (!t_shouldBeBackfaceCulled || Vector3::shouldBeBackfaceCulled(&t_cameraPos, &calc3Vectors[2], &calc3Vectors[1], &calc3Vectors[0]) == 0)
            for (u32 iVert = 0; iVert < 3; iVert++)
            {
                const u32 CURR_FRAME = animState.currentFrame;
                o_vertices[addedFaces][0] = calc3Vectors[iVert].x;
                o_vertices[addedFaces][1] = calc3Vectors[iVert].y;
                o_vertices[addedFaces][2] = calc3Vectors[iVert].z;
                o_vertices[addedFaces][3] = 1.0F;

                o_normals[addedFaces][0] = frames[CURR_FRAME].normals[frames[CURR_FRAME].materials[t_materialIndex].normalFaces[j]].x;
                o_normals[addedFaces][1] = frames[CURR_FRAME].normals[frames[CURR_FRAME].materials[t_materialIndex].normalFaces[j]].y;
                o_normals[addedFaces][2] = frames[CURR_FRAME].normals[frames[CURR_FRAME].materials[t_materialIndex].normalFaces[j]].z;
                o_normals[addedFaces][3] = 1.0F;

                o_coordinates[addedFaces][0] = frames[CURR_FRAME].coordinates[frames[CURR_FRAME].materials[t_materialIndex].coordinateFaces[j]].x;
                o_coordinates[addedFaces][1] = frames[CURR_FRAME].coordinates[frames[CURR_FRAME].materials[t_materialIndex].coordinateFaces[j]].y;
                o_coordinates[addedFaces][2] = 1.0F;
                o_coordinates[addedFaces][3] = 1.0F;

                o_colors[addedFaces][0] = 1.0F;
                o_colors[addedFaces][1] = 1.0F;
                o_colors[addedFaces][2] = 1.0F;
                o_colors[addedFaces++][3] = 1.0F;
            }
        // fillNextFace(o_vertices, o_normals, o_coordinates, o_colors, t_materialIndex, j, addedFaces++);
    }
    return addedFaces;
}

void ObjModel::fillNextFace(VECTOR *o_vertices, VECTOR *o_normals, VECTOR *o_coordinates, VECTOR *o_colors, u32 t_materialI, u32 t_getI, u32 t_setI)
{
}
