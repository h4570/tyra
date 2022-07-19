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

  void init(Path3* path3);

  /** Send draw finish tag via VU1 to GS. */
  void add();
  /** Check if finish flag was set by GS. */
  u8 check();
  /** Clear finish tag flag. */
  void clear();
  /** Wait for finish flag and clear it. */
  void waitAndClear();

  /** Clear() -> Add() -> waitAndClear() */
  void align();

 private:
  Path3* path3;
};

}  // namespace Tyra
