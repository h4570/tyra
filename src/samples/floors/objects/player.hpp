/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _PLAYER_
#define _PLAYER_

#include "../managers/floor_manager.hpp"
#include "../camera.hpp"
#include "./enemy.hpp"
#include <modules/pad.hpp>
#include <modules/timer.hpp>
#include <tamtypes.h>
#include <modules/audio.hpp>

struct FloorsCheck
{
    const Floor *currentFloor, *willCollideFloor;
    Vector3 currFloorMin, willCollideFloorMin, currFloorMax, willCollideFloorMax;
};

/** Player 3D object class  */
class Player
{

public:
    float gravity, velocity, lift;
    Mesh mesh;
    s16 indexOfCurrentFloor;
    Player(Audio *t_audio, TextureRepository *t_texRepo);
    ~Player();

    void update(const Pad &t_pad, const Camera &t_camera, const FloorManager &t_floorManager, Enemy &enemy);
    void onBeforePlayerFloorMove(Floor *t_floor, float &t_newY);
    const inline u32 &getJumpCount() const { return jumpCounter; }
    const inline u32 &getKilledEnemiesCount() const { return killedEnemies; }
    inline const Vector3 &getPosition() const { return mesh.position; }

private:
    TextureRepository *texRepo;
    u32 jumpCounter, killedEnemies;
    Vector3 *getNextPosition(const Pad &t_pad, const Camera &t_camera);
    FloorsCheck *checkFloors(const FloorManager &t_floorManager, const Vector3 &t_nextPos);
    u8 isWalkingAnimationSet, isJumpingAnimationSet, isFightingAnimationSet;
    u8 isFighting, isWalking;
    Audio *audio;
    Timer walkTimer, fightTimer;
    audsrv_adpcm_t *walkAdpcm, *jumpAdpcm, *boomAdpcm;
    float speed;
    void getMinMax(const Mesh &t_mesh, Vector3 &t_min, Vector3 &t_max);
    void updatePosition(const Pad &t_pad, const Camera &t_camera, const FloorManager &t_floorManager, const FloorsCheck &t_floorsCheck, const Vector3 &nextPos, Enemy &enemy);
    void updateGravity(FloorsCheck *t_floorsCheck);
};

#endif
