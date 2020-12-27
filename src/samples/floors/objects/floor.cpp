/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "floor.hpp"

#include "../utils.hpp"
#include "./player.hpp"
#include <cstdlib>
#include <time.h>

const int FLOOR_ANIMATION_LENGTH = 100;
const float FLOOR_ANIMATION_Y_MIN = 0.0F;
const float FLOOR_ANIMATION_Y_MAX = 10.0F;

// ----
// Constructors/Destructors
// ----

Floor::Floor()
{
    animTimer = 0;
    animDirection = 0;
    initOffset = 0;
    isByAudioTriggered = false;
}

Floor::~Floor() {}

// ----
// Methods
// ----

void Floor::init(const Mesh &mother, const Point &spiral, const u8 &initOffset)
{
    this->mesh.position.set(0.00F, -10.00F, 0.00F);
    this->mesh.loadFrom(mother);
    this->initOffset = initOffset;
    this->animTimer = rand() % FLOOR_ANIMATION_LENGTH;
    this->mesh.shouldBeLighted = true;
    this->mesh.shouldBeFrustumCulled = true;
    this->mesh.position.x = spiral.x * 80.0F;
    this->mesh.position.z = spiral.y * 80.0F;
}

/** Check and update animation direction and position
 * Runs Player::onBeforePlayerFloorMove() method
 */
void Floor::animate(Player &player)
{
    if (this->animDirection == 0)
        this->animTimer++;
    else
        this->animTimer--;

    if (this->animDirection == 0 && this->animTimer >= FLOOR_ANIMATION_LENGTH)
        this->animDirection = 1;
    else if (this->animDirection == 1 && this->animTimer <= 0)
        this->animDirection = 0;
    float newY = Utils::expoEaseInOut(this->animTimer, FLOOR_ANIMATION_Y_MIN, FLOOR_ANIMATION_Y_MAX, FLOOR_ANIMATION_LENGTH);
    float yDiff = newY - this->mesh.position.y;
    if (this->initOffset == player.indexOfCurrentFloor)
        player.onBeforePlayerFloorMove(this, yDiff);
    this->mesh.position.y = newY;
}
