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

#include "./audio_listener_ref.hpp"
#include "./adpcm_result.hpp"
#include <audsrv.h>
#include <string>

namespace Tyra {

/**
 * Class responsible for playing ADPCM samples.
 * ADPCM sample can't be stopped.
 */
class AudioAdpcm {
 public:
  AudioAdpcm();
  ~AudioAdpcm();

  void init();

  /**
   * Load ADPCM sample.
   * ADPCM sample is an output from "Convert WAV to ADPCM" task (adpenc tool).
   * Adpenc expects 22kHz 16bit file.
   * @param t_path Example: "host:hit.adpcm" or "host:folder/jump.adpcm"
   */
  audsrv_adpcm_t* load(const char* t_path);
  audsrv_adpcm_t* load(const std::string& t_path);

  /**
   * Frees up all memory taken by samples, and stops all voices from
   * being played.
   */
  void reset();

  /**
   * Try play ADPCM sample if channel(s) is not occupied.
   * @param t_adpcm ADPCM data, created by load();
   * @param t_ch Channel (0-23). Type -1 for use any free channel.
   */
  AdpcmResult tryPlay(audsrv_adpcm_t* t_adpcm);
  AdpcmResult tryPlay(audsrv_adpcm_t* t_adpcm, const s8& t_ch);

  /**
   * Play ADPCM sample, if channel is occupied, wait for it.
   * If not used properly, can hugely reduce performance.
   * @param t_adpcm ADPCM data, created by load();
   * @param t_ch Channel (0-23). Type -1 for use any free channel.
   */
  void playWait(audsrv_adpcm_t* t_adpcm);
  void playWait(audsrv_adpcm_t* t_adpcm, const s8& t_ch);

  /**
   * Set ADPCM volume.
   * @param t_vol Value 0-100
   * @param t_ch Channel (0-23)
   */
  void setVolume(const u8& t_vol, const s8& t_ch) {
    audsrv_adpcm_set_volume(t_ch, t_vol);
  }

 private:
  void initAUDSRV();
};

}  // namespace Tyra
