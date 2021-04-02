#ifndef _CUBE_
#define _CUBE_

#include <tamtypes.h>
#include <game.hpp>
#include <engine.hpp>
#include "managers/light_manager.hpp"
#include "./camera.hpp"

class Cube : public Game
{

public:
    Cube(Engine *t_engine);
    ~Cube();

    void onInit();
    void onUpdate();

    Engine *engine;

private:
    void setBgColorAndAmbientColor();
    LightManager lightManager;
    Camera camera;
};

#endif