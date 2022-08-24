/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include <tyra>
#include "tutorial_06.hpp"

namespace Tyra {

Tutorial06::Tutorial06(Engine* t_engine) : engine(t_engine) {}

Tutorial06::~Tutorial06() {}

void Tutorial06::init() {
  /** 16bit 22kHz wav file */
  engine->audio.song.load(FileUtils::fromCwd("kayah-supermenka.wav"));
  engine->audio.song.inLoop = true;
  engine->audio.song.setVolume(60);
  engine->audio.song.play();

  /** 16bit 22kHz wav file converted to adpcm sample */
  sample = engine->audio.adpcm.load(FileUtils::fromCwd("shoot.adpcm"));
  /** Set volume for channel 0 */
  engine->audio.adpcm.setVolume(30, 0);
}

void Tutorial06::loop() {
  engine->renderer.beginFrame();
  {
    /** Play ADPCM if (X) button is clicked! */
    if (engine->pad.getClicked().Cross) {
      /** Try to play sample in channel 0 */
      if (engine->audio.adpcm.tryPlay(sample, 0) == ADPCM_CHANNEL_USED) {
        TYRA_WARN("Channel is busy, please use another channel!");
      }
    }
  }
  engine->renderer.endFrame();
}

}  // namespace Tyra
