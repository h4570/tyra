/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/3d/pipeline/static/core/stapip_programs_repository.hpp"

namespace Tyra {

StapipProgramsRepository::StapipProgramsRepository() {}

StapipProgramsRepository::~StapipProgramsRepository() {}

StapipVU1Program* StapipProgramsRepository::getProgram(
    const StapipProgramName& name) {
  switch (name) {
    case StapipProgramName::StapipAsIsColor:
      return &asIsColor;
    case StapipProgramName::StapipCullColor:
      return &cullColor;

    case StapipProgramName::StapipAsIsDirLights:
      return &asIsLightingColor;
    case StapipProgramName::StapipCullDirLights:
      return &cullLightingColor;

    case StapipProgramName::StapipAsIsTextureDirLights:
      return &asIsLightingTextureColor;
    case StapipProgramName::StapipCullTextureDirLights:
      return &cullLightingTextureColor;

    case StapipProgramName::StapipAsIsTextureColor:
      return &asIsTextureColor;
    case StapipProgramName::StapipCullTextureColor:
      return &cullTextureColor;

    default:
      TYRA_TRAP("Unknown VU1 program name");
      return &cullLightingTextureColor;
  }
}

}  // namespace Tyra
