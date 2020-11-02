/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

// SANDRO TODO
// Ogarnac czemu tekstura sie zle wczytuje
// Ogarnac wiele tekstur do skyboxa (wczytywane z .mtl i .obj)
// texture loading for obj and md2
// animation for dff
// TODO obj destructor
// TODO md2 destructor
// TODO dff destructor

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

#include "models/mesh_frame.hpp"
#include <vector>

void Ari::onInit()
{
    player = new Player();
    camera = new Camera(&engine.screen);
    engine.renderer->setCameraDefinitions(&camera->worldView, &camera->unitCirclePosition, camera->planes);
    engine.audio.init(0);
    engine.audio.setVolume(40);
    engine.audio.loadSong("MOV-CIRC.WAV");
    texRepo = engine.renderer->getTextureRepository();
    // engine.audio.play();

    // islandAddons = new Mesh();
    // islandAddons->rotation.x = -1.6F;
    // islandAddons->loadDff("sunnyisl/", "sunnyisl3.dff", islandPos, 0.1F);
    // islandAddons->shouldBeFrustumCulled = false;

    // TODO 1 - Refactor mesh, loaders
    // TODO 2 - const & in parameters
    // TODO 3 - Destructor mesh
    // TODO 4 - Destructor RwClump in dff loader
    // TODO 5 - Test mesh copy();

    // test->shouldBeFrustumCulled = true;

    test = new Mesh();
    skybox = new Mesh();
    island = new Mesh();

    island->loadDff("sunnyisl/", "sunnyisl", 0.1F, false);
    test->loadMD2("warrior/", "warrior", 0.2F, true);
    skybox->loadObj("skybox/", "skybox", 80.0F, false); // po zamianie miejscami działa

    test->rotation.x = -1.6F;
    test->playAnimation(9, 15, 0);
    skybox->position.set(0.0F, 10.0F, 0.0F);
    island->rotation.x = -1.6F;
    island->position.set(0.0F, 10.0F, 0.0F);

    texRepo->addByMesh("warrior/", *test);
    texRepo->addByMesh("skybox/", *skybox);
    texRepo->addByMesh("sunnyisl/", *island);

    // waterFloors = new Mesh *[WATER_TILES_COUNT];
    // spirals = new Point[WATER_TILES_COUNT];
    // Vector3 initPos = Vector3(0.0F, 8.0F, 0.0F);
    // waterFloors[0] = new Mesh();
    // // loadObj("water/", "water", initPos, 5.0F,12);
    // // loadObj("water/", "water", initPos, 5.0F);
    // waterFloors[0]->loadObj("water/", "water", initPos, 5.0F, 2);
    // waterFloors[0]->shouldBeFrustumCulled = true;
    // for (u8 i = 0; i < WATER_TILES_COUNT; i++)
    // {
    //     spirals[i].x = 1.0F;
    //     spirals[i].y = 2.0F;
    // }
    // u32 spiralOffset = (u32)Math::sqrt(WATER_TILES_COUNT);
    // calcSpiral(spiralOffset, spiralOffset);
    // for (u8 i = 1; i < WATER_TILES_COUNT; i++)
    // {
    //     waterFloors[i] = new Mesh();
    //     Vector3 nextPos = Vector3(10.0F * spirals[i].x, 8.0F, 10.0F * spirals[i].y);
    //     waterFloors[i]->setObj(nextPos, waterFloors[0]->obj, waterFloors[0]->spec);
    //     waterFloors[i]->shouldBeFrustumCulled = true;
    // }
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
    engine.renderer->draw(test);
    engine.renderer->draw(skybox);
    engine.renderer->draw(island);
    // engine.renderer->draw(islandAddons);
    // // engine.renderer->draw(&player->mesh);
    // for (u8 i = 0; i < WATER_TILES_COUNT; i++)
    //     engine.renderer->draw(waterFloors[i]); // TODO
}
