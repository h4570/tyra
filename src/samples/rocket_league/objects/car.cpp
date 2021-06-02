/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2021, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "car.hpp"

#define Z_ROTATION_STEP 0.005F

// ----
// Constructors/Destructors
// ----

Car::Car(const float &preZRotation, const float &fullZRotation)
    : Movable(preZRotation, fullZRotation),
      acceleration(0), maxForwardSpeed(7.0F), maxBackwardSpeed(-3.0F), speedStep(.1F) {}

Car::~Car() {}

// ----
// Methods
// ----

void Car::drive(const u8 &up, const u8 &down, const u8 &left, const u8 &right)
{
    rotate(left, right);
    move(up, down);
}

// ==================

// --- Movement start
void Car::move(const u8 &up, const u8 &down)
{
    updateSpeed(up, down);
    if (acceleration)
        updatePosition();
}

void Car::updateSpeed(const u8 &up, const u8 &down)
{
    float change = 0.0F;
    if ((down || up))
    {
        if (acceleration < maxForwardSpeed / 8)
            change = speedStep;
        else if (acceleration < maxForwardSpeed / 4)
            change = speedStep * 1.5F;
        else
            change = speedStep * 2;

        if (up && acceleration < maxForwardSpeed)
        {
            if (acceleration >= 0)
                acceleration += change;
            else
                acceleration += change * 2;
        }
        else if (down && acceleration > maxBackwardSpeed)
        {
            if (acceleration <= 0)
                acceleration -= change;
            else
                acceleration -= change * 2;
        }
    }
    else // No joypad shift
    {
        change = speedStep / 2;
        if (acceleration > 0)
            acceleration -= change;
        else if (acceleration < 0)
            acceleration += change;
        if (acceleration > -0.2F && acceleration < 0.2F)
            acceleration = 0;
    }
}

void Car::updatePosition()
{
    float zMove, xMove = 0;
    setZXMove(zMove, xMove);
    mesh.position.z -= zMove;
    mesh.position.x += xMove;
}

/**
 * We must ensure, that at the beginning, car is right in front of us
 * 
 * Start: RotPerc:  0%   -> Z: Speed   X: 0        | Down: Z: Decreasing, X: Increasing
 *        RotPerc:  25%  -> Z: 0       X: Speed    | ...          , X: Decreasing
 *        RotPerc:  50%  -> Z: -Speed  X: 0        | Z: Increasing, ...
 *        RotPerc:  75%  -> Z: 0       X: -Speed   | ...          , X: Increasing
 *        RotPerc:  100% -> Z: Speed   X: 0        | Stop = Start
 */
void Car::setZXMove(float &zMove, float &xMove)
{
    // Update z
    if (rotationPercentage >= 0 && rotationPercentage < 50) // Z: Decreasing
        zMove = acceleration - ((acceleration / 25) * rotationPercentage);
    else if (rotationPercentage >= 50 && rotationPercentage < 100) // Z: Increasing
        zMove = -acceleration + ((acceleration / 25) * (rotationPercentage - 50));

    // Update x
    if (rotationPercentage >= 0 && rotationPercentage < 25) // X: Increasing
        xMove = (acceleration / 25) * rotationPercentage;
    else if (rotationPercentage >= 25 && rotationPercentage < 75) // X: Decreasing
        xMove = acceleration - ((acceleration / 25) * (rotationPercentage - 25));
    else if (rotationPercentage >= 75 && rotationPercentage < 100) // X: Increasing
        xMove = -acceleration + ((acceleration / 25) * (rotationPercentage - 75));
}
// --- Movement end

// --- Rotation start
void Car::rotate(const u8 &left, const u8 &right)
{
    updateRotation(left, right);
    fixZRotation(mesh.rotation);
    updateRotationPercentage(mesh.rotation.z);
}

void Car::updateRotation(const u8 &left, const u8 &right)
{
    if ((left && acceleration >= 0) || (right && acceleration < 0))
        mesh.rotation.z += Z_ROTATION_STEP * abs(acceleration);
    else if ((right && acceleration >= 0) || (left && acceleration < 0))
        mesh.rotation.z -= Z_ROTATION_STEP * abs(acceleration);
}
// --- Rotation end
