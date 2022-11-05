/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

#include <tyra>
#include <optional>

using Tyra::Ray;

namespace Demo {

struct PlayerShootAction {
  bool isShooting = false;
  std::optional<Ray> ray;
};

}  // namespace Demo
