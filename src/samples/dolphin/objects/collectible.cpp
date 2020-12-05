/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Michał Mostowik <mostek3pl@gmail.com>
*/

#include "collectible.hpp"

#include <loaders/obj_loader.hpp>
#include <loaders/bmp_loader.hpp>
#include <modules/gif_sender.hpp>
#include <utils/debug.hpp>
#include <utils/math.hpp>

Collectible::Collectible()
{
    mesh.loadObj("oyster/", "oyster", 10.F, false);
    mesh.shouldBeFrustumCulled = false;
    mesh.position.set(0.0F, 0.0F, 0.0f);
    bActive = true;
}

Collectible::~Collectible()
{
}

void Collectible::setActive(u8 b)
{
    bActive = b;
}

u8 Collectible::isActive()
{
    return bActive;
}