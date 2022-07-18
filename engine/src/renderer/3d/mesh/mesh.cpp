
/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/3d/mesh/mesh.hpp"

namespace Tyra {

Mesh::Mesh() { id = rand() % 1000000; }

Mesh::~Mesh() {}

}  // namespace Tyra
