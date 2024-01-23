/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Guido Diego Quispe Robles
*/

#include <tyra>
#include "tutorial_11.hpp"

namespace Tyra {

Tutorial11::Tutorial11(Engine* t_engine) : engine(t_engine) {}

Tutorial11::~Tutorial11() {
  /*
   * It is not necessary to use it here, it is destroyed when the font is
   * destroyed but in case you stop using it, is good to use it to free up
   * memory.
  */
  unloadFont(&font);
}

void Tutorial11::init() {
  engine->renderer.setClearScreenColor(Color(32.0F, 32.0F, 32.0F));
  // setMaxSizeInFontMemory(2.0f); // by default
  loadFont(&font, FileUtils::fromCwd("roboto-black.ttf"), 32);
  
  white = Color(255.0f, 255.0f, 255.0f, 128.0f);
  skyBlue = Color(32.0f, 164.0f, 243.0f, 128.0f);
}

void Tutorial11::loop() {
  auto& renderer = engine->renderer;

  /** Begin frame will clear our screen. */
  renderer.beginFrame();

  /** Render sprite font ttf. */
  printText(&font, "Hello world in", 20, 60, 32, white);
  printText(&font, "TYRA", 212, 60, 32, skyBlue);

  /** End frame will perform vsync. */
  renderer.endFrame();
}

}  // namespace Tyra
