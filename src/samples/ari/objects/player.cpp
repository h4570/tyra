/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "player.hpp"

#include <loaders/obj_loader.hpp>
#include <loaders/bmp_loader.hpp>
#include <modules/gif_sender.hpp>
#include <utils/debug.hpp>
#include <utils/math.hpp>

// ----
// Constructors/Destructors
// ----

Player::Player()
{
    PRINT_LOG("Creating player object");
    this->gravity = 0.1F;
    this->lift = -1.0F;
    Vector3 initPos = Vector3(0.0F, 8.0F, 0.0F);
    // this->mesh.loadMD2("ari/", "ari.md2", initPos, 0.0001F);
    // this->mesh.shouldBeBackfaceCulled = true;
    // this->mesh.shouldBeFrustumCulled = false;
    // this->mesh.shouldBeLighted = true;
    // this->mesh.setAnimSpeed(0.05F);
    // this->mesh.playAnimation(0, 1);
    PRINT_LOG("Player object created!");
}

Player::~Player()
{
    delete[] this->spec;
}

// ----
// Methods
// ----
