/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "../include/models/sprite.hpp"
#include "../include/utils/debug.hpp"
#include "../include/utils/string.hpp"
#include <cstdlib>

// ----
// Constructors/Destructors
// ----

Sprite::Sprite()
{
    id = rand() % 1000000;
    _isSizeSet = false;
    _flipH = false;
    _flipV = false;
    scale = 1.0F;
    mode = MODE_REPEAT;
    setDefaultColor();
    setDefaultLODAndClut();
}

Sprite::~Sprite() {}

// ----
// Methods
// ----

/** Set's default object color + no transparency */
void Sprite::setDefaultColor()
{
    color.r = 0x80;
    color.g = 0x80;
    color.b = 0x80;
    color.a = 0x80;
    color.q = 0.0F;
}

/** Sets texture level of details settings and CLUT settings */
void Sprite::setDefaultLODAndClut()
{
    clut.storage_mode = CLUT_STORAGE_MODE1;
    clut.start = 0;
    clut.psm = 0;
    clut.load_method = CLUT_NO_LOAD;
    clut.address = 0;
}
