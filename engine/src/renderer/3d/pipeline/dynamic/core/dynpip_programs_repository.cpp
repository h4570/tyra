/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/3d/pipeline/dynamic/core/dynpip_programs_repository.hpp"

namespace Tyra {

DynPipProgramsRepository::DynPipProgramsRepository() {}

DynPipProgramsRepository::~DynPipProgramsRepository() {}

DynPipVU1Program* DynPipProgramsRepository::getProgramByParams(
    const bool& isLightingEnabled, const bool& isTextureEnabled) {
  if (isLightingEnabled && isTextureEnabled)
    return &textureDirLights;
  else if (isLightingEnabled)
    return &dirLights;
  else if (isTextureEnabled)
    return &textureColor;
  else
    return &color;
}

DynPipVU1Program* DynPipProgramsRepository::getProgram(
    const DynPipProgramName& name) {
  switch (name) {
    case DynPipProgramName::DynPipColor:
      return &color;
    case DynPipProgramName::DynPipDirLights:
      return &dirLights;
    case DynPipProgramName::DynPipTextureDirLights:
      return &textureDirLights;
    case DynPipProgramName::DynPipTextureColor:
      return &textureColor;

    default:
      TYRA_TRAP("Unknown VU1 program name");
      return &color;
  }
}

}  // namespace Tyra
