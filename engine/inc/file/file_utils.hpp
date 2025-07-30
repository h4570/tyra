/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Wellington Carvalho <wellcoj@gmail.com>
*/

#pragma once

#include <string>

namespace Tyra {

class FileUtils {
 public:
  FileUtils();
  ~FileUtils();

  /**
   * Get current working directory.
   * Example: "host:" or "mass:"
   */
  static std::string getCwd();

  /**
   * Add current working directory prefix to path
   * Example: "host:..." or "mass:..."
   */
  static std::string fromCwd(const std::string& relativePath);
  static std::string fromCwd(const char* relativePath);

  static std::string getFilenameFromPath(const std::string& path);
  static std::string getPathFromFilename(const std::string& path);
  static std::string getFilenameWithoutExtension(const std::string& filename);
  static std::string getExtensionOfFilename(const std::string& filename);

  const char* getElfName() const { return elfName; };
  const char* getElfPath() const { return elfPath; };

 private:
  static const int MAX_NAME_LEN = 255;

  // Argv name+path & just path
  char cwd[MAX_NAME_LEN];
  char elfName[MAX_NAME_LEN];
  char elfPath[MAX_NAME_LEN - 14];

  void setPathInfo(const char* path);
};

}  // namespace Tyra
