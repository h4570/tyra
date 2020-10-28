/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "floor_manager.hpp"

#include <utils/math.hpp>
#include <utils/debug.hpp>
#include <loaders/obj_loader.hpp>

// ----
// Constructors/Destructors
// ----

/** Calculate square spiral offsets and initialize floors
 * @param floorAmount must be a number from square root table
 */
FloorManager::FloorManager(int t_floorAmount)
{
    floorAmount = t_floorAmount;
    spirals = new Point[t_floorAmount];
    int floorSpiralMaxOffset = (int)Math::sqrt(t_floorAmount);
    calcSpiral(floorSpiralMaxOffset, floorSpiralMaxOffset);
    initFloors();
    audioOffset = audioMode = audioTick = 0;
}

FloorManager::~FloorManager()
{
    delete[] spirals;
    delete[] spec;
    delete[] obj;
    delete[] meshes;
}

// ----
// Methods
// ----

/** Calculates square spiral offsets by specyfying max X and Y, fills "spirals" variable 
 * @param X Max X offset off spiral
 * @param Y Max Y offset off spiral
*/
void FloorManager::calcSpiral(int X, int Y)
{
    PRINT_LOG("Calculating square spiral for floors");
    int x, y, dx;
    x = y = dx = 0;
    int dy = -1;
    int t = X > Y ? X : Y;
    int maxI = t * t;
    for (int i = 0; i < maxI; i++)
    {
        if ((-X / 2 <= x) && (x <= X / 2) && (-Y / 2 <= y) && (y <= Y / 2))
        {
            spirals[i].x = x;
            spirals[i].y = y;
        }
        if ((x == y) || ((x < 0) && (x == -y)) || ((x > 0) && (x == 1 - y)))
        {
            t = dx;
            dx = -dy;
            dy = t;
        }
        x += dx;
        y += dy;
    }
    PRINT_LOG("Square spiral calculated!");
}

/** Initialize all floors by: 
 * - Setting position (with spirals offset)
 * - Anim timer position
 */
void FloorManager::initFloors()
{
    PRINT_LOG("Initializing floors");
    meshes = new Mesh *[floorAmount];

    Vector3 initPos = Vector3(0.0F, 0.0F, 0.0F);
    meshes[0] = new Mesh();
    meshes[0]->loadObj("floor/", "floor.obj", initPos, 2.0F);
    meshes[0]->shouldBeFrustumCulled = true;
    obj = meshes[0]->obj;
    spec = meshes[0]->spec;
    for (u8 i = 0; i < floorAmount; i++)
    {
        floors[i].init(obj, spec, spirals[i], i);
        meshes[i] = &floors[i].mesh;
    }
    PRINT_LOG("Floors initialized!");
}

/** Main floor loop.
 * - Checks if floor should be rendered (isVisible, frustum culling)
 * - Maintain floor animation
 * - Fix player position after floor position change
 */
void FloorManager::update(Player &t_player)
{
    for (u8 i = 0; i < floorAmount; i++)
    {
        floors[i].animate(t_player);

        if (floors[i].isByAudioTriggered == true)
            floors[i].mesh.color.a = 0x20;
        else
            floors[i].mesh.color.a = 0x80;
    }
}

/** Called by audio thread */
void FloorManager::onAudioTick()
{
    if (audioTick++ > 16)
    {
        if (audioMode == 0)
        {
            for (u8 i = 0; i < floorAmount; i++)
            {
                if ((i + audioOffset) % 2 == 0)
                    floors[i].isByAudioTriggered = true;
                else
                    floors[i].isByAudioTriggered = false;
            }
            if (++audioOffset > 3)
            {
                audioOffset = 0;
                audioMode = 1;
            }
        }
        else if (audioMode == 1)
        {
            for (u8 i = 0; i < floorAmount; i++)
            {
                if (i < floorAmount / (audioOffset + 1))
                    floors[i].isByAudioTriggered = true;
                else
                    floors[i].isByAudioTriggered = false;
            }
            if (++audioOffset > 3)
            {
                audioOffset = 0;
                audioMode = 2;
            }
        }
        else if (audioMode == 2)
        {
            for (u8 i = 0; i < floorAmount; i++)
            {
                if ((i + audioOffset) % 4 == 0)
                    floors[i].isByAudioTriggered = true;
                else
                    floors[i].isByAudioTriggered = false;
            }
            if (++audioOffset > 3)
            {
                audioOffset = 0;
                audioMode = 3;
            }
        }
        else if (audioMode == 3)
        {
            for (u8 i = 0; i < floorAmount; i++)
            {
                if (i + 1 > floorAmount / (audioOffset + 1))
                    floors[i].isByAudioTriggered = true;
                else
                    floors[i].isByAudioTriggered = false;
            }
            if (++audioOffset > 3)
            {
                audioOffset = 0;
                audioMode = 0;
            }
        }
    }
}
