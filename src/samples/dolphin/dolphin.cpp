/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Michał Mostowik <mostek3pl@gmail.com>
*/

#include "dolphin.hpp"
#include "utils/math.hpp"

const u8 WATER_TILE_SCALE = 5;
const u8 WATER_SIZE = 100;

const u8 MINES_COUNT = 15;
const u8 OYSTERS_COUNT = 5;

float Dolphin::engineFPS = 60.F;

Dolphin::Dolphin(Engine *t_engine) : engine(t_engine), camera(&engine->screen)
{
    oysters = new Collectible[OYSTERS_COUNT];
    mines = new Mine[MINES_COUNT];
}

Dolphin::~Dolphin()
{
    delete[] oysters;
    delete[] mines;
}

void Dolphin::onInit()
{
    player.init(engine);
    engine->renderer->setCameraDefinitions(&camera.worldView, &camera.position, camera.planes);

    engine->audio.loadSong("sound/dirediredocks.wav");
    engine->audio.playSong();
    engine->audio.setSongVolume(60);

    surfaceAmbient = engine->audio.loadADPCM("sound/surface.sad");
    underwaterAmbient = engine->audio.loadADPCM("sound/underwater.sad");
    pickupSound = engine->audio.loadADPCM("sound/pickup.sad");
    boomSound = engine->audio.loadADPCM("sound/boom.sad");

    engine->audio.playADPCM(surfaceAmbient, 0);
    engine->audio.playADPCM(underwaterAmbient, 1);

    texRepo = engine->renderer->getTextureRepository();

    engine->renderer->disableVSync();

    printf("loading island..\n");
    island.loadDff("sunnyisl/", "sunnyisl", 5.0F, false);
    printf("Loaded..\n");
    island.rotation.x = -1.6F;
    island.position.set(0.0F, 60.0F, 20.0F);
    island.shouldBeBackfaceCulled = true;
    island.shouldBeFrustumCulled = false;

    gameOver.size.set(640.0F, 480.0F);
    Texture *gameOverTex = texRepo->add("2d/", "gameover", PNG);
    gameOver.setMode(MODE_STRETCH);
    gameOverTex->addLink(gameOver.getId());

    printf("Loading water overlay...\n");
    waterOverlay.size.set(640.0F, 480.0F);
    Texture *watOverlayTex = texRepo->add("2d/", "underwater_overlay", PNG);
    waterOverlay.setMode(MODE_STRETCH);
    watOverlayTex->addLink(waterOverlay.getId());
    printf("Loaded.\n");

    printf("Loading water...\n");
    water.loadObj("water/", "water", WATER_TILE_SCALE, false);
    water.position.set(0, WATER_LEVEL, 0);
    texRepo->addByMesh("water/", water, BMP);
    water.shouldBeFrustumCulled = false;
    water.shouldBeBackfaceCulled = false;

    printf("Loading seabed...\n");
    seabed.loadObj("seabed/", "seabed", 10.0F, false);
    seabed.position.set(0, -250.0F, 0);
    texRepo->addByMesh("seabed/", seabed, BMP);
    seabed.shouldBeBackfaceCulled = false;
    seabed.shouldBeFrustumCulled = false;

    skybox.loadObj("skybox/", "skybox", 400.0F, false);
    skybox.shouldBeFrustumCulled = false;

    waterbox.loadObj("waterbox/", "waterbox", 250.0F, false);
    waterbox.shouldBeFrustumCulled = false;
    waterbox.shouldBeBackfaceCulled = false;
    waterbox.rotation.x = Math::PI;
    waterbox.position.y = -100.F;

    printf("Loading oyster dff\n");
    //oysters[0].loadDff("oyster/", "oyster", 1.F, false);
    oysters[0].mesh.loadObj("oyster/", "oyster", 10.F, false);
    printf("Loaded.");
    oysters[0].mesh.shouldBeBackfaceCulled = false;
    oysters[0].mesh.shouldBeFrustumCulled = false;
    oysters[0].mesh.position.set(15, 0, 15);
    texRepo->addByMesh("oyster/", oysters[0].mesh, BMP);
    printf("Adding oyster texture.\n");
    for (int i = 1; i < OYSTERS_COUNT; i++)
    {
        printf("Processing oyster %d\n", i);
        oysters[i].mesh.loadFrom(oysters[0].mesh);
        oysters[i].mesh.shouldBeBackfaceCulled = false;
        oysters[i].mesh.shouldBeFrustumCulled = false;
        texRepo->getByMesh(oysters[0].mesh.getId(), oysters[0].mesh.getMaterial(0).getId())
            ->addLink(oysters[i].mesh.getId(), oysters[i].mesh.getMaterial(0).getId());
        oysters[i].mesh.position.set(i * -100, 5.0F, i * -100);
    }

    printf("Loading mine .obj\n");
    mines[0].mesh.loadObj("mine/", "mine", 10.F, false);
    printf("Loaded.");
    mines[0].mesh.shouldBeBackfaceCulled = false;
    mines[0].mesh.shouldBeFrustumCulled = false;
    mines[0].mesh.position.set(5, -15, 32);
    texRepo->addByMesh("mine/", mines[0].mesh, BMP);
    printf("Adding mine texture.\n");
    for (int i = 1; i < MINES_COUNT; i++)
    {
        printf("Processing mine %d\n", i);
        mines[i].mesh.loadFrom(mines[0].mesh);
        mines[i].mesh.shouldBeBackfaceCulled = false;
        mines[i].mesh.shouldBeFrustumCulled = false;
        texRepo->getByMesh(mines[0].mesh.getId(), mines[0].mesh.getMaterial(0).getId())
            ->addLink(mines[i].mesh.getId(), mines[i].mesh.getMaterial(0).getId());
        mines[i].mesh.position.set(i * -10, 0.0F, i * -68);
    }

    Texture *pLifeTex = texRepo->add("2d/", "life", PNG);
    //pLifeTex->addLink(lifeSprites[0].getId());
    for (int i = 0; i < 3; i++)
    {
        lifeSprites[i].size.set(64.0F, 64.0F);
        lifeSprites[i].setMode(MODE_STRETCH);
        lifeSprites[i].position.set(395 + (64 * i), 0);
        pLifeTex->addLink(lifeSprites[i].getId());
    }

    texRepo->addByMesh("dolphin/", player.mesh, BMP);
    texRepo->addByMesh("sunnyisl/", island, BMP);
    texRepo->addByMesh("skybox/", skybox, BMP);
    texRepo->addByMesh("waterbox/", waterbox, BMP);

    bulb.intensity = 55;
    bulb.position.set(5.0F, 10.0F, 10.0f);
    printf("Finished initialization\n");
}

void Dolphin::onUpdate()
{
    if (player.getLifes() <= 0)
    {
        engine->audio.stopSong();
        engine->audio.setADPCMVolume(0, 0);
        engine->audio.setADPCMVolume(75, 1);
        engine->renderer->draw(gameOver);
        return;
    }
    Dolphin::engineFPS = engine->fps;
    if (engine->pad.isCrossClicked)
        printf("Delta multiplier: %f\n", 60.0F / engine->fps);

    float xDist = player.mesh.position.x - water.position.x;
    float zDist = player.mesh.position.z - water.position.z;
    if (xDist < 0)
        xDist *= -1;
    if (zDist < 0)
        zDist *= -1;
    if (xDist > WATER_SIZE * WATER_TILE_SCALE / 4 ||
        zDist > WATER_SIZE * WATER_TILE_SCALE / 4)
    {
        water.position.x = (player.mesh.position.x / WATER_SIZE) * WATER_SIZE;
        water.position.z = (player.mesh.position.z / WATER_SIZE) * WATER_SIZE;
    }

    for (int i = 0; i < OYSTERS_COUNT; i++)
    {
        oysters[i].update();
        Vector3 vecDist = oysters[i].mesh.position - player.mesh.position;
        float dist = vecDist.length();
        if (dist < 20.F && player.isJumping() && oysters[i].isActive())
        {
            printf("Pickup %d Dist %d\n", i, dist);
            oysters[i].disappear();
            player.setLife(player.getLifes() + 1);
            engine->audio.setADPCMVolume(30, 3);
            engine->audio.playADPCM(pickupSound, 3);
        }
    }

    for (int i = 0; i < MINES_COUNT; i++)
    {
        mines[i].update();
        Vector3 vecDist = mines[i].mesh.position - player.mesh.position;
        float dist = vecDist.length();

        if (dist < 20.F && !mines[i].getExplosionTicks())
        {
            printf("Vecdist %f,%f,%f\n", vecDist.x, vecDist.y, vecDist.z);
            printf("Explode %d Dist %d\n", i, dist);
            mines[i].explode();
            player.setLife(player.getLifes() - 1);
            //Buffer switching to prevent playing two sounds on one buffer
            engine->audio.setADPCMVolume(65, Mine::lastSoundBuffer);
            engine->audio.playADPCM(boomSound, Mine::lastSoundBuffer);
            Mine::lastSoundBuffer = Mine::lastSoundBuffer == 4 ? 5 : 4;
        }
    }

    if (player.mesh.position.y > WATER_LEVEL - 5)
        player.mesh.position.y = WATER_LEVEL - 5;

    player.update(engine->pad);
    camera.update(engine->pad, player.mesh);
    skybox.position.set(player.mesh.position.x, +200.0F, player.mesh.position.z);
    waterbox.position.set(player.mesh.position.x, -260.0F, player.mesh.position.z);
    engine->renderer->draw(island);
    engine->renderer->draw(water);
    engine->renderer->draw(skybox);
    engine->renderer->draw(waterbox);
    engine->renderer->draw(seabed);
    for (u8 i = 0; i < OYSTERS_COUNT; i++)
    {
        if (oysters[i].isActive())
        {
            engine->renderer->draw(oysters[i].mesh);
        }
    }
    for (u8 i = 0; i < MINES_COUNT; i++)
    {
        if (mines[i].getExplosionTicks() < MINE_EXPLOSION_TICKS)
            engine->renderer->draw(mines[i].mesh);
    }
    for (u8 i = 0; i < player.getLifes(); i++)
    {
        engine->renderer->draw(lifeSprites[i]);
    }
    engine->renderer->draw(player.mesh);
    if (camera.position.y < WATER_LEVEL)
    {
        engine->renderer->draw(waterOverlay);
        engine->audio.setADPCMVolume(0, 0);  //Surface ambient
        engine->audio.setADPCMVolume(65, 1); //Underwater ambient
    }
    else
    {
        engine->audio.setADPCMVolume(50, 0); //Surface ambient
        engine->audio.setADPCMVolume(0, 1);  //Underwater ambient
    }
}
