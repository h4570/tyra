/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _TYRA_SPRITE_
#define _TYRA_SPRITE_

#include <models/math/point.hpp>
#include <draw_types.h>
#include <draw_buffers.h>

/** 
 * Class which have contain 2D image data.
 */
class Sprite
{

public:
    Sprite();
    ~Sprite();

    Point position, size;
    float scale;
    color_t color;
    clutbuffer_t clut;

    // ----
    // Getters
    // ----

    /** Auto generated unique Id. */
    inline const u32 &getId() const { return id; };

    // ----
    //  Setters
    // ----

    // ----
    //  Other
    // ----

    /** Flip texture horizontally. */
    inline void flipHorizontally(const u8 &t_set) { _flipH = t_set; };

    /** Flip texture vertically. */
    inline void flipVertically(const u8 &t_set) { _flipV = t_set; };

    /** Check if texture is vertically flipped. */
    inline const u8 &isFlippedVertically() const { return _flipV; };

    /** Check if texture is horizontally flipped. */
    inline const u8 &isFlippedHorizontally() const { return _flipH; };

private:
    u32 id;
    u8 _isSizeSet, _flipH, _flipV;
    void setDefaultColor();
    void setDefaultLODAndClut();
};

#endif
