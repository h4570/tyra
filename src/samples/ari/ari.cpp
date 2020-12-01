/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "ari.hpp"

#include "utils/math.hpp"

// ----
// Constructors/Destructors
// ----

const u8 WATER_TILES_COUNT = 64;

Ari::Ari(Engine *t_engine) : engine(t_engine), camera(&engine->screen)
{
    waterFloors = new Mesh[WATER_TILES_COUNT];
    spirals = new Point[WATER_TILES_COUNT];
}

Ari::~Ari() {}

// ----
// Methods
// ----

void Ari::onInit()
{
    engine->renderer->setCameraDefinitions(&camera.worldView, &camera.position, camera.planes);
    engine->audio.setSongVolume(40);
    // engine->audio.loadSong("MOV-CIRC.WAV");
    engine->audio.loadSong("nob-else.wav");

    adpcm1 = engine->audio.loadADPCM("ziobro1.adpcm");
    adpcm2 = engine->audio.loadADPCM("ziobro2.adpcm");

    texRepo = engine->renderer->getTextureRepository();
    engine->renderer->disableVSync();
    island.loadDff("sunnyisl/", "sunnyisl", 0.1F, false);
    island.rotation.x = -1.6F;
    island.position.set(0.0F, 10.0F, 20.0F);
    island.shouldBeBackfaceCulled = true;
    island.shouldBeFrustumCulled = false;

    fist.size.set(100.0F, 100.0F);
    Texture *fistTex = texRepo->add("2d/", "fist", PNG);
    fistTex->addLink(fist.getId());

    islandAddons.loadDff("sunnyisl/", "sunnyisl3", 0.1F, false);
    islandAddons.shouldBeBackfaceCulled = true;
    islandAddons.rotation.x = -1.6F;
    islandAddons.position.set(0.0F, 10.0F, 20.0F);

    skybox.loadObj("skybox/", "skybox", 100.0F, false);
    skybox.shouldBeFrustumCulled = false;

    waterFloors[0].loadObj("water/", "water", 5.0F, false);
    waterFloors[0].position.set(0.0F, 8.0F, 0.0F);
    texRepo->addByMesh("water/", waterFloors[0], BMP);
    for (u8 i = 0; i < WATER_TILES_COUNT; i++)
    {
        spirals[i].x = 1.0F;
        spirals[i].y = 2.0F;
    }
    u32 spiralOffset = (u32)Math::sqrt(WATER_TILES_COUNT);
    calcSpiral(spiralOffset, spiralOffset);
    for (u8 i = 1; i < WATER_TILES_COUNT; i++)
    {
        waterFloors[i].loadFrom(waterFloors[0]);
        waterFloors[i].position.set(10.0F * spirals[i].x, 8.0F, 10.0F * spirals[i].y);
        texRepo->getByMesh(waterFloors[0].getId(), waterFloors[0].getMaterial(0).getId())
            ->addLink(waterFloors[i].getId(), waterFloors[i].getMaterial(0).getId());
    }

    texRepo->addByMesh("sunnyisl/", island, BMP);
    texRepo->addByMesh("sunnyisl/", islandAddons, BMP);
    texRepo->addByMesh("skybox/", skybox, BMP);
    texRepo->addByMesh("ari/", player.mesh, BMP);
    // engine->audio.playSong();
}

void Ari::initBulb()
{
    bulb.intensity = 15;
    bulb.position.set(5.0F, 10.0F, 10.0F);
}

void Ari::onUpdate()
{
    if (engine->pad.isCrossClicked)
    {
        // engine->audio.loadSong("nob-else.wav");
        engine->audio.playADPCM(adpcm1);
        printf("FPS:%f\n", engine->fps);
    }
    if (engine->pad.isCircleClicked)
        engine->audio.playADPCM(adpcm2);
    camera.update(engine->pad, player.mesh);
    engine->renderer->draw(skybox);
    engine->renderer->draw(fist);
    engine->renderer->draw(island);
    engine->renderer->draw(islandAddons);
    engine->renderer->draw(player.mesh);
    for (u8 i = 0; i < WATER_TILES_COUNT; i++)
        engine->renderer->draw(waterFloors[i]);
}

void Ari::calcSpiral(int X, int Y)
{
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
}
