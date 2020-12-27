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
    texRepo = engine->renderer->getTextureRepository();
    enemy = new Enemy(texRepo);
    ui = new Ui(texRepo);
    floorManager = new FloorManager(FLOORS_COUNT, texRepo); // we must init FloorManager before starting audio!
    player = new Player(&engine->audio, texRepo);
    setBgColorAndAmbientColor();
    engine->renderer->setCameraDefinitions(&camera.view, &camera.unitCirclePosition, camera.planes);
    engine->audio.addSongListener(this);
    engine->audio.loadSong("sounds/mafikizolo-loot.wav");
    engine->audio.playSong();
    engine->audio.setSongVolume(80);
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

    // You can draw array of meshes in draw() function also. It can be A LOT faster than for looping!
    // Why? When given mesh is small, is not animated and is not backface culled, vertex data are send once!
    engine->renderer->draw(floorManager->getMeshes(), FLOORS_COUNT, lightManager.bulbs, lightManager.bulbsCount);
    // draw() in array mode, sometimes will force you to do synchronization between EE <-> VU1 <-> GS via:
    // renderer->clearAndWaitForRender(); or renderer->waitForRender();

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
