
/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

namespace Tyra {

enum AnimationSequenceCallback {
  /** On frame change */
  AnimationSequenceCallback_NextFrame,
  /** When animation in loop = false will end */
  AnimationSequenceCallback_End,
  /** When animation in loop = true will start from beginning */
  AnimationSequenceCallback_Loop
};

}  // namespace Tyra
