/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020-2022, tyra - https://github.com/h4570/tyrav2
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
# Sandro Wellinator <wellcoj@gmail.com>
*/

#include <kernel.h>
#include <libpad.h>

#pragma once

namespace Tyra {

struct PadButtons {
  u8 Cross, Square, Triangle, Circle, DpadUp, DpadDown, DpadLeft, DpadRight, L1,
      L2, L3, R1, R2, R3, Start, Select;
};

struct PadJoy {
  u8 h, v, isCentered, isMoved;
};

/** Class responsible for player pad */
class Pad {
 public:
  Pad();
  ~Pad();

  void init();
  void update();

  inline const PadButtons& getClicked() { return clicked; }
  inline const PadButtons& getPressed() { return pressed; }
  inline const PadJoy& getLeftJoyPad() { return leftJoyPad; }
  inline const PadJoy& getRightJoyPad() { return rightJoyPad; }

 private:
  char padBuf[256] alignas(sizeof(char) * 256);
  char actAlign[6];
  int actuators, ret, port, slot;
  padButtonStatus buttons;
  u32 padData, oldPad, newPad;
  PadButtons pressed, clicked;
  PadJoy leftJoyPad, rightJoyPad;

  void reset();
  void handleClickedButtons();
  void handlePressedButtons();
  int waitPadReady();
  int initPad();
};

}  // namespace Tyra