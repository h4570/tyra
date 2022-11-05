/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Wellington Carvalho <wellcoj@gmail.com>
*/

#include "debug/debug.hpp"
#include <tamtypes.h>
#include <cstdio>
#include <kernel.h>
#include <limits.h>
#include <syslimits.h>
#include <unistd.h>
#include <cstring>
#include "file/file_utils.hpp"

namespace Tyra {

FileUtils::FileUtils() {
  getcwd(cwd, sizeof(cwd));
  setPathInfo(cwd);
}

FileUtils::~FileUtils() {}

void FileUtils::setPathInfo(const char* path) {
  char* ptr;

  strcpy(this->elfName, path);
  strcpy(this->elfPath, path);

  ptr = strrchr(this->elfPath, '/');
  if (ptr == nullptr) {
    ptr = strrchr(this->elfPath, '\\');
    if (ptr == nullptr) {
      ptr = strrchr(this->elfPath, ':');
      if (ptr == nullptr) {
        TYRA_TRAP("Did not find path! PATH: ", path);
      }
    }
  }

  ptr++;
  *ptr = '\0';
}

std::string FileUtils::getCwd() {
  std::string result;
  char _cwd[NAME_MAX];
  getcwd(_cwd, sizeof(_cwd));
  result = _cwd;
  return result;
}

std::string FileUtils::fromCwd(const std::string& relativePath) {
  return fromCwd(relativePath.c_str());
}

std::string FileUtils::fromCwd(const char* file) {
  auto cwd = getCwd();
  return cwd + file;
}

std::string FileUtils::getFilenameFromPath(const std::string& path) {
  std::string filename = path.substr(path.find_last_of("/\\") + 1);
  if (filename.size() == path.size()) {
    filename = path.substr(path.find_last_of(":\\") + 1);
  }
  return filename;
}

std::string FileUtils::getPathFromFilename(const std::string& path) {
  std::string basepath = path.substr(0, path.find_last_of("/\\"));
  if (basepath.size() == path.size()) {
    basepath = path.substr(0, path.find_last_of(":\\"));
  }
  return basepath;
}

std::string FileUtils::getFilenameWithoutExtension(
    const std::string& filename) {
  auto lastindex = filename.find_last_of(".");
  return filename.substr(0, lastindex);
}

std::string FileUtils::getExtensionOfFilename(const std::string& filename) {
  auto lastindex = filename.find_last_of(".");
  return filename.substr(lastindex + 1);
}

}  // namespace Tyra
