
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

Mesh::Mesh(const u8& isMother) {
  id = rand() % 1000000;
  _isMother = isMother;
  translation.translate(Vec4(0.0F, 0.0F, 0.0F, 1.0F));
}

M4x4 Mesh::getModelMatrix() const { return translation * rotation * scale; }

Mesh::~Mesh() {}

}  // namespace Tyra
