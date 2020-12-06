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

const u16 FLOORS_COUNT = 144; // Temp change it also in floor_manager.hpp

Floors::Floors(Engine *t_engine)
    : engine(t_engine), floorManager(FLOORS_COUNT),
      player(&t_engine->audio), camera(&t_engine->screen)
{
    audioTicks = 0;
    skip1Beat = 0;
}

Floors::~Floors()
{
    delete enemy;
    delete ui;
}

// ----
// Methods
// ----

void Floors::onInit()
{
    engine->renderer->setCameraDefinitions(&camera.worldView, &camera.unitCirclePosition, camera.planes);
    engine->audio.addSongListener(this);
    engine->audio.loadSong("nob-else.wav");
    engine->audio.playSong();
    texRepo = engine->renderer->getTextureRepository();
    enemy = new Enemy(texRepo);
    ui = new Ui(texRepo);
    engine->audio.setSongVolume(80);

    texRepo->addByMesh("warrior/", player.mesh, BMP);
    texRepo->addByMesh("floor/", floorManager.floors[0].mesh, BMP);
    for (u32 i = 1; i < floorManager.floorAmount; i++)
        texRepo->getByMesh(floorManager.floors[0].mesh.getId(), floorManager.floors[0].mesh.getMaterial(0).getId())
            ->addLink(floorManager.floors[i].mesh.getId(), floorManager.floors[i].mesh.getMaterial(0).getId());
}

void Floors::onUpdate()
{
    ui->update(player);
    lightManager.update();
    camera.update(engine->pad, player.mesh);
    floorManager.update(player);
    player.update(engine->pad, camera, floorManager, *enemy);
    engine->renderer->draw(player.mesh);
    engine->renderer->draw(enemy->getMeshes(), enemy->getMeshesCount());
    for (u16 i = 0; i < FLOORS_COUNT; i++)
        engine->renderer->drawByPath3(floorManager.floors[i].mesh, lightManager.bulbs, lightManager.bulbsCount);
    ui->render(engine->renderer); // 2D rendering ist LAST step, because layers gonna play there.
}

void Floors::onAudioTick()
{
    if ((++audioTicks + 20) % 28 == 0)
    {
        if (skip1Beat)
            floorManager.onAudioTick();
        lightManager.onAudioTick();
        skip1Beat = !skip1Beat;
    }
    if (audioTicks > 10000)
        audioTicks = 0;
}

void Floors::onAudioFinish()
{
    audioTicks = 0;
}
