/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, Tyra - https://github.com/h4570/tyrav2
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
# Wellinator Carvalho <wellcoj@gmail.com>
*/

#pragma once

namespace Tyra {

class AudioListener {
 public:
  virtual ~AudioListener(){};
  virtual void onAudioTick() = 0;
  virtual void onAudioFinish() = 0;

 private:
};

}  // namespace Tyra
