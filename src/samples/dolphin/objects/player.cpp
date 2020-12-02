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

#include <loaders/obj_loader.hpp>
#include <loaders/bmp_loader.hpp>
#include <modules/gif_sender.hpp>
#include <utils/debug.hpp>
#include <utils/math.hpp>

Player::Player()
{
    mesh.loadMD2("dolphin/","dolphin",0.05F,true);
    mesh.shouldBeFrustumCulled = false;
    mesh.position.set(0.0F,20.0F,-10.0f);
    mesh.rotation.x = -1.6F;
    mesh.setAnimSpeed(0.05F);
}

Player::~Player()
{

}

void Player::update(Pad& t_pad)
{
    velocity=0;
    if(t_pad.lJoyV <= 100)
        velocity=1;
    else if(t_pad.lJoyV >= 200)
        velocity=-1;

    if(t_pad.lJoyH >= 200)
        mesh.rotation.z-=0.1;
    else if(t_pad.lJoyH <= 100)
        mesh.rotation.z+=0.1;

    if(velocity>0 && mesh.getCurrentAnimationFrame()==0)
        mesh.playAnimation(0, 14);

    mesh.position.z+=Math::cos(mesh.rotation.z)*velocity;
    mesh.position.x+=Math::sin(mesh.rotation.z)*velocity;

}