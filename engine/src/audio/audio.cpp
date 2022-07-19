/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020 - 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "debug/debug.hpp"
#include <tamtypes.h>
#include <vector>
#include <stdio.h>
#include <malloc.h>
#include <kernel.h>
#include <cstdlib>
#include <loadfile.h>
#include <sifrpc.h>
#include <sbv_patches.h>
#include <audsrv.h>
#include <string.h>
#include "thread/threading.hpp"
#include "audio/audio.hpp"

extern void* _gp;

void audioThread(Tyra::Audio* audio) {
  while (true) audio->work();
}

namespace Tyra {

const u16 Audio::chunkSize = 4 * 1024;
const u16 Audio::threadStackSize = 2 * 1024;

Audio::Audio() {
  chunkReadStatus = 0;
  tyraVolume = 0;
  audsrvVolume = 0;

  songLoaded = false;
  songPlaying = false;
  songInLoop = false;
  songFinished = false;

  chunk = nullptr;
  threadStack = nullptr;
}

Audio::~Audio() {
  if (chunk) delete[] chunk;
  if (threadStack) delete[] threadStack;
}

void Audio::init() {
  chunk = static_cast<char*>(memalign(sizeof(char), chunkSize));
  threadStack = static_cast<u8*>(memalign(sizeof(u8), threadStackSize));

  initSema();
  initAUDSRV();
  setSongFormat();
  initThread();
}

void Audio::loadSong(const char* t_path) {
  if (songLoaded) unloadSong();
  wav = fopen(t_path, "rb");
  TYRA_ASSERT(wav != NULL, "Failed to open wav file!");
  rewindSongToStart();
  songLoaded = true;
  TYRA_LOG("Song loaded!");
}

void Audio::playSong() {
  TYRA_ASSERT(songLoaded, "Cant play song because was not loaded!");
  if (songFinished) rewindSongToStart();
  tyraVolume = audsrvVolume;
  audsrv_set_volume(tyraVolume);
  songPlaying = true;
}

void Audio::stopSong() {
  tyraVolume = 0;
  audsrv_set_volume(tyraVolume);
  songPlaying = false;
}

/**
 * Initialize AUDSRV main and ADPCM module.
 * Install AUDSRV callback.
 */
void Audio::initAUDSRV() {
  TYRA_LOG("Initializing AUDSRV");

  int ret = audsrv_init();
  TYRA_ASSERT(ret >= 0,
              "AUDSRV returned error string: ", audsrv_get_error_string());

  ret = audsrv_adpcm_init();
  TYRA_ASSERT(ret >= 0,
              "AUDSRV returned error string: ", audsrv_get_error_string());

  ret = audsrv_on_fillbuf(chunkSize, (audsrv_callback_t)iSignalSema,
                          (void*)fillbufferSema);

  TYRA_ASSERT(ret >= 0,
              "AUDSRV returned error string:", audsrv_get_error_string());

  TYRA_LOG("AUDSRV initialized!");
}

/** Initialize semaphore which will wait until chunk of the song is not
 * finished. */
void Audio::initSema() {
  TYRA_LOG("Creating audio semaphore");
  sema.init_count = 0;
  sema.max_count = 1;
  sema.option = 0;
  fillbufferSema = CreateSema(&sema);
  TYRA_LOG("Audio semaphore created");
}

void Audio::initThread() {
  thread.gp_reg = &_gp;
  thread.func = reinterpret_cast<void*>(audioThread);
  thread.stack = threadStack;
  thread.stack_size = threadStackSize;
  thread.initial_priority = 0x5;
  threadId = CreateThread(&thread);
  TYRA_ASSERT(threadId >= 0, "Create audio thread failed!");
  StartThread(threadId, this);
}

/**
 * Close file.
 * Delete song path from memory.
 */
void Audio::unloadSong() {
  songLoaded = false;
  fclose(wav);
}

/** Fseek on wav. */
void Audio::rewindSongToStart() {
  if (wav != NULL) fseek(wav, 0x30, SEEK_SET);
  chunkReadStatus = 0;
  songFinished = false;
}

/** Set WAV format to 16bit, 22050Hz, stereo. */
void Audio::setSongFormat() {
  format.bits = 16;
  format.freq = 22050;
  format.channels = 2;
  audsrv_set_format(&format);
}

void Audio::setSongVolume(const u8& t_vol) {
  audsrvVolume = t_vol;
  if (songPlaying) tyraVolume = t_vol;
  audsrv_set_volume(tyraVolume);
}

void Audio::work() {
  Threading::switchThread();

  if (!songPlaying || !songLoaded) return;
  if (songFinished) {
    TYRA_LOG("Audio: Song finished. ");
    if (songInLoop) {
      TYRA_LOG("Running again.");
      for (u32 i = 0; i < getSongListenersCount(); i++)
        songListeners[i]->listener->onAudioFinish();
      rewindSongToStart();
    } else {
      TYRA_LOG("Stopping song.");
      stopSong();
      return;
    }
  }

  if (chunkReadStatus > 0) {
    WaitSema(fillbufferSema);  // wait until previous chunk wasn't finished
    audsrv_play_audio(chunk, chunkReadStatus);
    for (u32 i = 0; i < getSongListenersCount(); i++)
      songListeners[i]->listener->onAudioTick();
  }

  chunkReadStatus = fread(chunk, 1, chunkSize, wav);

  if (chunkReadStatus < (s32)chunkSize) songFinished = true;
}

u32 Audio::addSongListener(AudioListener* t_listener) {
  AudioListenerRef* ref = new AudioListenerRef;
  ref->id = rand() % 1000000;
  ref->listener = t_listener;
  songListeners.push_back(ref);
  return ref->id;
}

void Audio::removeSongListener(const u32& t_id) {
  s32 index = -1;
  for (u32 i = 0; i < songListeners.size(); i++)
    if (songListeners[i]->id == t_id) {
      index = i;
      break;
    }
  TYRA_ASSERT(index != -1,
              "Cant remove listener because given id was not found!");
  delete songListeners[index];
  songListeners.erase(songListeners.begin() + index);
}

audsrv_adpcm_t* Audio::loadADPCM(const char* t_path) {
  FILE* file = fopen(t_path, "rb");
  fseek(file, 0, SEEK_END);
  u32 adpcmFileSize = ftell(file);
  u8 data[adpcmFileSize];
  rewind(file);
  fread(data, sizeof(u8), adpcmFileSize, file);
  audsrv_adpcm_t* result = new audsrv_adpcm_t();
  result->size = 0;
  result->buffer = 0;
  result->loop = 0;
  result->pitch = 0;
  result->channels = 0;
  if (audsrv_load_adpcm(result, data, adpcmFileSize)) {
    TYRA_LOG("AUDSRV returned error string: ", audsrv_get_error_string());
    TYRA_TRAP("audsrv_load_adpcm() failed!");
  }
  fclose(file);
  return result;
}

void Audio::playADPCM(audsrv_adpcm_t* t_adpcm) {
  if (audsrv_play_adpcm(t_adpcm)) {
    TYRA_LOG("AUDSRV returned error string: ", audsrv_get_error_string());
    TYRA_TRAP("audsrv_play_adpcm() failed!");
  }
}

void Audio::playADPCM(audsrv_adpcm_t* t_adpcm, const s8& t_ch) {
  if (audsrv_ch_play_adpcm(t_ch, t_adpcm)) {
    TYRA_LOG("AUDSRV returned error string: ", audsrv_get_error_string());
    TYRA_TRAP("audsrv_ch_play_adpcm() failed!");
  }
}

}  // namespace Tyra
