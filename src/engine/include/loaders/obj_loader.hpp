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

#include "../models/obj_model.hpp"
#include <stdio.h>

/** Class responsible for loading&parsing .obj 3D files */
class ObjLoader
{

public:
    ObjLoader();
    ~ObjLoader();

    void load(Frame *o_result, char *t_fileName, float t_scale);

private:
    void allocateObjMemory(FILE *t_file, Frame *t_result);
};

#endif
