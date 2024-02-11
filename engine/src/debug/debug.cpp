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

bool EESIO_Initialized = false;
void TyraDebug::writeInLogFile(std::stringstream* ss) {
  std::ofstream logFile;
  logFile.open(Tyra::FileUtils::fromCwd("log.txt"),
               std::ofstream::out | std::ofstream::app);
  logFile << ss->str();
  logFile.flush();
  // logFile.close();
}

void TyraDebug::initializeEESIO() {
  if (EESIO_Initialized)
    return;
#ifndef EESIO_UART_USE_SIOCOOKIE
  sio_init(38400, 0, 0, 0, 0);
  sio_putsn("TYRA: EE_SIO Enabled\n");
#else
  ee_sio_start(38400, 0, 0, 0, 0, 1); // alternative wrapper. initializes UART, but also re-routes STDOUT and STDERR FILE* streams to EE_SIO
  printf("TYRA: EE_SIO Enabled & STDOUT/STDERR hooked\n")
#endif
  EESIO_Initialized = true;
}
