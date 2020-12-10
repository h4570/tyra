/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Michał Mostowik <mostek3pl@gmail.com>
*/

#include "player.hpp"
#include "dolphin.hpp"

#include <loaders/obj_loader.hpp>
#include <loaders/bmp_loader.hpp>
#include <modules/gif_sender.hpp>
#include <utils/debug.hpp>
#include <utils/math.hpp>

Player::Player()
{
    mesh.loadMD2("dolphin/", "dolphin", 0.025F, true);
    mesh.shouldBeFrustumCulled = false;
    mesh.position.set(0.0F, 0.0F, 10.0f);
    mesh.rotation.x = -1.6F;
    mesh.setAnimSpeed(0.04F);
    bIsJumping = false;
    lifes = 3;
}

Player::~Player()
{
}

void Player::init(Engine *t_engine)
{
    pEngine = t_engine;
    waterImpact = t_engine->audio.loadADPCM("sound/water_impact.sad");
}

void Player::update(Pad &t_pad)
{
    if (lift > 0.0F)
        lift -= 0.05F;
    if (lift < 0.0F)
        lift += 0.05F;
    if (lift < 0.1F && lift > -0.1F)
        lift = 0;
    if (t_pad.lJoyV <= 100)
        velocity = 1;
    else if (t_pad.lJoyV >= 200)
        velocity = -1;

    if (t_pad.lJoyH >= 200)
        mesh.rotation.z -= 0.05;
    else if (t_pad.lJoyH <= 100)
        mesh.rotation.z += 0.05;

    if (t_pad.rJoyV >= 200 && lift <= 1)
        lift += 0.2F;
    else if (t_pad.rJoyV <= 100 && lift >= -1)
        lift -= 0.2F;

    if (t_pad.isCrossClicked && mesh.getCurrentAnimationFrame() <= 14 && WATER_LEVEL - 15 < mesh.position.y)
    {
        printf("Cross\n");
        mesh.setAnimSpeed(0.8F);
        mesh.playAnimation(14, 58, 0);
    }

    if (mesh.getCurrentAnimationFrame() > 25 && mesh.getCurrentAnimationFrame() < 47)
        bIsJumping = true;
    else
        bIsJumping = false;
    /*
    if (lift < 0)
        mesh.rotation.y = 1;
    else if (lift > 0)
        mesh.rotation.y = -1;
    else
        mesh.rotation.y = 0;
        */

    if (velocity > 0 && mesh.getCurrentAnimationFrame() == 0)
    {
        mesh.setAnimSpeed(0.05F);
        printf("Swim animation\n");
        mesh.playAnimation(1, 14, 0);
    }
    if (mesh.getCurrentAnimationFrame() == 42)
    {
        if (!bIsImpactingWater)
        {
            pEngine->audio.setADPCMVolume(50, 2);
            pEngine->audio.playADPCM(waterImpact, 2);
        }
        bIsImpactingWater = true;
    }
    else if (bIsImpactingWater)
    {
        bIsImpactingWater = false;
    }

    velocity *= 60.0F / Dolphin::engineFPS; //Moving by deltatime of last frame.
    mesh.position.z +=
        Math::cos(mesh.rotation.z) * velocity * (bIsJumping + 1);
    mesh.position.x +=
        Math::sin(mesh.rotation.z) * velocity * (bIsJumping + 1);
    mesh.position.y += lift;

    velocity = 0;
}
