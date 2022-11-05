/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "audio/audio_adpcm.hpp"
#include "debug/debug.hpp"
#include <tamtypes.h>
#include <malloc.h>
#include <kernel.h>
#include <cstdlib>
#include <audsrv.h>
#include "thread/threading.hpp"

namespace Tyra {

AudioAdpcm::AudioAdpcm() {}

AudioAdpcm::~AudioAdpcm() {}

void AudioAdpcm::init() { initAUDSRV(); }

void AudioAdpcm::initAUDSRV() {
  int ret = audsrv_adpcm_init();

  TYRA_ASSERT(ret >= 0,
              "AUDSRV returned error string:", audsrv_get_error_string());
}

void AudioAdpcm::reset() { audsrv_adpcm_init(); }

audsrv_adpcm_t* AudioAdpcm::load(const char* t_path) {
  FILE* file = fopen(t_path, "rb");
  fseek(file, 0, SEEK_END);
  u32 adpcmFileSize = ftell(file);
  u8 data[adpcmFileSize];
  rewind(file);
  fread(data, sizeof(u8), adpcmFileSize, file);
  auto* result = new audsrv_adpcm_t();
  result->size = 0;
  result->buffer = 0;
  result->loop = 0;
  result->pitch = 0;
  result->channels = 0;

  if (audsrv_load_adpcm(result, data, adpcmFileSize)) {
    TYRA_ERROR("AUDSRV returned error string: ", audsrv_get_error_string());
  }

  fclose(file);
  return result;
}

audsrv_adpcm_t* AudioAdpcm::load(const std::string& t_path) {
  return load(t_path.c_str());
}

AdpcmResult AudioAdpcm::tryPlay(audsrv_adpcm_t* t_adpcm) {
  return tryPlay(t_adpcm, -1);
}

AdpcmResult AudioAdpcm::tryPlay(audsrv_adpcm_t* t_adpcm, const s8& t_ch) {
  int res = audsrv_ch_play_adpcm(t_ch, t_adpcm);
  if (res >= 0) {
    return AdpcmResult::ADPCM_OK;
  } else if (res == -AUDSRV_ERR_NO_MORE_CHANNELS) {
    if (t_ch < 0) {
      return AdpcmResult::ADPCM_NO_FREE_CHANNELS;
    } else {
      return AdpcmResult::ADPCM_CHANNEL_USED;
    }
  } else {
    return AdpcmResult::ADPCM_ERROR;
  }
}

void AudioAdpcm::playWait(audsrv_adpcm_t* t_adpcm) { playWait(t_adpcm, -1); }

void AudioAdpcm::playWait(audsrv_adpcm_t* t_adpcm, const s8& t_ch) {
  int res = tryPlay(t_adpcm, t_ch);

  while (res == AdpcmResult::ADPCM_NO_FREE_CHANNELS ||
         res == AdpcmResult::ADPCM_CHANNEL_USED) {
    Threading::switchThread();
    res = tryPlay(t_adpcm, t_ch);
  }
}

}  // namespace Tyra
