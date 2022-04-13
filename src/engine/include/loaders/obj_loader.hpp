/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020 - 2022, tyra - https://github.com/h4570/tyra
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

    /** Parse .obj file, allocate output data and store it. Multitexture support
     * Notice: At this moment textures names are MATERIAL names from .obj file!
     * Notice 2: Faces MUST be triangulated (check out blender export settings).
     */
    void load(MeshFrame *o_result, const char *t_fileName, const float &t_scale, const u8 &t_invertT);

private:
    void allocateObjMemory(FILE *t_file, MeshFrame *t_result);
};

#endif
