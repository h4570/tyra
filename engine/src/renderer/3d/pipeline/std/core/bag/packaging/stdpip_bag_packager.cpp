/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include <tamtypes.h>
#include <math.h>
#include "debug/debug.hpp"
#include "renderer/3d/pipeline/std/core/bag/packaging/stdpip_bag_packager.hpp"

namespace Tyra {

StdpipBagPackager::StdpipBagPackager() {}
StdpipBagPackager::~StdpipBagPackager() {}

void StdpipBagPackager::init(Renderer3DFrustumPlanes* t_frustumPlanes) {
  frustumPlanes = t_frustumPlanes;
}

/**
 * @brief Create render packages from provided render data
 *
 * @param size Max maxVertCount verts (VU1 buffer size)
 */
StdpipBagPackage* StdpipBagPackager::create(u16* o_size, StdpipBag* data,
                                            u16 size) {
  TYRA_ASSERT(size <= maxVertCount, "StdpipBagPackage can have max ",
              maxVertCount, " verts. Provided \"", size, "\"");

  *o_size = ceil(data->count / static_cast<float>(size));
  StdpipBagPackage* result = new StdpipBagPackage[*o_size];

  for (u16 i = 0; i < *o_size; i++) {
    result[i].bag = data;
    result[i].vertices = &data->vertices[i * size];

    if (data->texture) result[i].sts = &data->texture->coordinates[i * size];

    if (data->color->many)
      result[i].colors = reinterpret_cast<Vec4*>(&data->color->many[i * size]);

    if (data->lighting) result[i].normals = &data->lighting->normals[i * size];

    result[i].indexOf1By3BBox = (i * size) / (maxVertCount / 3);

    if (i == *o_size - 1) {
      result[i].size = data->count - i * size;
    } else {
      result[i].size = size;
    }

    result[i].isInFrustum = checkFrustum(result[i]);
  }

  return result;
}

/**
 * @brief Split render package to smaller packages
 *
 * @param size Max maxVertCount verts (VU1 buffer size)
 */
StdpipBagPackage* StdpipBagPackager::create(u16* o_count,
                                            const StdpipBagPackage& pkg,
                                            u16 size) {
  TYRA_ASSERT(size <= maxVertCount, "StdpipBagPackage can have max ",
              maxVertCount, " verts. Provided \"", size, "\"");

  *o_count = ceil(pkg.size / static_cast<float>(size));
  auto* result = new StdpipBagPackage[*o_count];

  for (u16 i = 0; i < *o_count; i++) {
    result[i].bag = pkg.bag;
    result[i].vertices = &pkg.vertices[i * size];

    if (pkg.bag->texture) result[i].sts = &pkg.sts[i * size];

    if (pkg.bag->color->many) result[i].colors = &pkg.colors[i * size];

    if (pkg.bag->lighting) result[i].normals = &pkg.normals[i * size];

    result[i].indexOf1By3BBox =
        pkg.indexOf1By3BBox + ((i * size) / (maxVertCount / 3));

    if (i == *o_count - 1) {
      result[i].size = pkg.size - i * size;
    } else {
      result[i].size = size;
    }

    result[i].isInFrustum = checkFrustum(result[i]);
  }

  return result;
}

CoreBBoxFrustum StdpipBagPackager::checkFrustum(const StdpipBagPackage& pkg) {
  if (pkg.size <= (maxVertCount / 3)) {  // Is subpackage
    auto& bbox = renderBBox->getChildBBox1By3(pkg.indexOf1By3BBox);
    return bbox.clipIsInFrustum(frustumPlanes->getAll(), *pkg.bag->info->model);
  } else {  // Is package
    auto bbox = renderBBox->createChildBBox(
        pkg.indexOf1By3BBox,
        ceil(pkg.size / static_cast<float>(maxVertCount / 3)));

    return bbox.clipIsInFrustum(frustumPlanes->getAll(), *pkg.bag->info->model);
  }
}

void StdpipBagPackager::setMaxVertCount(const u32& count) {
  maxVertCount = count;
}

}  // namespace Tyra
