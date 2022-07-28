/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

#include "renderer/renderer_settings.hpp"

namespace Tyra {

class RendererCoreGS {
 public:
  RendererCoreGS();
  ~RendererCoreGS();

  prim_t prim;
  lod_t lod;
  zbuffer_t zBuffer;

  void init(RendererSettings* settings);

  void flipBuffers();

  float getVRamFreeSpaceInMB() const {
    return 4.0F - spaceOccupiedByFrameBuffers - spaceOccupiedByZBuffer -
           vramMargin;
  }

 private:
  constexpr static float gsCenter = 4096.0F;
  constexpr static float screenCenter = gsCenter / 2.0F;
  constexpr static float vramMargin =
      0.003F;  // We don't have perfectly 4MB of VRAM

  float spaceOccupiedByFrameBuffers, spaceOccupiedByZBuffer;
  RendererSettings* settings;
  framebuffer_t frameBuffers[2];
  packet2_t* flipPacket;
  u8 context;
  u8 currentField;

  void allocateBuffers();
  void initDrawingEnvironment();
  void setPrim();
  void setLod();
  void initChannels();
  void updateCurrentField();
  qword_t* setXYOffset(qword_t* q, const int& drawContext, const float& x,
                       const float& y);
};

}  // namespace Tyra
