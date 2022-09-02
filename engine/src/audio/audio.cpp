/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "audio/audio.hpp"
#include "debug/debug.hpp"
#include "thread/threading.hpp"
#include <kernel.h>
#include <malloc.h>

extern void* _gp;

void audioThread(Tyra::Audio* audio) {
  while (true) audio->work();
}

namespace Tyra {

const u16 Audio::threadStackSize = 2 * 1024;

Audio::Audio() {
  threadStack = static_cast<u8*>(memalign(sizeof(u8), threadStackSize));
}

Audio::~Audio() {
  if (threadStack) delete[] threadStack;
}

void Audio::init() {
  initAUDSRV();

  song.init();
  adpcm.init();

  initThread();

  TYRA_LOG("Audio initialized!");
}

void Audio::initAUDSRV() {
  int ret = audsrv_init();

  TYRA_ASSERT(ret >= 0,
              "AUDSRV returned error string: ", audsrv_get_error_string());
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

void Audio::work() {
  Threading::switchThread();

  song.work();
}

}  // namespace Tyra
