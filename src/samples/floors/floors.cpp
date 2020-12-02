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

const u8 FLOORS_COUNT = 144; // Temp change it also in floor_manager.hpp

Floors::Floors(Engine *t_engine)
    : engine(t_engine), floorManager(FLOORS_COUNT), camera(&t_engine->screen)
{
    audioTicks = 0;
}

Floors::~Floors() {}

// ----
// Methods
// ----

void Floors::onInit()
{
    engine->renderer->setCameraDefinitions(&camera.worldView, &camera.unitCirclePosition, camera.planes);
    engine->audio.addSongListener(this);
    engine->audio.loadSong("NF-CHILL.WAV");
    engine->audio.setSongVolume(100);
    engine->audio.playSong();
    texRepo = engine->renderer->getTextureRepository();
    texRepo->addByMesh("warrior/", player.mesh, BMP);
    texRepo->addByMesh("floor/", floorManager.floors[0].mesh, BMP);
    for (u32 i = 1; i < floorManager.floorAmount; i++)
        texRepo->getByMesh(floorManager.floors[0].mesh.getId(), floorManager.floors[0].mesh.getMaterial(0).getId())
            ->addLink(floorManager.floors[i].mesh.getId(), floorManager.floors[i].mesh.getMaterial(0).getId());
}

void Floors::onUpdate()
{
    if (engine->pad.isCrossClicked)
        printf("FPS:%f\n", engine->fps);
    lightManager.update();
    camera.update(engine->pad, player.mesh);
    floorManager.update(player);
    player.update(engine->pad, camera, floorManager);
    engine->renderer->draw(player.mesh);
    for (u8 i = 0; i < FLOORS_COUNT; i++)
        engine->renderer->drawByPath3(floorManager.floors[i].mesh, lightManager.bulbs, lightManager.bulbsCount);
}

void Floors::onAudioTick()
{
    if ((++audioTicks + 20) % 41 == 0)
    {
        floorManager.onAudioTick();
        lightManager.onAudioTick();
    }
    if (audioTicks > 10000)
        audioTicks = 0;
}

void Floors::onAudioFinish()
{
    audioTicks = 0;
}
