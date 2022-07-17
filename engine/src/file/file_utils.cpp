/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020 - 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
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
  if (ptr == NULL) {
    ptr = strrchr(this->elfPath, '\\');
    if (ptr == NULL) {
      ptr = strrchr(this->elfPath, ':');
      if (ptr == NULL) {
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

std::string FileUtils::fromCwd(const char* file) {
  auto cwd = getCwd();
  return cwd + file;
}
}  // namespace Tyra
