/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Wellington Carvalho <wellcoj@gmail.com>
*/

#ifndef _CUBE_
#define _CUBE_

#include "../camera.hpp"
#include <tamtypes.h>
#include <modules/pad.hpp>
#include <modules/timer.hpp>
#include "modules/texture_repository.hpp"

/** Cube 3D object class  */
class Cube
{
    public:
        Mesh mesh;
        Cube( TextureRepository *t_texRepo );
        ~Cube();

        void update(const Pad &t_pad, const Camera &t_camera);

    private:
        TextureRepository *texRepo;
        Vector3 *getNextPosition(const Pad &t_pad, const Camera &t_camera);
        void rotate(const Pad &t_pad);
        float speed;
};

#endif
