/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020 - 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
# André Guilherme <andregui17@outlook.com>
*/

#ifndef TYRA_MD_LOADER_HPP
#define TYRA_MD_LOADER_HPP
#include "../models/mesh_frame.hpp"
#include "md2_structure.hpp"
#include "md3_structure.hpp"
#include <stdio.h>
#include <tamtypes.h>


/** Class responsible for loading and parsing Quake's II ".md2" and Quake III ".md3" 3D files */
class MDLoader
{
public:
    MDLoader();
    ~MDLoader();

    MeshFrame *load_md2(u32 &o_framesCount, char *t_subpath, char *t_nameWithoutExtension, float t_scale, u8 t_invertT);
	MeshFrame *load_md3(u32 &o_framesCount, char *t_subpath, char *t_nameWithoutExtension, float t_scale, u8 t_invertT);
};

#endif //TYRA_MD_LOADER_HPP
