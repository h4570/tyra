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
        float speed;
};

#endif
