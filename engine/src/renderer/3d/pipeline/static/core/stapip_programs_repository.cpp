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

StaPipProgramsRepository::StaPipProgramsRepository() {}

StaPipProgramsRepository::~StaPipProgramsRepository() {}

StaPipVU1Program* StaPipProgramsRepository::getProgram(
    const StaPipProgramName& name) {
  switch (name) {
    case StaPipProgramName::StaPipAsIsColor:
      return &asIsColor;
    case StaPipProgramName::StaPipCullColor:
      return &cullColor;

    case StaPipProgramName::StaPipAsIsDirLights:
      return &asIsLightingColor;
    case StaPipProgramName::StaPipCullDirLights:
      return &cullLightingColor;

    case StaPipProgramName::StaPipAsIsTextureDirLights:
      return &asIsLightingTextureColor;
    case StaPipProgramName::StaPipCullTextureDirLights:
      return &cullLightingTextureColor;

    case StaPipProgramName::StaPipAsIsTextureColor:
      return &asIsTextureColor;
    case StaPipProgramName::StaPipCullTextureColor:
      return &cullTextureColor;

    default:
      TYRA_TRAP("Unknown VU1 program name");
      return &cullLightingTextureColor;
  }
}

}  // namespace Tyra
