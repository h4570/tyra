/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "enemy.hpp"
#include "../utils.hpp"

#include <loaders/md_loader.hpp>
#include <loaders/bmp_loader.hpp>
#include <modules/gif_sender.hpp>
#include <utils/debug.hpp>
#include <utils/math.hpp>
#include <stdlib.h>

// ----
// Constructors/Destructors
// ----

Enemy::Enemy(TextureRepository *t_texRepo)
{
    consoleLog("Creating enemy object");

    meshes = new Mesh[getMeshesCount()];
    drawMeshes = new Mesh *[getMeshesCount()];

    meshes[0].loadMD2("meshes/enemy/", "poss_head", 0.3F, false);
    meshes[1].loadMD2("meshes/enemy/", "poss_body", 0.3F, false);
    meshes[2].loadMD2("meshes/enemy/", "poss_weapon", 0.3F, false);

    for (u8 i = 0; i < 3; i++)
    {
        meshes[i].rotation.x = -1.566F;
        meshes[i].rotation.z = 1.566F;
        meshes[i].shouldBeFrustumCulled = false;
        t_texRepo->addByMesh("meshes/enemy/", meshes[i], PNG);
        meshes[i].playAnimation(1, 6);
        drawMeshes[i] = &meshes[i];
    }

    Vector3 initPos = Vector3(0.00F, 40.00F, 0.00F);
    setPosition(initPos);

    isKilled = true;

    consoleLog("Enemy object created!");
}

Enemy::~Enemy()
{
    delete[] meshes;
}

// ----
// Methods
// ----

void Enemy::setPosition(const Vector3 &t_vec)
{
    position.set(t_vec);
    for (u8 i = 0; i < getMeshesCount(); i++)
        meshes[i].position.set(t_vec);
}

void Enemy::setPosition(const float &t_y)
{
    position.y = t_y;
    for (u8 i = 0; i < getMeshesCount(); i++)
        meshes[i].position.y = t_y;
}

/** Called by player class. */
void Enemy::kill(const Player &player)
{
    isKilled = true;
}

void Enemy::update(const FloorManager &floorManager)
{
    if (isKilled)
    {
        isKilled = false;
        int randFloor = rand() % (floorManager.floorAmount + 1);
        currentFloor = &floorManager.floors[randFloor];
        setPosition(currentFloor->mesh.position);
    }
    updateGravity();
}

void Enemy::updateGravity()
{
    Utils::getMinMax(currentFloor->mesh, currFloorMin, currFloorMax);
    setPosition(currFloorMax.y);
}
