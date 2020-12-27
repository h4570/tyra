/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _TYRA_UTILS_
#define _TYRA_UTILS_

#include <tamtypes.h>
#include <models/mesh.hpp>
#include <models/math/vector3.hpp>

class Utils
{

public:
    static float expoEaseInOut(float t, float b, float c, float d);
    static void getMinMax(const Mesh &t_mesh, Vector3 &t_min, Vector3 &t_max);
};

#endif
