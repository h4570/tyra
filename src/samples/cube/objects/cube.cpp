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
    speed = 1.5F;

    mesh.loadObj("meshes/cube/", "cube", 6.0F, true);
    mesh.position.set(20.00F, 40.00F, 10.00F);
    mesh.rotation.x = -1.566F;
    mesh.rotation.z = 1.566F;

    texRepo->addByMesh("meshes/cube/", mesh, PNG);

    PRINT_LOG("Cube object created!");
}

void Cube::update(const Pad &t_pad, const Camera &t_camera)
{
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