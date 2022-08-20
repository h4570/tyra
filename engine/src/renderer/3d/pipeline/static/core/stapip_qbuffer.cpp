/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/3d/pipeline/static/core/stapip_qbuffer.hpp"
#include <sstream>
#include <iomanip>

namespace Tyra {

StaPipQBuffer::StaPipQBuffer() {
  size = 0;
  _isDynamicallyAllocated = false;
  _stAllocated = false;
  _colorAllocated = false;
  _normalAllocated = false;

  vertices = nullptr;
  colors = nullptr;
  sts = nullptr;
  normals = nullptr;
}

StaPipQBuffer::~StaPipQBuffer() { deallocateDynamicData(); }

void StaPipQBuffer::setMaxVertCount(const u32& count) { maxVertCount = count; }

void StaPipQBuffer::fillByPointer(const StaPipBagPackage& pkg) {
  TYRA_ASSERT(pkg.size <= maxVertCount, "VU1 buffer supports only ",
              maxVertCount, " verts. Provided: ", pkg.size);

  deallocateDynamicData();

  // Too bad, but in reality const is not violated.
  // This class needs refactor
  vertices = const_cast<Vec4*>(pkg.vertices);
  sts = const_cast<Vec4*>(pkg.sts);
  colors = const_cast<Vec4*>(pkg.colors);
  normals = const_cast<Vec4*>(pkg.normals);
  size = pkg.size;
  bag = pkg.bag;
}

void StaPipQBuffer::fillByCopyMax(const StaPipBagPackage& pkg1,
                                  const StaPipBagPackage& pkg2,
                                  const StaPipBagPackage& pkg3) {
  TYRA_ASSERT(pkg1.size <= maxVertCount / 3,
              "Wrong package size (1). Provided: ", pkg1.size);
  TYRA_ASSERT(pkg2.size <= maxVertCount / 3,
              "Wrong package size (2). Provided: ", pkg2.size);
  TYRA_ASSERT(pkg3.size <= maxVertCount / 3,
              "Wrong package size (3). Provided: ", pkg3.size);

  deallocateDynamicData();
  size = pkg1.size + pkg2.size + pkg3.size;
  allocateDynamicData(size, pkg1.bag);

  for (u16 i = 0; i < pkg1.size; i++) {
    vertices[i].set(pkg1.vertices[i]);

    if (pkg1.bag->texture) sts[i].set(pkg1.sts[i]);

    if (pkg1.bag->color->many)
      colors[i].set(reinterpret_cast<const Vec4&>(pkg1.colors[i]));

    if (pkg1.bag->lighting) normals[i].set(pkg1.normals[i]);
  }

  for (u16 i = 0; i < pkg2.size; i++) {
    vertices[i + pkg1.size].set(pkg2.vertices[i]);

    if (pkg1.bag->texture) sts[i + pkg1.size].set(pkg2.sts[i]);

    if (pkg1.bag->color->many)
      colors[i + pkg1.size].set(reinterpret_cast<const Vec4&>(pkg2.colors[i]));

    if (pkg1.bag->lighting) normals[i + pkg1.size].set(pkg2.normals[i]);
  }

  for (u16 i = 0; i < pkg3.size; i++) {
    vertices[i + pkg1.size + pkg2.size].set(pkg3.vertices[i]);

    if (pkg1.bag->texture) sts[i + pkg1.size + pkg2.size].set(pkg3.sts[i]);

    if (pkg1.bag->color->many)
      colors[i + pkg1.size + pkg2.size].set(
          reinterpret_cast<const Vec4&>(pkg3.colors[i]));

    if (pkg1.bag->lighting)
      normals[i + pkg1.size + pkg2.size].set(pkg3.normals[i]);
  }

  bag = pkg1.bag;
}

void StaPipQBuffer::fillByCopy1By2(const StaPipBagPackage& pkg1,
                                   const StaPipBagPackage& pkg2) {
  TYRA_ASSERT(pkg1.size <= maxVertCount / 3,
              "Wrong package size (1). Provided: ", pkg1.size);
  TYRA_ASSERT(pkg2.size <= maxVertCount / 3,
              "Wrong package size (2). Provided: ", pkg2.size);

  deallocateDynamicData();
  size = pkg1.size + pkg2.size;
  allocateDynamicData(size, pkg1.bag);

  for (u16 i = 0; i < pkg1.size; i++) {
    vertices[i].set(pkg1.vertices[i]);

    if (pkg1.bag->texture) sts[i].set(pkg1.sts[i]);

    if (pkg1.bag->color->many)
      colors[i].set(reinterpret_cast<const Vec4&>(pkg1.colors[i]));

    if (pkg1.bag->lighting) normals[i].set(pkg1.normals[i]);
  }

  for (u16 i = 0; i < pkg2.size; i++) {
    vertices[i + pkg1.size].set(pkg2.vertices[i]);

    if (pkg1.bag->texture) sts[i + pkg1.size].set(pkg2.sts[i]);

    if (pkg1.bag->color->many)
      colors[i + pkg1.size].set(reinterpret_cast<const Vec4&>(pkg2.colors[i]));

    if (pkg1.bag->lighting) normals[i + pkg1.size].set(pkg2.normals[i]);
  }

  bag = pkg1.bag;
}

void StaPipQBuffer::fillByCopy1By3(const StaPipBagPackage& pkg) {
  TYRA_ASSERT(pkg.size <= maxVertCount / 3,
              "Wrong package size (1). Provided: ", pkg.size);

  deallocateDynamicData();
  size = pkg.size;
  allocateDynamicData(size, pkg.bag);

  for (u16 i = 0; i < pkg.size; i++) {
    vertices[i].set(pkg.vertices[i]);

    if (pkg.bag->texture) sts[i].set(pkg.sts[i]);

    if (pkg.bag->color->many)
      colors[i].set(reinterpret_cast<const Vec4&>(pkg.colors[i]));

    if (pkg.bag->lighting) normals[i].set(pkg.normals[i]);
  }

  bag = pkg.bag;
}

void StaPipQBuffer::reallocateManually(const u16& t_size) {
  deallocateDynamicData();
  allocateDynamicData(t_size, bag);
  size = t_size;
}

void StaPipQBuffer::deallocateDynamicData() {
  if (!_isDynamicallyAllocated) return;

  delete[] vertices;

  if (_stAllocated) {
    delete[] sts;
    _stAllocated = false;
  }

  if (_colorAllocated) {
    delete[] colors;
    _colorAllocated = false;
  }

  if (_normalAllocated) {
    delete[] normals;
    _normalAllocated = false;
  }

  _isDynamicallyAllocated = false;
}

/**
 * When we not receive maxVertCount vertices, we must align it by ourself.
 * Too bad - not efficient.
 */
void StaPipQBuffer::allocateDynamicData(u16 size, StaPipBag* bag) {
  TYRA_ASSERT(size <= maxVertCount, "Wrong size. Max buffer size in VU1 is ",
              maxVertCount, ". Provided: ", size);
  TYRA_ASSERT(!_isDynamicallyAllocated, "Buffer is already allocated");

  if (size == 0) return;

  vertices = new Vec4[size];

  if (bag->texture != nullptr) {
    sts = new Vec4[size];
    _stAllocated = true;
  }

  if (bag->color->many != nullptr) {
    colors = new Vec4[size];
    _colorAllocated = true;
  }

  if (bag->lighting != nullptr) {
    normals = new Vec4[size];
    _normalAllocated = true;
  }

  _isDynamicallyAllocated = true;
}

bool StaPipQBuffer::any() const { return size > 0; }

void StaPipQBuffer::print() const {
  auto text = getPrint(nullptr);
  printf("%s\n", text.c_str());
}

void StaPipQBuffer::print(const char* name) const {
  auto text = getPrint(name);
  printf("%s\n", text.c_str());
}

std::string StaPipQBuffer::getPrint(const char* name) const {
  std::stringstream res;
  if (name) {
    res << name << "(";
  } else {
    res << "StaPipQBuffer(";
  }
  res << std::fixed << std::setprecision(2);
  res << std::endl;
  res << "Size: " << static_cast<int>(size) << std::endl;

  res << "Vertices: " << std::endl;
  for (u32 i = 0; i < size; i++)
    res << i << ": " << vertices[i].getPrint() << std::endl;

  if (bag->texture != nullptr) {
    res << "STs: " << std::endl;
    for (u32 i = 0; i < size; i++)
      res << i << ": " << sts[i].getPrint() << std::endl;
  }

  if (bag->color->many != nullptr) {
    res << "Colors: " << std::endl;
    for (u32 i = 0; i < size; i++)
      res << i << ": " << colors[i].getPrint() << std::endl;
  }

  if (bag->lighting != nullptr) {
    res << "Normals: " << std::endl;
    for (u32 i = 0; i < size; i++) {
      res << i << ": " << normals[i].getPrint();
      if (i < size - 1) {
        res << std::endl;
      }
    }
  }

  res << ")";
  return res.str();
}

}  // namespace Tyra
