/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2021, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "player.hpp"

// ----
// Constructors/Destructors
// ----

Player::Player(TextureRepository *t_texRepo)
{
    consoleLog("Creating player object");
    texRepo = t_texRepo;
    gravity = 0.1F;
    lift = -1.0F;
    mesh.loadMD2("meshes/car/", "car", 0.2F, true);
    mesh.position.set(0.00F, 40.00F, 0.00F);
    mesh.rotation.x = -1.566F;
    mesh.rotation.z = 1.566F;
    mesh.shouldBeBackfaceCulled = false;
    mesh.shouldBeFrustumCulled = false;
    texRepo->addByMesh("meshes/car/", mesh, BMP);
    consoleLog("Player object created!");
}

Player::~Player()
{
}

// ----
// Methods
// ----

void Player::update(const Pad &t_pad, const Camera &t_camera)
{
    updatePosition(t_pad, t_camera);
    updateGravity();
}

void Player::updatePosition(const Pad &t_pad, const Camera &t_camera)
{
    if (t_pad.rJoyH >= 200)
        mesh.rotation.z += 0.08;
    else if (t_pad.rJoyH <= 100)
        mesh.rotation.z -= 0.08;

    if (t_pad.isCrossClicked)
    {
        velocity = lift;
    }
}

/** Update player position by gravity and update index of current floor */
void Player::updateGravity()
{
    this->velocity += this->gravity;
    this->mesh.position.y -= this->velocity;
    u8 isOnFloor = mesh.position.y <= 0.0F;

    if (this->mesh.position.y >= 60.0F || mesh.position.y < -60.0F)
    {
        this->mesh.position.y = 60.0F;
        this->velocity = 0;
    }
    else if (isOnFloor)
    {
        this->mesh.position.y = 0.0F;
        this->velocity = 0;
    }
}
