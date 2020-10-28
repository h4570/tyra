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
#include <models/obj_model.hpp>
#include <models/mesh_spec.hpp>
#include <modules/camera_base.hpp>
#include <models/audio_listener.hpp>

#include "../objects/floor.hpp"

/** Class which is maintaining floors objects */
class FloorManager : public AudioListener
{

public:
    FloorManager(int t_floorAmount);
    ~FloorManager();
    Floor floors[64]; // Temp change it also in floors.cpp
    int floorAmount;
    void update(Player &t_player);
    Mesh **meshes;
    void onAudioTick();
    MeshSpec *spec;

private:
    u8 audioOffset, audioMode;
    u32 audioTick;
    ObjModel *obj;
    Point *spirals;
    void calcSpiral(int X, int Y);
    void initFloors();
};

#endif
