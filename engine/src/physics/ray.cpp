/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022-2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Wellinator Carvalho <wellcoj@gmail.com>
*/

#include "physics/ray.hpp"

#include <math.h>
#include <algorithm>
#include <sstream>
#include <iomanip>

namespace Tyra {

Ray::Ray() {}
Ray::Ray(const Vec4& t_origin, const Vec4& t_direction) {
  origin.set(t_origin);
  direction.set(t_direction);
}

Ray::~Ray() {}

Vec4 Ray::at(const float& t) const { return (direction * t) + origin; }

float Ray::distanceToPoint(const Vec4& point) const {
  return origin.distanceTo(point);
}

bool Ray::intersectBox(const Vec4& minCorner, const Vec4& maxCorner,
                       float* outputDistance) const {
  auto min = (minCorner - origin) / direction;
  auto max = (maxCorner - origin) / direction;

  float tmin =
      std::max(std::max(std::min(min.x, max.x), std::min(min.y, max.y)),
               std::min(min.z, max.z));
  float tmax =
      std::min(std::min(std::max(min.x, max.x), std::max(min.y, max.y)),
               std::max(min.z, max.z));

  // if tmax < 0, ray (line) is intersecting AABB, but whole AABB is behing us
  if (tmax < 0) {
    if (outputDistance != nullptr) {
      *outputDistance = -1.0f;
    }
    return false;
  }

  // if tmin > tmax, ray doesn't intersect AABB
  if (tmin > tmax) {
    if (outputDistance != nullptr) {
      *outputDistance = -1.0f;
    }
    return false;
  }

  if (tmin < 0) {
    if (outputDistance != nullptr) {
      *outputDistance = tmax;
    }
    return true;
  }

  if (outputDistance != nullptr) {
    *outputDistance = tmin;
  }

  return true;
}

Vec4 Ray::invDir() const {
  return Vec4(1 / this->direction.x, 1 / this->direction.y,
              1 / this->direction.z, 1);
}

void Ray::print() const {
  auto text = getPrint(nullptr);
  printf("%s\n", text.c_str());
}

void Ray::print(const char* name) const {
  auto text = getPrint(name);
  printf("%s\n", text.c_str());
}

std::string Ray::getPrint(const char* name) const {
  std::stringstream res;
  if (name) {
    res << name << "(";
  } else {
    res << "Ray(";
  }
  res << std::fixed << std::setprecision(4);
  res << origin.getPrint("origin") << ", " << direction.getPrint("direction")
      << ")";
  return res.str();
}

}  // Namespace Tyra