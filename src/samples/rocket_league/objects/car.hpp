/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2021, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _CAR_
#define _CAR_

#include "movable.hpp"
#include <tamtypes.h>
#include <models/mesh.hpp>
#include <modules/pad.hpp>
#include <utils/math.hpp>

class Car : public Movable
{

public:
    Car(const float &preZRotation, const float &fullZRotation);
    ~Car();
    Mesh mesh;
    const float &getAcceleration() const { return acceleration; };
    float getAbsAcceleration() const { return abs(acceleration); };

protected:
    float acceleration;

    void drive(const u8 &up, const u8 &down, const u8 &left, const u8 &right);
    const float &getMaxForwardSpeed() const { return maxForwardSpeed; };
    const float &getMaxBackwardSpeed() const { return maxBackwardSpeed; };
    const float &getSpeedStep() const { return speedStep; };

    void setMaxForwardSpeed(const float &b) { maxForwardSpeed = b; };
    void setMaxBackwardSpeed(const float &b) { maxBackwardSpeed = b; };
    void setSpeedStep(const float &b) { speedStep = b; };

private:
    float maxForwardSpeed, maxBackwardSpeed, speedStep;

    void move(const u8 &up, const u8 &down);
    void updateSpeed(const u8 &up, const u8 &down);
    void updatePosition();
    void setZXMove(float &zMove, float &xMove);

    void rotate(const u8 &left, const u8 &right);
    void updateRotation(const u8 &left, const u8 &right);
};

#endif
