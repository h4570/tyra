/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _TYRA_OBJ_LOADER_
#define _TYRA_OBJ_LOADER_

#include "../models/mesh_frame.hpp"
#include <stdio.h>

/** Class responsible for loading&parsing .obj 3D files */
class ObjLoader
{

public:
    ObjLoader();
    ~ObjLoader();

    void load(MeshFrame *o_result, char *t_fileName, float t_scale, u8 t_invertT);

private:
    void allocateObjMemory(FILE *t_file, MeshFrame *t_result);
};

#endif
