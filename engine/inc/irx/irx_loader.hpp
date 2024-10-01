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
#include <map>
#include <string>

namespace Tyra {

class IrxLoader {
 public:
  IrxLoader();
  ~IrxLoader();

  void loadAll(const bool& withUsb, const bool& isLoggingToFile);

 private:
  static bool isLoaded;

  void loadSio2man(const bool& verbose);
  void loadPadman(const bool& verbose);
  void loadLibsd(const bool& verbose);
  void loadIO(const bool& verbose);
  void loadUsbModules(const bool& verbose);
  void loadAudsrv(const bool& verbose);

  int applyRpcPatches();
  void waitUntilUsbDeviceIsReady();
  void delay(int count);
  std::string GetIrxErrorDescription(const int ID, const int RET);
  static std::map<int, std::string> IOPErrors;
};

}  // namespace Tyra
