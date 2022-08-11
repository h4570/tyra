/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022-2022, Tyra - https://github.com/h4570/tyrav2
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
# Wellinator Carvalho <wellcoj@gmail.com>
*/

#pragma once

#include "./audio_listener.hpp"
#include <audsrv.h>
#include <string>

namespace Tyra {

struct AudioListenerRef {
  AudioListener* listener;
  u32 id;
};

/**
 * Class responsible for audio playing.
 * There are two main features:
 * - WAV song (background music)
 * - ADPCM samples
 */
class Audio {
 public:
  /** Constructor is called by the engine. */
  Audio();
  ~Audio();

  void init();

  /**
   * Load 24bit 22050Hz Stereo WAV.
   * Can be used multiple times for song switching.
   * @param t_path Example: "host:song.wav" or "host:folder/song.wav"
   */
  void loadSong(const char* t_path);

  /**
   * Load 24bit 22050Hz Stereo WAV.
   * Can be used multiple times for song switching.
   * @param t_path Example: "host:song.wav" or "host:folder/song.wav"
   */
  inline void loadSong(const std::string& t_path) { loadSong(t_path.c_str()); }

  void playSong();

  void stopSong();

  const u8& isSongPlaying() const { return songPlaying; }

  const u8& isSongLoaded() const { return songLoaded; }

  /**
   * Set song loop/noloop.
   * @returns Looping state is  0 - noloop / 1 - loop
   */
  const u8& isSongInLoop() const { return songInLoop; }

  /**
   * Set song loop/noloop.
   * @param t_set false - noloop, true - loop
   */
  void setSongLoop(const u8& t_set) { songInLoop = t_set; }

  const u8& getVolume() const { return tyraVolume; }

  /**
   * Set song volume.
   * @param t_vol Value 0-100
   */
  void setSongVolume(const u8& t_vol);

  /**
   * If you want to synchronize some actions with
   * background song, add audio listener.
   * @returns Listener id.
   */
  u32 addSongListener(AudioListener* t_listener);

  /** Remove song listener. */
  void removeSongListener(const u32& t_id);

  /**
   * Load ADPCM sample. ADPCM sample is an output from
   * "adpenc" tool, shipped with PS2SDK.
   * @param t_path Example: "host:hit.adpcm" or "host:folder/jump.adpcm"
   */
  audsrv_adpcm_t* loadADPCM(const char* t_path);

  /**
   * Load ADPCM sample. ADPCM sample is an output from
   * "adpenc" tool, shipped with PS2SDK.
   * @param t_path Example: "host:hit.adpcm" or "host:folder/jump.adpcm"
   */
  inline audsrv_adpcm_t* loadADPCM(const std::string& t_path) {
    return loadADPCM(t_path.c_str());
  }

  /**
   * Play ADPCM sample.
   * ADPCM sample can't be stopped.
   * @param t_adpcm ADPCM data, created by loadADPCM();
   */
  void playADPCM(audsrv_adpcm_t* t_adpcm);

  /**
   * Play ADPCM sample.
   * ADPCM sample can't be stopped.
   * @param t_adpcm ADPCM data, created by loadADPCM();
   * @param t_ch Channel (0-23). Type -1 for use any free channel.
   */
  void playADPCM(audsrv_adpcm_t* t_adpcm, const s8& t_ch);

  /**
   * Set ADPCM volume.
   * @param t_vol Value 0-100
   * @param t_ch Channel (0-23)
   */
  void setADPCMVolume(const u8& t_vol, const s8& t_ch) {
    audsrv_adpcm_set_volume(t_ch, t_vol);
  }

  void work();

 private:
  u8 songLoaded, tyraVolume, audsrvVolume, songPlaying, songInLoop,
      songFinished;
  FILE* wav;
  audsrv_fmt_t format;
  std::vector<AudioListenerRef*> songListeners;

  ee_thread_t thread;
  int threadId;
  static const u16 threadStackSize;
  u8* threadStack;

  ee_sema_t sema;
  s32 fillbufferSema;
  static const u16 chunkSize;
  char* chunk;
  s32 chunkReadStatus;

  void setSongFormat();
  void unloadSong();
  void rewindSongToStart();
  u32 getSongListenersCount() const { return songListeners.size(); }

  void initSema();
  void initAUDSRV();
  void initThread();
};

}  // namespace Tyra
