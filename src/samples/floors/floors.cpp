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

Floors::Floors() {}

Floors::~Floors() {}

// ----
// Methods
// ----

void Floors::onInit()
{
    player = new Player();
    camera = new Camera(&engine.screen);
    engine.renderer->setCameraDefinitions(&camera->worldView, &camera->unitCirclePosition, camera->planes);
    floorManager = new FloorManager(64); // Temp change it also in floor_manager.hpp
    engine.audio.init(2);
    engine.audio.addListener(floorManager);
    engine.audio.addListener(&lightManager);
    engine.audio.loadSong("NF-CHILL.WAV");
    engine.audio.play();
    texRepo = engine.renderer->getTextureRepository();
    texRepo->addByMesh("warrior/", player->mesh);
    texRepo->addByMesh("floor/", *floorManager->meshes[0]);
    for (size_t i = 1; i < floorManager->floorAmount; i++)
        texRepo->getByMesh(floorManager->meshes[0]->getId(), floorManager->meshes[0]->getMaterial(0).getId())
            ->addLink(floorManager->meshes[i]->getId(), floorManager->meshes[i]->getMaterial(0).getId());
}

void Floors::onUpdate()
{
    if (engine.pad.isCrossClicked)
        printf("FPS:%f\n", engine.fps);
    lightManager.update();
    camera->update(engine.pad, player->mesh);
    floorManager->update(*player);
    player->update(engine.pad, *camera, *floorManager);
    engine.renderer->draw(&player->mesh);
    engine.renderer->drawByPath3(floorManager->meshes, floorManager->floorAmount, lightManager.bulbs, lightManager.bulbsCount);
}
