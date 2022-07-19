/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020 - 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

#include "renderer/core/3d/bbox/render_bbox.hpp"

namespace Tyra {

/**
 * Splits all 3D input vertices into 16vert parts and creates child bboxes for
 * it
 */
class StdpipBagPackagesBBox {
 public:
  StdpipBagPackagesBBox(Vec4* t_vertices, u32* t_faces, const u32& t_facesCount,
                        const u32& t_maxVertCount);
  StdpipBagPackagesBBox(Vec4* t_vertices, const u32& t_counts,
                        const u32& t_maxVertCount);
  ~StdpipBagPackagesBBox();

  void setMaxVertCount(const u32& count);

  /**
   * @param index Index of part
   */
  const RenderBBox& getChildBBox1By3(const u32& index) const;

  /** @brief CoreBBox created from all of vertices */
  RenderBBox* getMainBBox();

  /** @brief Get amount of child Bbox1/3 parts */
  const u32& getPartsCount() const;

  /** @brief Get amount of vertices */
  const u32& getVertexCount() const;

  /**
   * @brief For example if we want to build a bbox from 96-112 vertices, we
   * should input index = 3, partsSize = 1
   * @param index Index of part
   * @param partsSize How many 1/3 parts
   */
  RenderBBox createChildBBox(const u32& index, const u16& partsSize) const;

  void print() const;
  void print(const char* name) const;
  void print(const std::string& name) const { print(name.c_str()); }
  std::string getPrint(const char* name = nullptr) const;

 private:
  u32 maxVertCount;
  u32 vertexCount, partsCount;

  /** @brief CoreBBox of all vertices */
  RenderBBox* mainBBox;

  /** @brief Bounding boxes of 1/3 vertices */
  std::vector<CoreBBox>* bboxParts;
};

}  // namespace Tyra
