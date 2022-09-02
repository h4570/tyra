/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

#include "renderer/core/3d/bbox/render_bbox.hpp"

namespace Tyra {

/**
 * Splits all 3D input vertices into 16vert parts and creates child bboxes for
 * it
 */
class StaPipBagPackagesBBox {
 public:
  StaPipBagPackagesBBox(const Vec4* t_vertices, unsigned int* t_faces,
                        const unsigned int& t_facesCount,
                        const unsigned int& t_maxVertCount);
  StaPipBagPackagesBBox(const Vec4* t_vertices, const unsigned int& t_counts,
                        const unsigned int& t_maxVertCount);
  ~StaPipBagPackagesBBox();

  void setMaxVertCount(const unsigned int& count);

  /**
   * @param index Index of part
   */
  const RenderBBox& getChildBBox1By3(const unsigned int& index) const;

  /** @brief CoreBBox created from all of vertices */
  RenderBBox* getMainBBox();

  /** @brief Get amount of child Bbox1/3 parts */
  const unsigned int& getPartsCount() const;

  /** @brief Get amount of vertices */
  const unsigned int& getVertexCount() const;

  const std::vector<CoreBBox>& getParts() const { return *bboxParts; }

  /**
   * @brief For example if we want to build a bbox from 96-112 vertices, we
   * should input index = 3, partsSize = 1
   * @param index Index of part
   * @param partsSize How many 1/3 parts
   */
  RenderBBox createChildBBox(const unsigned int& index,
                             const unsigned short& partsSize) const;

  void print() const;
  void print(const char* name) const;
  void print(const std::string& name) const { print(name.c_str()); }
  std::string getPrint(const char* name = nullptr) const;

 private:
  unsigned int maxVertCount;
  unsigned int vertexCount, partsCount;

  /** @brief CoreBBox of all vertices */
  RenderBBox* mainBBox;

  /** @brief Bounding boxes of 1/3 vertices */
  std::vector<CoreBBox>* bboxParts;
};

}  // namespace Tyra
