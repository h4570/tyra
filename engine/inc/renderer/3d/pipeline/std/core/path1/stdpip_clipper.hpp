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

#include <vector>
#include "debug/debug.hpp"
#include "./stdpip_qbuffer.hpp"
#include "renderer/renderer_settings.hpp"
#include "renderer/core/paths/path1/clipper/path1_ee_clip_algorithm.hpp"

namespace Tyra {

/**
 * @brief This class requires VU1 buffer with max buffsize/3 vertices.
 * It will clip triangles and fill the buffer.
 *
 * To be honest clipping algorithm should be moved to VU1 and "AsIs" VU1 program
 * should be renamed to "Clip" - I don't want to do it now, too much time.
 */
class StdpipClipper {
 public:
  StdpipClipper();
  ~StdpipClipper();
  void clip(StdpipQBuffer* buffer);
  void init(const RendererSettings& settings);
  void setMaxVertCount(const u32& count);
  void setMVP(M4x4* mvp);

 private:
  u32 maxVertCount;
  Path1EEClipAlgorithm algorithm;
  M4x4* mvp;

  void perspectiveDivide(std::vector<Path1ClipVertex>* vertices);
  void moveDataToBuffer(const std::vector<Path1ClipVertex>& vertices,
                        StdpipQBuffer* buffer);
};

}  // namespace Tyra
