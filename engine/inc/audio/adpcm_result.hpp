/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, Tyra - https://github.com/h4570/tyrav2
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
# Wellington Carvalho <wellcoj@gmail.com>
*/

#pragma once

namespace Tyra {

enum AdpcmResult {
  ADPCM_OK,

  /** When provided channel by user is currently in use */
  ADPCM_CHANNEL_USED,

  /** When user did not provided channel and all are in use */
  ADPCM_NO_FREE_CHANNELS,

  ADPCM_ERROR
};

}  // namespace Tyra
