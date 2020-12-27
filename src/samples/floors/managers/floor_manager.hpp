/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _FLOOR_MANAGER_
#define _FLOOR_MANAGER_

class Player; // Forward definition

#include <models/math/point.hpp>
#include <modules/texture_repository.hpp>
#include <modules/camera_base.hpp>
#include <models/audio_listener.hpp>

#include "../objects/floor.hpp"

/** Class which is maintaining floors objects */
class FloorManager
{

public:
    FloorManager(int t_floorAmount, TextureRepository *t_texRepo);
    ~FloorManager();
    Floor floors[144]; // Temp change it also in floors.cpp
    u16 floorAmount;
    void update(Player &t_player);
    void onAudioTick();
    Mesh **getMeshes() { return meshes; }

private:
    Mesh **meshes;
    TextureRepository *texRepo;
    u8 audioOffset, audioMode;
    u32 audioTick;
    float trick;
    u8 isTimeForChangeTriggerColor, isTimeForChangeDefaultColor;
    color_t trigColor, defaultColor;
    u8 trickMode;
    Point *spirals;
    void calcSpiral(int X, int Y);
    void doTheTrick();
    void initFloors();
};

#endif
