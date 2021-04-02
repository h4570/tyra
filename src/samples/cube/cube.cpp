#include "cube.hpp"

Cube::Cube(Engine *t_engine)
    : engine(t_engine), camera(&t_engine->screen)
{
    return;
}

Cube::~Cube()
{
    return;
}

void Cube::onInit(){
    setBgColorAndAmbientColor();
    engine->renderer->setCameraDefinitions(&camera.view, &camera.unitCirclePosition, camera.planes);
}

void Cube::onUpdate(){
    lightManager.update();
}

void Cube::setBgColorAndAmbientColor()
{
    color_t bgColor;
    bgColor.r = 0x20;
    bgColor.g = 0x20;
    bgColor.b = 0x20;
    engine->renderer->setWorldColor(bgColor);
    Vector3 ambient = Vector3(0.003F, 0.003F, 0.003F);
    engine->renderer->setAmbientLight(ambient);
}