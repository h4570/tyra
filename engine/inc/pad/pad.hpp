/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022-2022, tyra - https://github.com/h4570/tyrav2
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
# Wellington Carvalho <wellcoj@gmail.com>
*/

#include <kernel.h>
#include <libpad.h>

#pragma once

namespace Tyra {

struct PadButtons {
  unsigned char Cross, Square, Triangle, Circle, DpadUp, DpadDown, DpadLeft,
      DpadRight, L1, L2, L3, R1, R2, R3, Start, Select;
};

struct PadJoy {
  unsigned char h, v, isCentered, isMoved;
};

/** Class responsible for player pad */
class Pad {
 public:
  Pad();
  ~Pad();

  void init();
  void update();

  inline const PadButtons& getClicked() const { return clicked; }
  inline const PadButtons& getPressed() const { return pressed; }
  inline const PadJoy& getLeftJoyPad() const { return leftJoyPad; }
  inline const PadJoy& getRightJoyPad() const { return rightJoyPad; }

 private:
  char padBuf[256] alignas(sizeof(char) * 256);
  char actAlign[6];
  int actuators, ret, port, slot;
  padButtonStatus buttons;
  unsigned int padData, oldPad, newPad;
  PadButtons pressed, clicked;
  PadJoy leftJoyPad, rightJoyPad;

  void reset();
  void handleClickedButtons();
  void handlePressedButtons();
  int waitPadReady();
  int initPad();
};

}  // namespace Tyra