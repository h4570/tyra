/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "../include/models/md2_model.hpp"

#include "../include/utils/anorms.hpp"
#include "../include/utils/string.hpp"
#include "../include/utils/debug.hpp"

// ----
// Constructors/Destructors
// ----

MD2Model::MD2Model(char *t_md2File)
{
    filename = String::createWithoutExtension(t_md2File);

    verticesPerFrameCount = 0;
    coordinatesCount = 0;
    trianglesCount = 0;
    framesCount = 0;

    animState.startFrame = 0;
    animState.endFrame = 0;
    animState.interpolation = 0.0F;
    animState.animType = 0;
    animState.currentFrame = 0;
    animState.nextFrame = 0;
    animState.speed = 0.1F;

    vertices = 0;
    coordinates = 0;
    normalIndexes = 0;
    triangles = 0;
}

MD2Model::~MD2Model()
{
    delete[] vertices;
    delete[] normalIndexes;
    delete[] coordinates;
    delete[] triangles;
}

// ----
// Methods
// ----

u32 MD2Model::getCurrentFrameData(VECTOR *o_vertices, VECTOR *o_normals, VECTOR *o_coordinates, Vector3 &t_cameraPos, float t_scale, u8 t_shouldBeBackfaceCulled)
{
    u32 i = 0;
    animState.interpolation += animState.speed;
    if (animState.interpolation >= 1.0F)
    {
        animState.interpolation = 0.0F;
        animState.currentFrame = animState.nextFrame;
        if (++animState.nextFrame > animState.endFrame)
            animState.nextFrame = animState.startFrame;
    }
    for (u32 iTri = 0; iTri < trianglesCount; iTri++)
    {
        for (u32 iVert = 0; iVert < 3; iVert++)
        {
            const u32 CURR_VERTICE =
                triangles[iTri].verticeIndexes[iVert] +
                (verticesPerFrameCount * animState.currentFrame);

            if (animState.startFrame == animState.endFrame)
            {
                calc3Vectors[iVert].x = vertices[CURR_VERTICE].x * t_scale;
                calc3Vectors[iVert].y = vertices[CURR_VERTICE].y * t_scale;
                calc3Vectors[iVert].z = vertices[CURR_VERTICE].z * t_scale;
            }
            else
            {
                const u32 NEXT_VERTICE =
                    triangles[iTri].verticeIndexes[iVert] +
                    (verticesPerFrameCount * animState.nextFrame);
                calc3Vectors[iVert].setByLerp(vertices[CURR_VERTICE], vertices[NEXT_VERTICE], animState.interpolation, t_scale);
            }
        }
        if (!t_shouldBeBackfaceCulled || Vector3::shouldBeBackfaceCulled(&t_cameraPos, &calc3Vectors[2], &calc3Vectors[1], &calc3Vectors[0]) == 0)
            for (u32 iVert = 0; iVert < 3; iVert++)
            {
                const u32 CURR_VERTICE =
                    triangles[iTri].verticeIndexes[iVert] +
                    (verticesPerFrameCount * animState.currentFrame);

                const u32 CURR_COORD = triangles[iTri].coordIndexes[iVert];

                o_vertices[i][0] = calc3Vectors[iVert].x;
                o_vertices[i][1] = calc3Vectors[iVert].y;
                o_vertices[i][2] = calc3Vectors[iVert].z;
                o_vertices[i][3] = 1.0F;

                o_normals[i][0] = ANORMS[normalIndexes[CURR_VERTICE]][0];
                o_normals[i][1] = ANORMS[normalIndexes[CURR_VERTICE]][1];
                o_normals[i][2] = ANORMS[normalIndexes[CURR_VERTICE]][2];
                o_normals[i][3] = 1.0F;

                o_coordinates[i][0] = coordinates[CURR_COORD].x;
                o_coordinates[i][1] = 1.0F - coordinates[CURR_COORD].y;
                o_coordinates[i][2] = 1.0F;
                o_coordinates[i][3] = 1.0F;
                i++;
            }
    }
    return i;
}

void MD2Model::allocateMemory()
{
    vertices = new Vector3[verticesPerFrameCount * framesCount];
    normalIndexes = new u32[verticesPerFrameCount * framesCount];
    coordinates = new Point[coordinatesCount];
    triangles = new MD2Triangle[trianglesCount];
}
