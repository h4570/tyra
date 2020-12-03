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

const u8 WATER_TILE_SCALE = 5.0F;
const u8 WATER_SIZE = 100.0F;

Dolphin::Dolphin(Engine *t_engine) : engine(t_engine), camera(&engine->screen)
{
}

Dolphin::~Dolphin() {}

void Dolphin::onInit()
{
    engine->renderer->setCameraDefinitions(&camera.worldView, &camera.position, camera.planes);

    texRepo = engine->renderer->getTextureRepository();

    engine->renderer->disableVSync();

    printf("loading island..\n");
    island.loadDff("sunnyisl/", "sunnyisl", 1.0F, false);
    printf("Loaded..\n");
    island.rotation.x = -1.6F;
    island.position.set(0.0F, 10.0F, 20.0F);
    island.shouldBeBackfaceCulled = true;
    island.shouldBeFrustumCulled = false;

    printf("Loading water overlay...\n");
    waterOverlay.size.set(100.0F, 100.0F);
    Texture *watOverlayTex = texRepo->add("2d/", "underwater_overlay", PNG);
    watOverlayTex->addLink(waterOverlay.getId());
    printf("Loaded.\n");

    printf("Loading water...\n");
    water.loadObj("water/", "water", WATER_TILE_SCALE, false);
    water.position.set(0, WATER_LEVEL, 0);
    texRepo->addByMesh("water/", water, BMP);
    water.shouldBeFrustumCulled = false;
    water.shouldBeBackfaceCulled = false;

    skybox.loadObj("skybox/", "skybox", 400.0F, false);
    skybox.shouldBeFrustumCulled = false;

    waterbox.loadObj("waterbox/", "waterbox", 250.0F, false);
    waterbox.shouldBeFrustumCulled = false;
    waterbox.shouldBeBackfaceCulled = false;
    waterbox.rotation.x = Math::PI;
    waterbox.position.y = -100.F;

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
    if (engine->pad.isCrossClicked)
        printf("PlayerPos (%f,%f), FPS:%f\n", player.mesh.position.x, player.mesh.position.z, this->engine->fps);

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
    engine->renderer->draw(player.mesh);
    //if (player.mesh.position.y < WATER_LEVEL)
    //engine->renderer->draw(waterOverlay);
}
