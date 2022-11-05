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

#include "./bbox_face.hpp"
#include "renderer/core/3d/bbox/core_bbox.hpp"
#include "math/vec4.hpp"

namespace Tyra {

/** Bounding box with more features */
class BBox : public CoreBBox {
 public:
  BBox(const BBox& t_bbox);
  explicit BBox(CoreBBox** t_bboxes, const u32& count);
  explicit BBox(const Vec4* t_vertices, const u32* faces, const u32& t_count);
  explicit BBox(const Vec4* t_vertices, const u32& t_count);
  explicit BBox(const BBox& t_bbox, const M4x4& t_matrix);
  explicit BBox(const Vec4* t_vertices);

  static BBox create(const Vec4& center, const float& size);

  void operator=(const BBox& v);

  const float& getHeight() const { return _height; }
  const float& getDepth() const { return _depth; }
  const float& getWidth() const { return _width; }

  /** @returns the vector directly in middle of the bounding box. */
  const Vec4& getCenter() const { return _centerVector; }

  /** @returns the front face (further on z-axis) */
  const BBoxFace& getFrontFace() const { return _frontFace; }

  /** @returns the back face (nearer on z-axis) */
  const BBoxFace& getBackFace() const { return _backFace; }

  /** @returns the left face (further on x-axis) */
  const BBoxFace& getLeftFace() const { return _leftFace; }

  /** @returns the right face (nearer on x-axis) */
  const BBoxFace& getRightFace() const { return _rightFace; }

  /** @returns the top face (further on y-axis) */
  const BBoxFace& getTopFace() const { return _topFace; }

  /** @returns the bottom face (nearer on y-axis) */
  const BBoxFace& getBottomFace() const { return _bottomFace; }

  /** @returns the lower (x, y, z) boundary of the box. */
  Vec4 min() const;
  /** @returns the upper (x, y, z) boundary of the box. */
  Vec4 max() const;

  /**
   * @param res_min Vec4 to store the min result
   * @param res_max Vec4 to store the min result
   * @brief Calc and stores the min and max points of box at a single loop
   * */
  void getMinMax(Vec4* res_min, Vec4* res_max) const;

  /** Get new transformed BBox by model matrix */
  BBox getTransformed(const M4x4& t_matrix) const;

 protected:
  float _height, _depth, _width;
  Vec4 _centerVector;
  /** Front face (further on z-axis) */
  BBoxFace _frontFace;
  /** Back face (nearer on z-axis) */
  BBoxFace _backFace;
  /** Left face (nearer on x-axis) */
  BBoxFace _leftFace;
  /** Right face (further on x-axis) */
  BBoxFace _rightFace;
  /** Top face (further on y-axis) */
  BBoxFace _topFace;
  /** Bottom face (nearer on y-axis) */
  BBoxFace _bottomFace;

 private:
  void setData();
};

}  // namespace Tyra
