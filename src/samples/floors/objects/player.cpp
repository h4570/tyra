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

#include <loaders/md2_loader.hpp>
#include <loaders/bmp_loader.hpp>
#include <modules/gif_sender.hpp>
#include <utils/debug.hpp>
#include <utils/math.hpp>

// ----
// Constructors/Destructors
// ----

Player::Player(Audio *t_audio)
{
    PRINT_LOG("Creating player object");
    audio = t_audio;
    gravity = 0.1F;
    lift = -1.0F;
    jumpCounter = 0;
    killedEnemies = 0;
    isWalking = false;
    isFighting = false;
    isWalkingAnimationSet = false;
    isJumpingAnimationSet = false;
    isFightingAnimationSet = false;

    mesh.loadMD2("warrior/", "warrior", 0.2F, true);
    mesh.position.set(0.00F, 40.00F, 0.00F);
    mesh.rotation.x = -1.566F;
    mesh.rotation.z = 1.566F;
    mesh.shouldBeBackfaceCulled = false;
    mesh.shouldBeFrustumCulled = false;
    mesh.setAnimSpeed(0.17F);
    mesh.playAnimation(0, 0);

    walkAdpcm = audio->loadADPCM("walk.adpcm");
    jumpAdpcm = audio->loadADPCM("jump.adpcm");
    audio->setADPCMVolume(60, 0);

    PRINT_LOG("Player object created!");
}

Player::~Player()
{
}

// ----
// Methods
// ----

/** Update player position and control gravity */
void Player::update(const Pad &t_pad, const Camera &t_camera, const FloorManager &floorManager, const Enemy &enemy)
{
    Vector3 *nextPos = getNextPosition(t_pad, t_camera);
    FloorsCheck *floorsCheck = checkFloors(floorManager, *nextPos);
    if (floorsCheck->currentFloor != NULL)
        indexOfCurrentFloor = floorsCheck->currentFloor->initOffset;
    updatePosition(t_pad, t_camera, floorManager, *floorsCheck, *nextPos, enemy);
    updateGravity(floorsCheck);
    delete floorsCheck;
    delete nextPos;
}

Vector3 *Player::getNextPosition(const Pad &t_pad, const Camera &t_camera)
{
    Vector3 *result = new Vector3(mesh.position);
    Vector3 normalizedCamera = Vector3(t_camera.unitCirclePosition);
    normalizedCamera.normalize();
    if (t_pad.lJoyV <= 100)
    {
        result->x += -normalizedCamera.x;
        result->z += -normalizedCamera.z;
    }
    else if (t_pad.lJoyV >= 200)
    {
        result->x += normalizedCamera.x;
        result->z += normalizedCamera.z;
    }
    if (t_pad.lJoyH <= 100)
    {
        result->x += -normalizedCamera.z;
        result->z += normalizedCamera.x;
    }
    else if (t_pad.lJoyH >= 200)
    {
        result->x += normalizedCamera.z;
        result->z += -normalizedCamera.x;
    }
    return result;
}

/** Move player when pad move buttons are pressed and is not blocked by any floor side */
void Player::updatePosition(const Pad &t_pad, const Camera &t_camera, const FloorManager &t_floorManager, const FloorsCheck &t_floorsCheck, const Vector3 &t_nextPos, const Enemy &enemy)
{
    if (t_pad.rJoyH >= 200)
        mesh.rotation.z += 0.08;
    else if (t_pad.rJoyH <= 100)
        mesh.rotation.z -= 0.08;

    isWalking = mesh.position.x != t_nextPos.x || mesh.position.z != t_nextPos.z;
    isFighting = isFighting || t_pad.isCircleClicked;

    if (isFighting)
    {
        if (!isFightingAnimationSet)
        {
            isFightingAnimationSet = true;
            mesh.playAnimation(8, 19, 0);
            fightTimer.prime();
        }
        if (fightTimer.getTimeDelta() > 25000)
        { // end of fighting
            float distance = getPosition().distanceTo(enemy.getPosition());
            if (12.0F > distance)
            {
                enemy.kill(*this);
                killedEnemies++;
            }
            isFightingAnimationSet = false;
            isFighting = false;
        }
    }
    else if (isWalking)
    {
        if (!isWalkingAnimationSet)
        {
            isWalkingAnimationSet = true;
            this->mesh.playAnimation(1, 8);
        }
        if (walkTimer.getTimeDelta() > 8000)
        {
            walkTimer.prime();
            audio->playADPCM(walkAdpcm, 0);
        }
    }
    else
    {
        isWalkingAnimationSet = false;
        mesh.playAnimation(0, 0);
    }

    if (t_floorsCheck.willCollideFloor == NULL && !isFighting)
    {
        mesh.position.x = t_nextPos.x;
        mesh.position.z = t_nextPos.z;
    }

    if (t_pad.isCrossClicked == 1)
    {
        velocity = lift;
        audio->playADPCM(jumpAdpcm);
        jumpCounter++;
    }
}

/** 
 * Do not call this.
 * This is called by Floor::animate() on player's floor move in Y direction
 */
void Player::onBeforePlayerFloorMove(Floor *t_floor, float &t_newY)
{
    if (t_floor->animDirection == 0 && t_newY > 0.0F)
        this->mesh.position.y += t_newY * 1.05F;
    else if ((t_floor->animDirection == 1 && -t_newY < 0.0F))
        this->mesh.position.y -= -t_newY * 1.05F;
}

/** Update player position by gravity and update index of current floor */
void Player::updateGravity(FloorsCheck *t_floorsCheck)
{
    this->velocity += this->gravity;
    this->mesh.position.y -= this->velocity;
    u8 isOnFloor = t_floorsCheck->currentFloor != NULL && mesh.position.y < t_floorsCheck->currFloorMax.y;

    if (this->mesh.position.y >= 60.0F || mesh.position.y < -60.0F)
    {
        this->mesh.position.y = 60.0F;
        this->velocity = 0;
    }
    else if (isOnFloor)
    {
        this->mesh.position.y = t_floorsCheck->currFloorMax.y;
        this->velocity = 0;
    }
}

// ---
// Private
// ---

FloorsCheck *Player::checkFloors(const FloorManager &t_floorManager, const Vector3 &t_nextPos)
{
    FloorsCheck *result = new FloorsCheck;
    result->currentFloor = NULL;
    result->willCollideFloor = NULL;
    Vector3 min = Vector3();
    Vector3 max = Vector3();
    for (u32 i = 0; i < t_floorManager.floorAmount; i++)
    {
        getMinMax(t_floorManager.floors[i].mesh, min, max);
        if (result->currentFloor == NULL && this->mesh.position.isOnSquare(min, max))
        {
            result->currentFloor = &t_floorManager.floors[i];
            result->currFloorMin.set(min);
            result->currFloorMax.set(max);
        }
        if (result->willCollideFloor == NULL && t_nextPos.collidesSquare(min, max))
        {
            result->willCollideFloor = &t_floorManager.floors[i];
            result->willCollideFloorMin.set(min);
            result->willCollideFloorMax.set(max);
        }
        if (result->currentFloor != NULL && result->willCollideFloor != NULL)
            break;
    }
    return result;
}

/** Calculates minimum and maximum X, Y, Z of mesh vertices + current position */
void Player::getMinMax(const Mesh &t_mesh, Vector3 &t_min, Vector3 &t_max)
{
    Vector3 calc = Vector3();
    u8 isInitialized = 0;
    Vector3 *boundingBox = t_mesh.getCurrentBoundingBox();
    for (u32 i = 0; i < 8; i++)
    {
        calc.set(
            boundingBox[i].x + t_mesh.position.x,
            boundingBox[i].y + t_mesh.position.y,
            boundingBox[i].z + t_mesh.position.z);
        if (isInitialized == 0)
        {
            isInitialized = 1;
            t_min.set(calc);
            t_max.set(calc);
        }

        if (t_min.x > calc.x)
            t_min.x = calc.x;
        if (calc.x > t_max.x)
            t_max.x = calc.x;

        if (t_min.y > calc.y)
            t_min.y = calc.y;
        if (calc.y > t_max.y)
            t_max.y = calc.y;

        if (t_min.z > calc.z)
            t_min.z = calc.z;
        if (calc.z > t_max.z)
            t_max.z = calc.z;
    }
}
