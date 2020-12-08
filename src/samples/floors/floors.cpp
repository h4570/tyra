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
    : engine(t_engine), floorManager(), camera(&t_engine->screen)
{
    audioTicks = 0;
    skip1Beat = 0;
}

Floors::~Floors()
{
    delete enemy;
    delete ui;
    delete floorManager;
    delete player;
}

// ----
// Methods
// ----

void Floors::onInit()
{
    setBgColorAndAmbientColor();
    engine->renderer->setCameraDefinitions(&camera.worldView, &camera.unitCirclePosition, camera.planes);
    engine->audio.addSongListener(this);
    engine->audio.loadSong("sounds/mafikizolo-loot.wav");
    engine->audio.playSong();
    engine->audio.setSongVolume(80);
    texRepo = engine->renderer->getTextureRepository();
    enemy = new Enemy(texRepo);
    ui = new Ui(texRepo);
    floorManager = new FloorManager(FLOORS_COUNT, texRepo);
    player = new Player(&engine->audio, texRepo);
}

void Floors::onUpdate()
{
    ui->update(*player);
    enemy->update(*floorManager);
    lightManager.update();
    camera.update(engine->pad, player->mesh);
    floorManager->update(*player);
    player->update(engine->pad, camera, *floorManager, *enemy);
    engine->renderer->draw(player->mesh);
    engine->renderer->draw(enemy->getMeshes(), enemy->getMeshesCount());
    for (u16 i = 0; i < FLOORS_COUNT; i++)
        engine->renderer->drawByPath3(floorManager->floors[i].mesh, lightManager.bulbs, lightManager.bulbsCount);
    ui->render(engine->renderer); // 2D rendering ist LAST step, because layers gonna play there.
}

void Floors::onAudioTick()
{
    if ((++audioTicks + 20) % 21 == 0)
    {
        if (skip1Beat)
            floorManager->onAudioTick();
        lightManager.onAudioTick();
        skip1Beat = !skip1Beat;
    }
    if (audioTicks > 10000)
        audioTicks = 0;
}

void Floors::setBgColorAndAmbientColor()
{
    color_t bgColor;
    bgColor.r = 0x20;
    bgColor.g = 0x20;
    bgColor.b = 0x20;
    engine->renderer->setWorldColor(bgColor);
    Vector3 ambient = Vector3(0.003F, 0.003F, 0.003F);
    engine->renderer->setAmbientLight(ambient);
}

void Floors::onAudioFinish()
{
    audioTicks = 0;
}
