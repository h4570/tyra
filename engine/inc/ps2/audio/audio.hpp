/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, Tyra - https://github.com/h4570/tyrav2
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
# Wellington Carvalho <wellcoj@gmail.com>
*/

#pragma once

#include "./audio_adpcm.hpp"
#include "./audio_song.hpp"

namespace Tyra {

/** Class responsible for audio. */
class Audio {
 public:
  Audio();
  ~Audio();

  void init();

  AudioSong song;
  AudioAdpcm adpcm;

  void work();

 private:
  ee_thread_t thread;
  int threadId;
  static const unsigned short threadStackSize;
  unsigned char* threadStack;

  void initAUDSRV();
  void initThread();
};

}  // namespace Tyra
