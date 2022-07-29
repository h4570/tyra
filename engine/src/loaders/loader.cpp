
/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "loaders/loader.hpp"

namespace Tyra {

std::string Loader::getFilenameFromPath(const std::string& path) {
  std::string filename = path.substr(path.find_last_of("/\\") + 1);
  if (filename.size() == path.size()) {
    filename = path.substr(path.find_last_of(":\\") + 1);
  }
  return filename;
}

std::string Loader::getFilenameWithoutExtension(const std::string& filename) {
  auto lastindex = filename.find_last_of(".");
  return filename.substr(0, lastindex);
}

std::string Loader::getExtensionOfFilename(const std::string& filename) {
  auto lastindex = filename.find_last_of(".");
  return filename.substr(lastindex + 1);
}

}  // namespace Tyra
