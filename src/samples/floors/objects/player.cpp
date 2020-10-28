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

    Vector3 initPos = Vector3(0.00F, 20.00F, 0.00F);
    this->mesh.loadMD2("warrior/", "warrior.md2", initPos, 0.2F);
    this->mesh.rotation.x = -1.6F;
    this->mesh.shouldBeBackfaceCulled = false;
    this->mesh.shouldBeFrustumCulled = false;

    this->mesh.setAnimSpeed(0.2F);
    this->mesh.playAnimation(0, 24);
    PRINT_LOG("Player object created!");
}

Player::~Player()
{
    delete[] this->spec;
}

// ----
// Methods
// ----

/** Update player position and control gravity */
void Player::update(Pad &pad, Camera &camera, FloorManager &floorManager)
{
    this->updatePosition(pad, camera, floorManager);
    this->updateGravity(pad, floorManager);
}

/** Move player when pad move buttons are pressed and is not blocked by any floor side */
void Player::updatePosition(Pad &pad, Camera &camera, FloorManager &floorManager)
{

    if (pad.isDpadUpPressed == 1)
    {
        this->playerNextPosition.x = this->mesh.position.x + 0.01F * -camera.unitCirclePosition.x;
        this->playerNextPosition.z = this->mesh.position.z + 0.01F * -camera.unitCirclePosition.z;
    }
    if (pad.isDpadDownPressed == 1)
    {
        this->playerNextPosition.x = this->mesh.position.x + 0.01F * camera.unitCirclePosition.x;
        this->playerNextPosition.z = this->mesh.position.z + 0.01F * camera.unitCirclePosition.z;
    }
    if (pad.isDpadLeftPressed == 1)
    {
        this->playerNextPosition.x = this->mesh.position.x + 0.01F * -camera.unitCirclePosition.z;
        this->playerNextPosition.z = this->mesh.position.z + 0.01F * camera.unitCirclePosition.x;
    }
    if (pad.isDpadRightPressed == 1)
    {
        this->playerNextPosition.x = this->mesh.position.x + 0.01F * camera.unitCirclePosition.z;
        this->playerNextPosition.z = this->mesh.position.z + 0.01F * -camera.unitCirclePosition.x;
    }
    this->playerNextPosition.y = this->mesh.position.y;
    this->isCollideFloor = 0;
    Vector3 min = Vector3();
    Vector3 max = Vector3();
    for (int i = 0; i < floorManager.floorAmount; i++)
    {
        floorManager.floors[i].mesh.getMinMax(&min, &max);
        this->isCollideFloor = this->playerNextPosition.collidesSquare(min, max);
        if (this->isCollideFloor == 1)
            break;
    }
    if (this->isCollideFloor == 0)
    {
        this->mesh.position.x = this->playerNextPosition.x;
        this->mesh.position.z = this->playerNextPosition.z;
    }
}

/** Do not call this.
 * This is called by Floor::animate() on player's floor move in Y direction
 */
void Player::onBeforePlayerFloorMove(Floor *floor, float &newY)
{
    if (floor->animDirection == 0 && newY > 0.0F)
        this->mesh.position.y += newY * 1.05F;
    else if ((floor->animDirection == 1 && -newY < 0.0F))
        this->mesh.position.y -= -newY * 1.05F;
}

/** Update player position by gravity and update index of current floor */
void Player::updateGravity(Pad &pad, FloorManager &floorManager)
{
    Vector3 min = Vector3();
    Vector3 max = Vector3();

    if (pad.isCrossClicked == 1)
        this->velocity = this->lift;

    this->isOnFloor = 0;
    for (int i = 0; i < floorManager.floorAmount; i++)
    {
        floorManager.floors[i].mesh.getMinMax(&min, &max);
        this->isOnFloor = this->mesh.position.isOnSquare(min, max);
        if (this->isOnFloor == 1)
        {
            this->indexOfCurrentFloor = floorManager.floors[i].initOffset;
            break;
        }
    }

    this->velocity += this->gravity;
    this->mesh.position.y -= this->velocity;

    if (this->mesh.position.y >= 60.0F)
    {
        this->mesh.position.y = 60.0F;
        this->velocity = 0;
    }
    else if ((this->mesh.position.y) < max.y && this->isOnFloor)
    {
        this->mesh.position.y = max.y;
        this->velocity = 0;
    }
}
