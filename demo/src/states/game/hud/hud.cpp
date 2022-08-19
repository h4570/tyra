/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "states/game/hud/hud.hpp"

using Tyra::FileUtils;

namespace Demo {

Hud::Hud(TextureRepository* t_repo) {
  repo = t_repo;

  hpSprite = std::make_unique<Sprite>();
  hpSprite->mode = Tyra::MODE_STRETCH;
  hpSprite->size.set(128.0F, 64.0F);
  hpSprite->position.set(0, 448.0F - 64.0F - 5.0F);

  hpTexture = repo->add(FileUtils::fromCwd("game/health_bar.png"));
  hpTexture->addLink(hpSprite->id);

  crosshairSprite = std::make_unique<Sprite>();
  crosshairSprite->mode = Tyra::MODE_STRETCH;
  crosshairSprite->size.set(16.0F, 16.0F);
  crosshairSprite->position.set(512.0F / 2.0F - 16.0F / 2,
                                448.0F / 2.0F - 16.0F / 2);

  crosshairTexture = repo->add(FileUtils::fromCwd("game/crosshair.png"));
  crosshairTexture->addLink(crosshairSprite->id);
}

Hud::~Hud() {
  repo->free(crosshairTexture->id);
  repo->free(hpTexture->id);
}

}  // namespace Demo
