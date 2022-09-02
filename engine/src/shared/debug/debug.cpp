/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Wellington Carvalho <wellcoj@gmail.com>
*/

#include "shared/debug/debug.hpp"

std::unique_ptr<std::ofstream> TyraDebug::logFile;

std::ofstream* TyraDebug::getLogFile() {
  if (logFile) {
    return logFile.get();
  } else {
    logFile = std::make_unique<std::ofstream>();
    logFile->open(Tyra::FileUtils::fromCwd("log.txt"),
                  std::ofstream::out | std::ofstream::app);
    return logFile.get();
  }

}  // namespace Tyra
