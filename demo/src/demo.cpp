/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "demo.hpp"

namespace Tyra {

Demo::Demo(Engine* t_engine) { engine = t_engine; }
Demo::~Demo() {}

void Demo::init() { TYRA_LOG("Init!"); }
void Demo::loop() {}

}  // namespace Tyra
