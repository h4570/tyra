/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0F
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/3d/pipeline/minecraft/data/mcpip_multi_tex_block_data.hpp"

namespace Tyra {

McpipMultiTexBlockData::McpipMultiTexBlockData() {
  allocateTempData();
  unroll();
  dellocateTempData();
}

McpipMultiTexBlockData::~McpipMultiTexBlockData() {
  if (comboData != nullptr) {
    delete[] comboData;
  }
}

void McpipMultiTexBlockData::allocateTempData() {
  u32 tempVertsStsCount = 24;

  tempVerts = new Tyra::Vec4[tempVertsStsCount];
  tempVerts[0].set(-1.0F, -1.0F, -1.0F);
  tempVerts[1].set(1.0F, -1.0F, -1.0F);
  tempVerts[2].set(1.0F, -1.0F, 1.0F);
  tempVerts[3].set(-1.0F, -1.0F, 1.0F);
  tempVerts[4].set(1.0F, -1.0F, 1.0F);
  tempVerts[5].set(1.0F, 1.0F, 1.0F);
  tempVerts[6].set(-1.0F, 1.0F, 1.0F);
  tempVerts[7].set(-1.0F, -1.0F, 1.0F);
  tempVerts[8].set(-1.0F, -1.0F, 1.0F);
  tempVerts[9].set(-1.0F, 1.0F, 1.0F);
  tempVerts[10].set(-1.0F, 1.0F, -1.0F);
  tempVerts[11].set(-1.0F, -1.0F, -1.0F);
  tempVerts[12].set(1.0F, -1.0F, -1.0F);
  tempVerts[13].set(1.0F, 1.0F, -1.0F);
  tempVerts[14].set(1.0F, 1.0F, 1.0F);
  tempVerts[15].set(1.0F, -1.0F, 1.0F);
  tempVerts[16].set(-1.0F, -1.0F, -1.0F);
  tempVerts[17].set(-1.0F, 1.0F, -1.0F);
  tempVerts[18].set(1.0F, 1.0F, -1.0F);
  tempVerts[19].set(1.0F, -1.0F, -1.0F);
  tempVerts[20].set(1.0F, 1.0F, -1.0F);
  tempVerts[21].set(-1.0F, 1.0F, -1.0F);
  tempVerts[22].set(-1.0F, 1.0F, 1.0F);
  tempVerts[23].set(1.0F, 1.0F, 1.0F);

  tempTexCoords = new Tyra::Vec4[tempVertsStsCount];
  // Y = 1.0F - Y
  tempTexCoords[0].set(0.0625F, 0.1875F, 1.0000F, 0.0000F);
  tempTexCoords[1].set(0.0625F, 0.1250F, 1.0000F, 0.0000F);
  tempTexCoords[2].set(0.0000F, 0.1250F, 1.0000F, 0.0000F);
  tempTexCoords[3].set(0.0000F, 0.1875F, 1.0000F, 0.0000F);
  tempTexCoords[4].set(0.0625F, 0.2500F, 1.0000F, 0.0000F);
  tempTexCoords[5].set(0.0625F, 0.1875F, 1.0000F, 0.0000F);
  tempTexCoords[6].set(0.0000F, 0.1875F, 1.0000F, 0.0000F);
  tempTexCoords[7].set(0.0000F, 0.2500F, 1.0000F, 0.0000F);
  tempTexCoords[8].set(0.0625F, 0.3125F, 1.0000F, 0.0000F);
  tempTexCoords[9].set(0.0000F, 0.3125F, 1.0000F, 0.0000F);
  tempTexCoords[10].set(0.0000F, 0.3750F, 1.0000F, 0.0000F);
  tempTexCoords[11].set(0.0625F, 0.3750F, 1.0000F, 0.0000F);
  tempTexCoords[12].set(0.0625F, 0.3750F, 1.0000F, 0.0000F);
  tempTexCoords[13].set(0.0000F, 0.3750F, 1.0000F, 0.0000F);
  tempTexCoords[14].set(0.0000F, 0.4375F, 1.0000F, 0.0000F);
  tempTexCoords[15].set(0.0625F, 0.4375F, 1.0000F, 0.0000F);
  tempTexCoords[16].set(0.0000F, 0.0625F, 1.0000F, 0.0000F);
  tempTexCoords[17].set(0.0000F, 0.1250F, 1.0000F, 0.0000F);
  tempTexCoords[18].set(0.0625F, 0.1250F, 1.0000F, 0.0000F);
  tempTexCoords[19].set(0.0625F, 0.0625F, 1.0000F, 0.0000F);
  tempTexCoords[20].set(0.0000F, 0.0625F, 1.0000F, 0.0000F);
  tempTexCoords[21].set(0.0625F, 0.0625F, 1.0000F, 0.0000F);
  tempTexCoords[22].set(0.0625F, 0.0000F, 1.0000F, 0.0000F);
  tempTexCoords[23].set(0.0000F, 0.0000F, 1.0000F, 0.0000F);

  // Because 16 blocks can fit in single column of 256x256 tex atlas
  offset = 1.0F / 16.0F;

  tempVertFaces = new u32[36];
  tempTexCoordsFaces = new u32[36];

  std::string vertexFaces =
      "1,2,3,1,3,4,5,6,7,5,7,8,9,10,11,9,11,12,13,14,15,13,15,16,17,18,19,17,"
      "19,20,21,22,23,21,23,24";
  std::stringstream ssVertexFaces(vertexFaces);

  std::string texCoordFaces =
      "1,2,3,1,3,4,5,6,7,5,7,8,9,10,11,9,11,12,13,14,15,13,15,16,17,18,19,17,"
      "19,20,21,22,23,21,23,24";
  std::stringstream ssTexCoordFaces(texCoordFaces);

  int i = 0;
  std::string item;

  while (std::getline(ssVertexFaces, item, ','))
    tempVertFaces[i++] = std::stoi(item) - 1;

  i = 0;

  while (std::getline(ssTexCoordFaces, item, ','))
    tempTexCoordsFaces[i++] = std::stoi(item) - 1;
}

void McpipMultiTexBlockData::unroll() {
  count = 36;
  comboData = new Tyra::Vec4[36 * 2];
  vertices = &comboData[0];
  textureCoords = &comboData[36];

  for (u32 i = 0; i < count; i++) {
    vertices[i] = tempVerts[tempVertFaces[i]];
    textureCoords[i] = tempTexCoords[tempTexCoordsFaces[i]];
  }
}

void McpipMultiTexBlockData::dellocateTempData() {
  delete[] tempVerts;
  delete[] tempTexCoords;
  delete[] tempVertFaces;
  delete[] tempTexCoordsFaces;
}

}  // namespace Tyra
