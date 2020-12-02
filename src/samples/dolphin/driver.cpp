/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Michał Mostowik <mostek3pl@gmail.com>
*/

#include "driver.hpp"
#include "utils/math.hpp"

Driver::Driver(Engine *t_engine) : engine(t_engine), camera(&engine->screen)
{

}

Driver::~Driver() {}

void Driver::onInit()
{
    engine->renderer->setCameraDefinitions(&camera.worldView, &camera.position, camera.planes);

    texRepo = engine->renderer->getTextureRepository();
    
    engine->renderer->disableVSync();

    island.loadDff("sunnyisl/", "sunnyisl", 1.0F, false);
    island.rotation.x = -1.6F;
    island.position.set(0.0F, 10.0F, 20.0F);
    island.shouldBeBackfaceCulled = true;
    island.shouldBeFrustumCulled = false;

    texRepo->addByMesh("dolphin/",player.mesh);
    texRepo->addByMesh("sunnyisl/",island);

    bulb.intensity=55;
    bulb.position.set(5.0F,10.0F,10.0f);
}

void Driver::onUpdate()
{
    player.update(engine->pad);
    camera.update(engine->pad,player.mesh);
    engine->renderer->draw(island);
    engine->renderer->draw(player.mesh);
}
