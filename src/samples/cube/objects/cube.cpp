/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Wellington Carvalho <wellcoj@gmail.com>
*/

#include "cube.hpp"
#include <models/mesh.hpp>
#include <loaders/obj_loader.hpp>
#include <utils/debug.hpp>

// ----
// Constructors/Destructors
// ----

Cube::Cube( TextureRepository *t_texRepo )
{
    PRINT_LOG("Creating cube");
    texRepo = t_texRepo;
    speed = 1.2F;

    mesh.loadObj("meshes/cube/", "cube", 10.0F, true);
    mesh.position.set(20.00F, 40.00F, 10.00F);
    mesh.rotation.x = -1.566F;
    mesh.rotation.z = 1.566F;

    texRepo->addByMesh("meshes/cube/", mesh, PNG);

    PRINT_LOG("Cube object created!");
}

void Cube::update(const Pad &t_pad, const Camera &t_camera)
{
    rotate(t_pad);
    Vector3 *nextPos = getNextPosition(t_pad, t_camera);
}

Vector3 *Cube::getNextPosition(const Pad &t_pad, const Camera &t_camera)
{
    Vector3 *result = new Vector3(mesh.position);
    Vector3 normalizedCamera = Vector3(t_camera.unitCirclePosition);
    normalizedCamera.normalize();
    normalizedCamera *= speed;
    if (t_pad.lJoyV <= 100)
    {
        result->x += -normalizedCamera.x;
        result->z += -normalizedCamera.z;
    }
    else if (t_pad.lJoyV >= 200)
    {
        result->x += normalizedCamera.x;
        result->z += normalizedCamera.z;
    }
    if (t_pad.lJoyH <= 100)
    {
        result->x += -normalizedCamera.z;
        result->z += normalizedCamera.x;
    }
    else if (t_pad.lJoyH >= 200)
    {
        result->x += normalizedCamera.z;
        result->z += -normalizedCamera.x;
    }
    return result;
}

void Cube::rotate(const Pad &t_pad){
    //if (t_pad.rJoyH >= 200)
    //else if (t_pad.rJoyH <= 100)
        mesh.rotation.x += (0.01 * speed);
        mesh.rotation.y += (0.01 * speed);
        mesh.rotation.z += (0.01 * speed);
}