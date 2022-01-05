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

#define PLAYER_PRE_Z_ROTATION -3.133F
#define PLAYER_PRE_X_ROTATION -1.566F

// ----
// Constructors/Destructors
// ----

Player::Player(TextureRepository *t_texRepo)
    : Car(PLAYER_PRE_Z_ROTATION, PLAYER_PRE_Z_ROTATION * 2)
{
    consoleLog("Creating player object");
    texRepo = t_texRepo;
    mesh.loadMD2("meshes/car/", "car", 0.2F, true);
    mesh.rotation.x = PLAYER_PRE_X_ROTATION;
    mesh.rotation.z = PLAYER_PRE_Z_ROTATION;
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

void Player::update(const Pad &t_pad)
{
    drive(t_pad.lJoyV <= 100, t_pad.lJoyV >= 200, t_pad.lJoyH <= 100, t_pad.lJoyH >= 200);
}
