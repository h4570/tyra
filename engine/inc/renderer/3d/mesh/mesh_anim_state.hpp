
/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020 - 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

#include <tamtypes.h>

namespace Tyra {

typedef struct {
  u32 startFrame;
  u32 endFrame;
  u32 stayFrame;
  u8 isStayFrameSet;
  float speed;
  float interpolation;
  u32 animType;
  u32 currentFrame;
  u32 nextFrame;
} MeshAnimState;

}  // namespace Tyra
