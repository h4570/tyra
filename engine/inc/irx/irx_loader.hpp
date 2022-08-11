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

#include <tamtypes.h>

namespace Tyra {

class IrxLoader {
 public:
  IrxLoader();
  ~IrxLoader();

  /** Load's audio and pad driver */
  void loadDefaultDrivers();

  void loadUSBDriver();

 private:
  int applyRpcPatches();
  int loadAudio();
  int loadPad();
  int loadUsb();
  void waitUntilUsbDeviceIsReady();
  void delay(int count);
};

}  // namespace Tyra
