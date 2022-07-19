/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020 - 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/3d/pipeline/std/core/path1/stdpip_programs_repository.hpp"

namespace Tyra {

StdpipProgramsRepository::StdpipProgramsRepository() {}

StdpipProgramsRepository::~StdpipProgramsRepository() {}

StdpipVU1Program* StdpipProgramsRepository::getProgram(
    const StdpipProgramName& name) {
  switch (name) {
    case StdpipProgramName::StdpipAsIsColor:
      return &asIsColor;
    case StdpipProgramName::StdpipCullColor:
      return &cullColor;

    case StdpipProgramName::StdpipAsIsDirLights:
      return &asIsLightingColor;
    case StdpipProgramName::StdpipCullDirLights:
      return &cullLightingColor;

    case StdpipProgramName::StdpipAsIsTextureDirLights:
      return &asIsLightingTextureColor;
    case StdpipProgramName::StdpipCullTextureDirLights:
      return &cullLightingTextureColor;

    case StdpipProgramName::StdpipAsIsTextureColor:
      return &asIsTextureColor;
    case StdpipProgramName::StdpipCullTextureColor:
      return &cullTextureColor;

    default:
      TYRA_TRAP("Unknown VU1 program name");
      return &cullLightingTextureColor;
  }
}

}  // namespace Tyra
