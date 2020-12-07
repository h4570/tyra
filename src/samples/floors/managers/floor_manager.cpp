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

// ----
// Constructors/Destructors
// ----

/** Calculate square spiral offsets and initialize floors
 * @param floorAmount must be a number from square root table
 */
FloorManager::FloorManager(int t_floorAmount, TextureRepository *t_texRepo)
{
    texRepo = t_texRepo;
    floorAmount = t_floorAmount;
    spirals = new Point[t_floorAmount];
    int floorSpiralMaxOffset = (int)Math::sqrt(t_floorAmount);
    calcSpiral(floorSpiralMaxOffset, floorSpiralMaxOffset);
    initFloors();
    trick = 0.0F;
    trickMode = 0;
    isTimeForChangeTriggerColor = true;
    isTimeForChangeDefaultColor = true;
    audioOffset = audioMode = audioTick = 0;
}

FloorManager::~FloorManager()
{
    delete[] spirals;
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
    floors[0].mesh.loadObj("meshes/floor/", "floor", 3.0F, false);
    floors[0].mesh.shouldBeFrustumCulled = true;
    floors[0].mesh.shouldBeLighted = true;
    texRepo->addByMesh("meshes/floor/", floors[0].mesh, BMP);
    for (u16 i = 1; i < floorAmount; i++)
    {
        floors[i].init(floors[0].mesh, spirals[i], i);
        texRepo->getByMesh(floors[0].mesh.getId(), floors[0].mesh.getMaterial(0).getId())
            ->addLink(floors[i].mesh.getId(), floors[i].mesh.getMaterial(0).getId());
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
    if (isTimeForChangeTriggerColor)
    {
        isTimeForChangeTriggerColor = false;
        trigColor.r = rand() % (128 - 64 + 1) + 64;
        trigColor.g = rand() % (128 - 64 + 1) + 64;
        trigColor.b = rand() % (128 - 64 + 1) + 64;
    }
    if (isTimeForChangeDefaultColor)
    {
        isTimeForChangeDefaultColor = false;
        defaultColor.r = rand() % (64 + 1);
        defaultColor.g = rand() % (64 + 1);
        defaultColor.b = rand() % (64 + 1);
    }
    doTheTrick();
    for (u16 i = 0; i < floorAmount; i++)
    {
        floors[i].animate(t_player);

        if (floors[i].isByAudioTriggered)
        {
            floors[i].mesh.color.r = trigColor.r;
            floors[i].mesh.color.g = trigColor.g;
            floors[i].mesh.color.b = trigColor.b;
        }
        else
        {
            floors[i].mesh.color.r = defaultColor.r;
            floors[i].mesh.color.g = defaultColor.g;
            floors[i].mesh.color.b = defaultColor.b;
        }
    }
}

void FloorManager::doTheTrick()
{
    if (trickMode == 0)
    {
        if (trick > 3.0F)
        {
            isTimeForChangeTriggerColor = true;
            trickMode = 1;
        }
        trick += 0.01F;
    }
    else if (trickMode == 1)
    {
        if (trick <= 0.1F)
        {
            isTimeForChangeTriggerColor = true;
            trickMode = 0;
        }
        trick -= 0.01F;
    }
    floors[0].mesh.getFrames()[0].getST(1).set(trick, trick);
    floors[0].mesh.getFrames()[0].getST(4).set(trick, trick);
    floors[0].mesh.getFrames()[0].getST(7).set(trick, trick);
    floors[0].mesh.getFrames()[0].getST(10).set(trick, trick);
    floors[0].mesh.getFrames()[0].getST(13).set(trick, trick);
}

/** Called by audio thread */
void FloorManager::onAudioTick()
{
    if (audioTick++ > 19)
    {
        if (audioMode == 0)
        {
            for (u16 i = 0; i < floorAmount; i++)
            {
                if ((i + audioOffset) % 2 == 0)
                    floors[i].isByAudioTriggered = true;
                else
                    floors[i].isByAudioTriggered = false;
            }
            if (++audioOffset > 3)
            {
                audioOffset = 0;
                isTimeForChangeDefaultColor = true;
                audioMode = 1;
            }
        }
        else if (audioMode == 1)
        {
            for (u16 i = 0; i < floorAmount; i++)
            {
                if (i < floorAmount / (audioOffset + 1))
                    floors[i].isByAudioTriggered = true;
                else
                    floors[i].isByAudioTriggered = false;
            }
            if (++audioOffset > 3)
            {
                audioOffset = 0;
                isTimeForChangeDefaultColor = true;
                audioMode = 2;
            }
        }
        else if (audioMode == 2)
        {
            for (u16 i = 0; i < floorAmount; i++)
            {
                if ((i + audioOffset) % 4 == 0)
                    floors[i].isByAudioTriggered = true;
                else
                    floors[i].isByAudioTriggered = false;
            }
            if (++audioOffset > 3)
            {
                audioOffset = 0;
                isTimeForChangeDefaultColor = true;
                audioMode = 3;
            }
        }
        else if (audioMode == 3)
        {
            for (u16 i = 0; i < floorAmount; i++)
            {
                if (u16(i + 1) > floorAmount / (audioOffset + 1))
                    floors[i].isByAudioTriggered = true;
                else
                    floors[i].isByAudioTriggered = false;
            }
            if (++audioOffset > 3)
            {
                audioOffset = 0;
                isTimeForChangeDefaultColor = true;
                audioMode = 0;
            }
        }
    }
}
