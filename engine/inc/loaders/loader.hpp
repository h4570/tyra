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

#include <string>

namespace Tyra {

class Loader {
 protected:
  static std::string getFilenameFromPath(const std::string& path);
  static std::string getPathFromFilename(const std::string& path);
  static std::string getFilenameWithoutExtension(const std::string& filename);
  static std::string getExtensionOfFilename(const std::string& filename);
};

}  // namespace Tyra
