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

#include <draw.h>
#include <gs_privileged.h>
#include "./paths/path3/path3.hpp"
#include "./paths/path1/path1.hpp"

namespace Tyra {

/**
 * Synchronization class.
 * Mainly between VU1 and EE.
 *
 * For example you can set texture, render X vertices, then add() wait, and
 * wait() for it. Without it, there is risk for example to send new texture
 * during drawing with previous one.
 */
class RendererCoreSync {
 public:
  RendererCoreSync();
  ~RendererCoreSync();

  void init(Path3* path3, Path1* path1);

  // --- Auto

  /** clear() -> sendPath1Req() -> waitAndClear() */
  void align3D();

  /** clear() -> sendPath3Req() -> waitAndClear() */
  void align2D();

  // --- Manual

  u8 check();
  void clear();
  void waitAndClear();
  void sendPath1Req();
  void sendPath3Req();

  void addPath1Req(packet2_t* packet);

 private:
  Path3* path3;
  Path1* path1;
};

}  // namespace Tyra
