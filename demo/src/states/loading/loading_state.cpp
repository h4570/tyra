/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include <tyra>
#include "states/loading/loading_state.hpp"
#include <memory>

using std::make_unique;
using std::unique_ptr;
using Tyra::FileUtils;
using Tyra::Sprite;

namespace Demo {

LoadingState::LoadingState(Engine* t_engine) : State(t_engine) {
  state = STATE_LOADING;
  _wantFinish = false;
  initialized = false;
}

LoadingState::~LoadingState() {}

void LoadingState::onStart() {
  TYRA_LOG("Loading. RAM: ", engine->info.getAvailableRAM(), "MB");

  auto* repo = &engine->renderer.core.texture.repository;

  auto fillerTexture = repo->add(FileUtils::fromCwd("loading/filler.png"));
  auto loadingTexture = repo->add(FileUtils::fromCwd("loading/loading.png"));

  auto fillerSprite = make_unique<Sprite>();
  auto loadingSprite = make_unique<Sprite>();

  fillerSprite->position.set(0.0F, 0.0F);
  fillerSprite->size.set(600.0F, 600.0F);
  fillerSprite->mode = Tyra::MODE_STRETCH;

  loadingSprite->size.set(128.0F, 64.0F);
  loadingSprite->mode = Tyra::MODE_STRETCH;
  loadingSprite->position.set(5.0F, 448.0F - 64.0F - 5.0F);

  fillerTexture->addLink(fillerSprite->id);
  loadingTexture->addLink(loadingSprite->id);

  engine->renderer.beginFrame();
  engine->renderer.renderer2D.render(fillerSprite.get());
  engine->renderer.renderer2D.render(loadingSprite.get());
  engine->renderer.endFrame();

  repo->free(fillerTexture->id);
  repo->free(loadingTexture->id);

  initialized = true;
}

GlobalStateType LoadingState::onFinish() {
  if (!initialized) return STATE_GAME;

  initialized = false;
  return STATE_GAME;
}

void LoadingState::update() { _wantFinish = true; }

}  // namespace Demo
