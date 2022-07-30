/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Wellington Carvalho <wellcoj@gmail.com>
*/

#pragma once

#include <string>

namespace Tyra {

class FileUtils {
 public:
  FileUtils();
  ~FileUtils();

  static std::string getCwd();
  static std::string fromCwd(const std::string& relativePath);
  static std::string fromCwd(const char* relativePath);

  const char* getElfName() const { return elfName; };
  const char* getElfPath() const { return elfPath; };

 private:
  // Argv name+path & just path
  char cwd[NAME_MAX];
  char elfName[NAME_MAX];
  char elfPath[NAME_MAX - 14];

  void setPathInfo(const char* path);
};

}  // namespace Tyra
