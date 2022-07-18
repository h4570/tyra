/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
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
  explicit BBox(CoreBBox** t_bboxes, const u32& count);
  explicit BBox(Vec4* t_vertices, u32* faces, u32 t_count);
  explicit BBox(Vec4* t_vertices, u32 t_count);
  explicit BBox(Vec4* t_vertices);
  const float& getHeight() { return _height; }
  const float& getDepth() { return _depth; }
  const float& getWidth() { return _width; }
  /** @returns the vector directly in middle of the bounding box. */
  const Vec4& getCenter() { return _centerVector; }
  /** @returns the front face (further on z-axis) */
  const BBoxFace& getFrontFace() { return _frontFace; }
  /** @returns the back face (nearer on z-axis) */
  const BBoxFace& getBackFace() { return _backFace; }
  /** @returns the left face (further on x-axis) */
  const BBoxFace& getLeftFace() { return _leftFace; }
  /** @returns the right face (nearer on x-axis) */
  const BBoxFace& getRightFace() { return _rightFace; }
  /** @returns the top face (further on y-axis) */
  const BBoxFace& getTopFace() { return _topFace; }
  /** @returns the bottom face (nearer on y-axis) */
  const BBoxFace& getBottomFace() { return _bottomFace; }

  /** @returns the lower (x, y, z) boundary of the box. */
  const Vec4 min();
  /** @returns the upper (x, y, z) boundary of the box. */
  const Vec4 max();
  /**
   * @returns void;
   * @param res_min Vec4 to store the min result
   * @param res_max Vec4 to store the min result
   * @brief Calc and stores the min and max points of box at a single loop
   * */
  void calcMinMax(Vec4& res_min, Vec4& res_max);

 protected:
  float _height, _depth, _width;
  Vec4 _centerVector, _min;
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
