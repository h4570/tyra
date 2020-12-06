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

#include <loaders/md2_loader.hpp>
#include <loaders/bmp_loader.hpp>
#include <modules/gif_sender.hpp>
#include <utils/debug.hpp>
#include <utils/math.hpp>

// ----
// Constructors/Destructors
// ----

Enemy::Enemy(TextureRepository *t_texRepo)
{
    PRINT_LOG("Creating enemy object");

    meshes = new Mesh[getMeshesCount()];

    meshes[0].loadMD2("poss/", "poss_head", 0.3F, false);
    meshes[0].position.set(0.00F, 40.00F, 0.00F);
    meshes[0].rotation.x = -1.566F;
    meshes[0].rotation.z = 1.566F;

    meshes[1].loadMD2("poss/", "poss_body", 0.3F, false);
    meshes[1].position.set(0.00F, 40.00F, 0.00F);
    meshes[1].rotation.x = -1.566F;
    meshes[1].rotation.z = 1.566F;

    meshes[2].loadMD2("poss/", "poss_weapon", 0.3F, false);
    meshes[2].position.set(0.00F, 40.00F, 0.00F);
    meshes[2].rotation.x = -1.566F;
    meshes[2].rotation.z = 1.566F;

    t_texRepo->addByMesh("poss/", meshes[0], PNG);
    t_texRepo->addByMesh("poss/", meshes[1], PNG);
    t_texRepo->addByMesh("poss/", meshes[2], PNG);

    meshes[0].playAnimation(1, 6);
    meshes[1].playAnimation(1, 6);
    meshes[2].playAnimation(1, 6);

    PRINT_LOG("Enemy object created!");
}

Enemy::~Enemy()
{
    delete[] meshes;
}

// ----
// Methods
// ----

/** Called by player class. */
void Enemy::kill(const Player &player) const
{
    printf("killed!\n");
}

void Enemy::update(const FloorManager &floorManager)
{
}
