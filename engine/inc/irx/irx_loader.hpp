/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Wellington Carvalho <wellcoj@gmail.com>
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
# André Guilherme <andregui17@outlook.com>
*/

#pragma once

#include <tamtypes.h>
#include <ps2_all_drivers.h>

namespace Tyra {

class IrxLoader {
 public:
  IrxLoader();
  ~IrxLoader();

  void loadAll(const bool& withUsb, const bool& withHdd, const bool& withCdFS, const bool&withMC, 
               const bool& isLoggingToFile);

  void UnLoad();

 private:
  static bool isLoaded;

  int applyRpcPatches();
  void waitUntilUsbDeviceIsReady();
  void delay(int count);
};

}  // namespace Tyra
