/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "states/intro/intro_state.hpp"
#include "file/file_utils.hpp"
#include "debug/debug.hpp"

#include "states/intro/states/intro_ps2dev_state.hpp"
#include "states/intro/states/intro_tyra_state.hpp"
#include "states/intro/states/intro_war_info_state.hpp"

using Tyra::FileUtils;

namespace Demo {

IntroState::IntroState(Engine* t_engine)
    : State(t_engine), stateManager(STATE_WAR_INFO) {
  state = STATE_INTRO;
  _wantFinish = false;
}

IntroState::~IntroState() {}

void IntroState::onStart() {
  engine->audio.loadSong(FileUtils::fromCwd("intro/intro.wav"));
  engine->audio.setSongLoop(false);
  engine->audio.setSongVolume(80);
  // engine->audio.playSong();

  stateManager.add(new IntroPs2DevState(engine));
  stateManager.add(new IntroTyraState(engine));
  stateManager.add(new IntroWarInfoState(engine));
}

void IntroState::update() { stateManager.update(); }

GlobalStateType IntroState::onFinish() { return STATE_EXIT; }

}  // namespace Demo
