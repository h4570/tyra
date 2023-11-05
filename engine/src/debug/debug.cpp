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

void TyraDebug::writeInLogFile(std::stringstream* ss) {
  std::ofstream logFile;
  logFile.open(Tyra::FileUtils::fromCwd("log.txt"),
               std::ofstream::out | std::ofstream::app);
  logFile << ss->str();
  logFile.flush();
  // logFile.close();
}
