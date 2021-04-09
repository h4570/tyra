#include "cubes.hpp"

Cubes::Cubes(Engine *t_engine)
    : engine(t_engine), camera(&t_engine->screen)
{
    PRINT_LOG("Initing cubes sample");
}

Cubes::~Cubes()
{
    return;
}

void Cubes::onInit(){
    texRepo = engine->renderer->getTextureRepository();
    cube = new Cube(texRepo);
    setBgColorAndAmbientColor();
    engine->renderer->setCameraDefinitions(&camera.view, &camera.unitCirclePosition, camera.planes);
}

void Cubes::onUpdate(){
    lightManager.update();
    camera.update(engine->pad, cube->mesh);
    cube->update(engine->pad, camera);
    engine->renderer->draw(cube->mesh);
}

void Cubes::setBgColorAndAmbientColor()
{
    color_t bgColor;
    bgColor.r = 0x20;
    bgColor.g = 0x20;
    bgColor.b = 0x20;
    engine->renderer->setWorldColor(bgColor);
    Vector3 ambient = Vector3(0.2F, 0.2F, 0.2F);
    engine->renderer->setAmbientLight(ambient);
}