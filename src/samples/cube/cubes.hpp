#ifndef _CUBES_
#define _CUBES_

#include <tamtypes.h>
#include <game.hpp>
#include <engine.hpp>
#include "managers/light_manager.hpp"
#include "modules/texture_repository.hpp"
#include "camera.hpp"
#include "objects/cube.hpp"

class Cubes : public Game
{

    public:
        Cubes(Engine *t_engine);
        ~Cubes();

        void onInit();
        void onUpdate();

        Engine *engine;

    private:
        void setBgColorAndAmbientColor();
        TextureRepository *texRepo;
        Cube *cube;
        LightManager lightManager;
        Camera camera;
};

#endif