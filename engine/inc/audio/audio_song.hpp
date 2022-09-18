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

#include "./audio_listener_ref.hpp"
#include <audsrv.h>
#include <string>
#include <vector>
#include <kernel.h>

namespace Tyra {

/** Class responsible for background audio playing. */
class AudioSong {
 public:
  AudioSong();
  ~AudioSong();

  void init();

  bool inLoop;

  /**
   * Load 16bit 22kHz Stereo WAV.
   * Can be used multiple times for song switching.
   * @param t_path Example: "host:song.wav" or "host:folder/song.wav"
   */
  void load(const char* t_path);
  void load(const std::string& t_path);

  void play();

  void stop();

  const bool& isPlaying() const;

  const bool& isLoaded() const;

  const u8& getVolume() const;

  /**
   * Set song volume.
   * @param t_vol Value 0-100
   */
  void setVolume(const u8& t_vol);

  /**
   * If you want to synchronize some actions with
   * background song, add audio listener.
   * @returns Listener id.
   */
  u32 addListener(AudioListener* t_listener);

  /** Remove song listener. */
  void removeListener(const u32& t_id);

  std::size_t getListenersCount() const;

  void work();

 private:
  bool songLoaded, songPlaying, songFinished;
  u8 tyraVolume, audsrvVolume;
  FILE* wav;
  audsrv_fmt_t format;
  std::vector<AudioListenerRef*> songListeners;

  ee_sema_t sema;
  s32 fillbufferSema;
  static const u16 chunkSize;
  char* chunk;
  s32 chunkReadStatus;

  void initSema();
  void initAUDSRV();
  void setSongFormat();
  void unloadSong();
  void rewindSongToStart();
};

}  // namespace Tyra
