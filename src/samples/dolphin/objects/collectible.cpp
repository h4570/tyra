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
    bDisappearing = false;
}

Collectible::~Collectible()
{
}

void Collectible::disappear()
{
    bDisappearing = true;
    lift = 10;
}

void Collectible::update()
{
    mesh.rotation.y += 0.08F;
    if (bDisappearing)
    {
        mesh.position.y += lift;
        lift = lift > 0 ? lift / 2 : 0;
        if (lift < 1)
        {
            setActive(false);
            bDisappearing = false;
        }
    }
}
