/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "floors.hpp"

// ----
// Constructors/Destructors
// ----

const u8 FLOORS_COUNT = 64; // Temp change it also in floor_manager.hpp

Floors::Floors(const Engine &t_engine)
    : engine(t_engine), floorManager(FLOORS_COUNT), camera(&engine.screen) {}

Floors::~Floors() {}

// ----
// Methods
// ----

void Floors::onInit()
{
    engine.renderer->setCameraDefinitions(&camera.worldView, &camera.unitCirclePosition, camera.planes);
    engine.audio.init(2);
    engine.audio.addListener(&floorManager);
    engine.audio.addListener(&lightManager);
    engine.audio.loadSong("NF-CHILL.WAV");
    engine.audio.play();
    texRepo = engine.renderer->getTextureRepository();
    texRepo->addByMesh("warrior/", player.mesh);
    texRepo->addByMesh("floor/", floorManager.floors[0].mesh);
    for (u32 i = 1; i < floorManager.floorAmount; i++)
        texRepo->getByMesh(floorManager.floors[0].mesh.getId(), floorManager.floors[0].mesh.getMaterial(0).getId())
            ->addLink(floorManager.floors[i].mesh.getId(), floorManager.floors[i].mesh.getMaterial(0).getId());
}

void Floors::onUpdate()
{
    if (engine.pad.isCrossClicked)
        printf("FPS:%f\n", engine.fps);
    lightManager.update();
    camera.update(engine.pad, player.mesh);
    floorManager.update(player);
    player.update(engine.pad, camera, floorManager);
    engine.renderer->draw(player.mesh);
    for (u8 i = 0; i < FLOORS_COUNT; i++)
        engine.renderer->drawByPath3(floorManager.floors[i].mesh, lightManager.bulbs, lightManager.bulbsCount);
}
