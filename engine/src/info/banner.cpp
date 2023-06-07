/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022-2023, tyra - https://github.com/h4570/tyrav2
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
# André Guilherme <andregui17@outlook.com>
*/

#include "info/banner.hpp"
#include "info/version.hpp"
#include <iostream>
#include <cstring>
#include "./banner_data.cpp"

namespace Tyra {

Banner::Banner() {}

Banner::~Banner() {}

void Banner::show(Renderer* renderer) {
  auto* bannerData = ___createTyraSplashBanner();

  TextureBuilderData tbd;
  tbd.bpp = bpp32;
  tbd.gsComponents = TEXTURE_COMPONENTS_RGBA;
  tbd.width = 128;
  tbd.height = 32;
  tbd.clut = nullptr;
  tbd.data = reinterpret_cast<unsigned char*>(bannerData);

  Sprite sprite;
  sprite.size.x = 128;
  sprite.size.y = 32;
  sprite.position.x =
      (renderer->core.getSettings().getWidth() / 2) - (sprite.size.x / 2);
  sprite.position.y =
      (renderer->core.getSettings().getHeight() / 2) - (sprite.size.y / 2);

  auto texture = Texture(&tbd);
  texture.addLink(sprite.id);
  renderer->core.texture.repository.add(&texture);

  for (int i = 0; i < 2; i++) {
    renderer->beginFrame();
    renderer->renderer2D.render(&sprite);
    renderer->endFrame();
  }

  renderer->core.texture.repository.removeById(texture.id);
  texture.core->data = nullptr;
  delete[] bannerData;

  std::cout << "\n";
  std::cout << "-----------------------------------------\n";
  std::cout << "        _____        ____   ___\n";
  std::cout << "          |     \\/   ____| |___|\n";
  std::cout << "          |     |   |   \\  |   |\n";
  std::cout << "-----------------------------------------\n";
  std::cout << "Copyright 2022 - 2023\n";
  std::cout << "Repository: https://github.com/h4570/tyra\n";
  std::cout << "Licensed under Apache License 2.0\n";
  std::cout << "Version: ";
  std::cout << Version::toString().c_str();
  std::cout << "\n";
  std::cout << "-----------------------------------------\n";
  std::cout << "\n";
}

}  // Namespace Tyra
