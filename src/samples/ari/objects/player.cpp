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
    // PRINT_LOG("Creating player object");
    // this->gravity = 0.1F;
    // this->lift = -1.0F;
    // this->mesh.loadMD2("ari/", "ari", 0.0001F, true);
    // this->mesh.position.set(0.0F, 10.0F, 0.0F);
    // this->mesh.shouldBeBackfaceCulled = true;
    // this->mesh.shouldBeFrustumCulled = false;
    // this->mesh.shouldBeLighted = true;
    // this->mesh.setAnimSpeed(0.05F);
    // this->mesh.playAnimation(0, 1);
    PRINT_LOG("Player object created!");
}

Player::~Player()
{
}

// ----
// Methods
// ----
