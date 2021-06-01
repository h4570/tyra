/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2021, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "beach_ball.hpp"

#define FLOOR_Y 7.0F

// ----
// Constructors/Destructors
// ----

BeachBall::BeachBall(TextureRepository *t_texRepo)
    : gravity(0.1F), velocity(0), lift(-3.0F)
{
    consoleLog("Creating beach ball object");
    texRepo = t_texRepo;
    mesh.loadObj("meshes/ball/", "ball", .5F, true);
    mesh.position.set(20.00F, 20.00F, 0.00F);
    mesh.shouldBeBackfaceCulled = false;
    mesh.shouldBeFrustumCulled = true;
    texRepo->addByMesh("meshes/ball/", mesh, PNG);
    consoleLog("Beach ball object created!");
}

BeachBall::~BeachBall()
{
}

// ----
// Methods
// ----

void BeachBall::push(const Vector3 &car)
{
    Vector3 temp = mesh.position - car;
    temp.normalize();
    temp *= 100;
    force.set(temp);

    velocity = lift;
}

void BeachBall::update()
{
    updateGravity();

    if (mesh.position.y > FLOOR_Y)
    {
        const float change = 1.5F;
        if (force.x)
        {
            if (force.z > 0)
                mesh.position.x += change;
            if (force.z < 0)
                mesh.position.x -= change;
            if (force.x > -change && force.x < change)
                force.x = 0;
            else
                force.x -= change;
        }
        if (force.z)
        {
            if (force.z > 0)
                mesh.position.z += change;
            if (force.z < 0)
                mesh.position.z -= change;
            if (force.z > -change && force.z < change)
                force.z = 0;
            else
                force.z -= change;
        }
    }
    else
        force.set(0, 0, 0);
}

void BeachBall::updateGravity()
{
    this->velocity += this->gravity;
    this->mesh.position.y -= this->velocity;

    if (this->mesh.position.y >= 60.0F || mesh.position.y < -60.0F)
    {
        this->mesh.position.y = 60.0F;
        this->velocity = 0;
    }
    else if (this->mesh.position.y <= FLOOR_Y) // is on floor
    {
        this->mesh.position.y = FLOOR_Y;
        this->velocity = 0;
    }
}