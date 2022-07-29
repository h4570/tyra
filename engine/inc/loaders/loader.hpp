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

#include <string>

namespace Tyra {

class Loader {
 protected:
  std::string getFilenameFromPath(const std::string& path);
  std::string getFilenameWithoutExtension(const std::string& filename);
  std::string getExtensionOfFilename(const std::string& filename);
};

}  // namespace Tyra
