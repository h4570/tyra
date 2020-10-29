/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "../include/models/mesh_spec.hpp"

#include "../include/utils/debug.hpp"
#include "../include/utils/math.hpp"
#include <graph_vram.h>
#include <gs_psm.h>

// ----
// Constructors/Destructors
// ----

MeshSpec::MeshSpec()
{
    PRINT_LOG("Creating object 3D spec");
    this->setupLodAndClut();
    PRINT_LOG("Object 3D spec created!");
}

/** Release memory if object was initialized */
MeshSpec::~MeshSpec()
{
}

// ----
// Methods
// ----

/** Sets texture level of details settings and CLUT settings */
void MeshSpec::setupLodAndClut()
{
    PRINT_LOG("Setting LOD, CLUT");
    this->lod.calculation = LOD_USE_K;
    this->lod.max_level = 0;
    this->lod.mag_filter = LOD_MAG_NEAREST;
    this->lod.min_filter = LOD_MIN_NEAREST;
    this->lod.l = 0;
    this->lod.k = 0.0F;

    this->clut.storage_mode = CLUT_STORAGE_MODE1;
    this->clut.start = 0;
    this->clut.psm = 0;
    this->clut.load_method = CLUT_NO_LOAD;
    this->clut.address = 0;
    PRINT_LOG("LOD, CLUT set!");
}
