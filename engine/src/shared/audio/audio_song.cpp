/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "shared/audio/audio_song.hpp"
#include "shared/debug/debug.hpp"

#include <malloc.h>
#include <cstdlib>
#include <audsrv.h>

namespace Tyra {

const unsigned short AudioSong::chunkSize = 4 * 1024;

AudioSong::AudioSong() {
  chunkReadStatus = 0;
  tyraVolume = 0;
  audsrvVolume = 0;

  songLoaded = false;
  songPlaying = false;
  inLoop = false;
  songFinished = false;

  chunk = nullptr;
}

AudioSong::~AudioSong() {
  if (chunk) delete[] chunk;
}

void AudioSong::init() {
  chunk = static_cast<char*>(memalign(sizeof(char), chunkSize));

  initSema();
  initAUDSRV();
  setSongFormat();
}

void AudioSong::load(const char* t_path) {
  if (songLoaded) unloadSong();
  wav = fopen(t_path, "rb");
  TYRA_ASSERT(wav != nullptr, "Failed to open wav file!");
  rewindSongToStart();
  songLoaded = true;
}

void AudioSong::load(const std::string& t_path) { load(t_path.c_str()); }

void AudioSong::play() {
  TYRA_ASSERT(songLoaded, "Cant play song because was not loaded!");
  if (songFinished) rewindSongToStart();
  tyraVolume = audsrvVolume;
  audsrv_set_volume(tyraVolume);
  songPlaying = true;
}

void AudioSong::stop() {
  tyraVolume = 0;
  audsrv_set_volume(tyraVolume);
  songPlaying = false;
}

const bool& AudioSong::isPlaying() const { return songPlaying; }

const bool& AudioSong::isLoaded() const { return songLoaded; }

const unsigned char& AudioSong::getVolume() const { return tyraVolume; }

std::size_t AudioSong::getListenersCount() const {
  return songListeners.size();
}

void AudioSong::initAUDSRV() {
  int ret = audsrv_on_fillbuf(chunkSize, (audsrv_callback_t)iSignalSema,
                              (void*)fillbufferSema);

  TYRA_ASSERT(ret >= 0,
              "AUDSRV returned error string:", audsrv_get_error_string());
}

/** Initialize semaphore which will wait until chunk of the song is not
 * finished. */
void AudioSong::initSema() {
  TYRA_LOG("Creating audio semaphore");
  sema.init_count = 0;
  sema.max_count = 1;
  sema.option = 0;
  fillbufferSema = CreateSema(&sema);
  TYRA_LOG("AudioSong semaphore created");
}

/**
 * Close file.
 * Delete song path from memory.
 */
void AudioSong::unloadSong() {
  songLoaded = false;
  fclose(wav);
}

/** Fseek on wav. */
void AudioSong::rewindSongToStart() {
  if (wav != nullptr) fseek(wav, 0x30, SEEK_SET);
  chunkReadStatus = 0;
  songFinished = false;
}

/** Set WAV format to 16bit, 22050Hz, stereo. */
void AudioSong::setSongFormat() {
  format.bits = 16;
  format.freq = 22050;
  format.channels = 2;

  if (audsrv_set_format(&format)) {
    TYRA_ERROR("AUDSRV returned error string: ", audsrv_get_error_string());
  }
}

void AudioSong::setVolume(const unsigned char& t_vol) {
  audsrvVolume = t_vol;
  if (songPlaying) tyraVolume = t_vol;

  if (audsrv_set_volume(tyraVolume)) {
    TYRA_ERROR("AUDSRV returned error string: ", audsrv_get_error_string());
  }
}

void AudioSong::work() {
  if (!songPlaying || !songLoaded) return;
  if (songFinished) {
    if (inLoop) {
      for (unsigned int i = 0; i < getListenersCount(); i++)
        songListeners[i]->listener->onAudioFinish();
      rewindSongToStart();
    } else {
      stop();
      return;
    }
  }

  if (chunkReadStatus > 0) {
    WaitSema(fillbufferSema);  // wait until previous chunk wasn't finished
    audsrv_play_audio(chunk, chunkReadStatus);
    for (unsigned int i = 0; i < getListenersCount(); i++)
      songListeners[i]->listener->onAudioTick();
  }

  chunkReadStatus = fread(chunk, 1, chunkSize, wav);

  if (chunkReadStatus < (int)chunkSize) songFinished = true;
}

unsigned int AudioSong::addListener(AudioListener* t_listener) {
  AudioListenerRef* ref = new AudioListenerRef;
  ref->id = rand() % 1000000;
  ref->listener = t_listener;
  songListeners.push_back(ref);
  return ref->id;
}

void AudioSong::removeListener(const unsigned int& t_id) {
  int index = -1;
  for (unsigned int i = 0; i < songListeners.size(); i++)
    if (songListeners[i]->id == t_id) {
      index = i;
      break;
    }

  TYRA_ASSERT(index != -1,
              "Cant remove listener because given id was not found!");

  delete songListeners[index];
  songListeners.erase(songListeners.begin() + index);
}

}  // namespace Tyra
