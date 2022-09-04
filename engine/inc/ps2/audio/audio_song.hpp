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

  const unsigned char& getVolume() const;

  /**
   * Set song volume.
   * @param t_vol Value 0-100
   */
  void setVolume(const unsigned char& t_vol);

  /**
   * If you want to synchronize some actions with
   * background song, add audio listener.
   * @returns Listener id.
   */
  unsigned int addListener(AudioListener* t_listener);

  /** Remove song listener. */
  void removeListener(const unsigned int& t_id);

  std::size_t getListenersCount() const;

  void work();

 private:
  bool songLoaded, songPlaying, songFinished;
  unsigned char tyraVolume, audsrvVolume;
  FILE* wav;
  audsrv_fmt_t format;
  std::vector<AudioListenerRef*> songListeners;

  ee_sema_t sema;
  int fillbufferSema;
  static const unsigned short chunkSize;
  char* chunk;
  int chunkReadStatus;

  void initSema();
  void initAUDSRV();
  void setSongFormat();
  void unloadSong();
  void rewindSongToStart();
};

}  // namespace Tyra
