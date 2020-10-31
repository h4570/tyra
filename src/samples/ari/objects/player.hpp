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

#include "../camera.hpp"
#include <modules/pad.hpp>
#include <tamtypes.h>

/** Player 3D object class  */
class Player
{

public:
    float gravity, velocity, lift;
    u8 isOnFloor, isCollideFloor, indexOfCurrentFloor;
    Mesh mesh;
    Player();
    ~Player();

private:
    Vector3 playerNextPosition;
};

// TODO Rysowanie idzie wg tekstur

// 2 Graczy, 1 mesh 2 tekstury
// sharedFrames = objLoader.load("meshes/", "block", 1.0F);
// var addedTextures = texturesRepo.addByMaterialNames("textures/dirt/", sharedFrames);
// var addedTexture = texturesRepo.add("textures/blocks/sand.bmp");
// dirt = new Mesh(sharedFrames);
// sand = new Mesh(sharedFrames);
// addedTextures[0].addUsage(dirt.Id, sharedFrames.getMaterialId(0));
// addedTexture.addUsage(sand.Id, sand.getMaterialId(0));

// 1 gracz, 2 dynamic tekstury
// sharedFrames = objLoader.load("meshes/", "block", 1.0F);
// var addedTexture1 = texturesRepo.add("textures/blocks/sand.bmp");
// var addedTexture2 = texturesRepo.add("textures/blocks/sand_damaged.bmp");
// sand = new Mesh(sharedFrames);
// addedTexture1.removeUsage(sand.Id, sand.getMaterialId(0));
// draw()
// addedTexture2.addUsage(sand.Id, sand.getMaterialId(0));

// 2 różne obj, te same tekstury
// frames1 = objLoader.load("meshes/", "island", 1.0F);
// frames2 = objLoader.load("meshes/", "islandAddons", 1.0F);
// island = new Mesh(frames1);
// islandAddons = new Mesh(frames2);
// var addedTextures1 = texturesRepo.addByMaterialNames("textures/island/",frames1);
// var addedTextures2 = texturesRepo.addByMaterialNames("textures/island/",frames2);
// texturesRepo.remove(addedTextures2[0]); // duplicate
// addedTextures1[0].addUsage(islandAddons.Id, islandAddons.getMaterialId(0));

// Mesh -> position, rotation, color, scale, shouldBeLighted, clut, lod

#endif
