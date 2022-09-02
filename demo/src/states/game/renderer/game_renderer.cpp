/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "states/game/renderer/game_renderer.hpp"

using Tyra::Threading;

namespace Demo {

GameRenderer::GameRenderer(Renderer* t_renderer) {
  renderer = t_renderer;

  stpip.setRenderer(&renderer->core);
  dypip.setRenderer(&renderer->core);
}

GameRenderer::~GameRenderer() {}

void GameRenderer::add(std::vector<RendererStaticPair*> t_staticPairs) {
  staticPairs.insert(staticPairs.end(), t_staticPairs.begin(),
                     t_staticPairs.end());
}

void GameRenderer::add(std::vector<RendererDynamicPair*> t_dynamicPairs) {
  dynamicPairs.insert(dynamicPairs.end(), t_dynamicPairs.begin(),
                      t_dynamicPairs.end());
}

void GameRenderer::add(Sprite* sprite) { sprites.push_back(sprite); }

void GameRenderer::add(const CoreBBox& bbox) { bboxes.push_back(bbox); }

void GameRenderer::add(RendererStaticPair* staticPair) {
  staticPairs.push_back(staticPair);
}

void GameRenderer::add(RendererDynamicPair* dynamicPair) {
  dynamicPairs.push_back(dynamicPair);
}

void GameRenderer::clear() {
  staticPairs.clear();
  dynamicPairs.clear();
  sprites.clear();
  bboxes.clear();
}

void GameRenderer::render() {
  // Render static stuff
  if (staticPairs.size()) {
    renderer->renderer3D.usePipeline(&stpip);

    for (auto& pair : staticPairs) {
      stpip.render(pair->mesh, pair->options);
    }
  }

  Threading::switchThread();  // give some time for audio thread

  // Render animated stuff
  if (dynamicPairs.size()) {
    renderer->renderer3D.usePipeline(&dypip);

    for (auto& pair : dynamicPairs) {
      dypip.render(pair->mesh, pair->options);
    }
  }

  Threading::switchThread();

  // Render debug stuff after stapip/dynpip, otherwise it will not be visible
  for (auto& bbox : bboxes) {
    renderer->renderer3D.utility.drawBBox(bbox);
  }

  // Render 2D
  for (auto& sprite : sprites) {
    renderer->renderer2D.render(sprite);
  }
}

}  // namespace Demo
