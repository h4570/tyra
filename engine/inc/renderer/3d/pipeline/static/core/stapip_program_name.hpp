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

namespace Tyra {

enum StaPipProgramName {
  StdipUndefinedProgram,

  StaPipCullColor,
  StaPipAsIsColor,

  StaPipCullDirLights,
  StaPipAsIsDirLights,

  StaPipCullTextureDirLights,
  StaPipAsIsTextureDirLights,

  StaPipCullTextureColor,
  StaPipAsIsTextureColor,
};

}  // namespace Tyra
