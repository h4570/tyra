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

Ari::Ari() {}

Ari::~Ari() {}

// ----
// Methods
// ----

// TEST START
const u8 WATER_TILES_COUNT = 64;
Mesh *island, *islandAddons, *skybox, *test;
Mesh **waterFloors;
Point *spirals;

void calcSpiral(int X, int Y)
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
// TEST END

// TODO 1 - const & in parameters
// TODO 2 - Destructor mesh

void Ari::onInit()
{
    player = new Player();
    camera = new Camera(&engine.screen);
    engine.renderer->setCameraDefinitions(&camera->worldView, &camera->unitCirclePosition, camera->planes);
    engine.audio.init(0);
    engine.audio.setVolume(40);
    engine.audio.loadSong("MOV-CIRC.WAV");
    engine.audio.play();

    texRepo = engine.renderer->getTextureRepository();

    island = new Mesh();
    island->loadDff("sunnyisl/", "sunnyisl", 0.1F, false);
    island->rotation.x = -1.6F;
    island->position.set(0.0F, 10.0F, 20.0F);
    island->shouldBeBackfaceCulled = true;
    island->shouldBeFrustumCulled = false;

    islandAddons = new Mesh();
    islandAddons->loadDff("sunnyisl/", "sunnyisl3", 0.1F, false);
    islandAddons->rotation.x = -1.6F;
    islandAddons->position.set(0.0F, 10.0F, 20.0F);

    skybox = new Mesh();
    skybox->loadObj("skybox/", "skybox", 100.0F, false);
    skybox->shouldBeFrustumCulled = false;

    waterFloors = new Mesh *[WATER_TILES_COUNT];
    spirals = new Point[WATER_TILES_COUNT];
    waterFloors[0] = new Mesh();
    waterFloors[0]->loadObj("water/", "water", 5.0F, false);
    waterFloors[0]->position.set(0.0F, 8.0F, 0.0F);
    texRepo->addByMesh("water/", *waterFloors[0]);
    for (u8 i = 0; i < WATER_TILES_COUNT; i++)
    {
        spirals[i].x = 1.0F;
        spirals[i].y = 2.0F;
    }
    u32 spiralOffset = (u32)Math::sqrt(WATER_TILES_COUNT);
    calcSpiral(spiralOffset, spiralOffset);
    for (u8 i = 1; i < WATER_TILES_COUNT; i++)
    {
        waterFloors[i] = new Mesh();
        waterFloors[i]->loadFrom(*waterFloors[0]);
        waterFloors[i]->position.set(10.0F * spirals[i].x, 8.0F, 10.0F * spirals[i].y);
        texRepo->getByMesh(waterFloors[0]->getId(), waterFloors[0]->getMaterial(0).getId())
            ->addLink(waterFloors[i]->getId(), waterFloors[i]->getMaterial(0).getId());
    }

    texRepo->addByMesh("sunnyisl/", *island);
    texRepo->addByMesh("sunnyisl/", *islandAddons);
    texRepo->addByMesh("skybox/", *skybox);
    texRepo->addByMesh("ari/", player->mesh);
}

void Ari::initBulb()
{
    bulb.intensity = 15;
    bulb.position.set(5.0F, 10.0F, 10.0F);
}

void Ari::onUpdate()
{
    if (engine.pad.isCrossClicked)
        printf("FPS:%f\n", engine.fps);
    camera->update(engine.pad, player->mesh);
    engine.renderer->draw(skybox);
    engine.renderer->draw(island);
    engine.renderer->draw(islandAddons);
    engine.renderer->draw(&player->mesh);
    for (u8 i = 0; i < WATER_TILES_COUNT; i++)
        engine.renderer->draw(waterFloors[i]);
}
