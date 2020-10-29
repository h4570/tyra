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
        result += frames[0].materials[i].getFacesCount();
    return result;
}

void ObjModel::animate()
{
    animState.interpolation += animState.speed;
    if (animState.interpolation >= 1.0F)
    {
        animState.interpolation = 0.0F;
        animState.currentFrame = animState.nextFrame;
        if (++animState.nextFrame > animState.endFrame)
            animState.nextFrame = animState.startFrame;
    }
}

u32 ObjModel::getDrawData(u32 t_materialIndex, VECTOR *o_vertices, VECTOR *o_normals, VECTOR *o_coordinates, VECTOR *o_colors, Vector3 &t_cameraPos, float t_scale, u8 t_shouldBeBackfaceCulled)
{
#define CURR_FRAME frames[animState.currentFrame]
#define NEXT_FRAME frames[animState.nextFrame]
#define MATERIAL CURR_FRAME.materials[t_materialIndex]
#define CURR_VERT CURR_FRAME.vertices[MATERIAL.getVertexFace(matI + vertI)]
#define NEXT_VERT NEXT_FRAME.vertices[MATERIAL.getVertexFace(matI + vertI)]
    u32 addedFaces = 0;

    for (u32 matI = 0; matI < MATERIAL.getFacesCount(); matI += 3)
    {
        for (u32 vertI = 0; vertI < 3; vertI++)
            if (animState.startFrame == animState.endFrame)
                calc3Vectors[vertI].set(CURR_VERT * t_scale);
            else
                calc3Vectors[vertI].setByLerp(CURR_VERT, NEXT_VERT, animState.interpolation, t_scale);

        if (!t_shouldBeBackfaceCulled ||
            Vector3::shouldBeBackfaceCulled(&t_cameraPos, &calc3Vectors[2], &calc3Vectors[1], &calc3Vectors[0]))

            for (u32 vertI = 0; vertI < 3; vertI++)
            {
                o_vertices[addedFaces][0] = calc3Vectors[vertI].x;
                o_vertices[addedFaces][1] = calc3Vectors[vertI].y;
                o_vertices[addedFaces][2] = calc3Vectors[vertI].z;
                o_vertices[addedFaces][3] = 1.0F;

                o_normals[addedFaces][0] = CURR_FRAME.normals[MATERIAL.getNormalFace(matI + vertI)].x;
                o_normals[addedFaces][1] = CURR_FRAME.normals[MATERIAL.getNormalFace(matI + vertI)].y;
                o_normals[addedFaces][2] = CURR_FRAME.normals[MATERIAL.getNormalFace(matI + vertI)].z;
                o_normals[addedFaces][3] = 1.0F;

                o_coordinates[addedFaces][0] = CURR_FRAME.coordinates[MATERIAL.getStFace(matI + vertI)].x;
                o_coordinates[addedFaces][1] = CURR_FRAME.coordinates[MATERIAL.getStFace(matI + vertI)].y;
                o_coordinates[addedFaces][2] = 1.0F;
                o_coordinates[addedFaces][3] = 1.0F;

                o_colors[addedFaces][0] = 1.0F;
                o_colors[addedFaces][1] = 1.0F;
                o_colors[addedFaces][2] = 1.0F;
                o_colors[addedFaces++][3] = 1.0F;
            }
    }
    return addedFaces;
}
