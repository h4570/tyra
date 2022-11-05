/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyrav2
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
# Wellington Carvalho <wellcoj@gmail.com>
# André Guilherme <andregui17@outlook.com>
*/
#include <xtl.h>
#include <stdint.h>

#pragma once

namespace Tyra {

struct PadButtons {
  uint8_t A, X, Y, B, DpadUp, DpadDown, DpadLeft, DpadRight, LB,
      LT, LS, RB, RT, RS, Start, Back;
};

struct PadJoy {
  uint8_t h, v, isCentered, isMoved;
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
  char actAlign[6];
  int actuators, ret, port, slot;
  PadButtons pressed, clicked;
  PadJoy leftJoyPad, rightJoyPad;

  void reset();
  void handleClickedButtons();
  void handlePressedButtons();
  int waitPadReady();
  int initPad();
};

}  // namespace Tyra