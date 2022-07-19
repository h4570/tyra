/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/3d/pipeline/std/core/path1/stdpip_qbuffer.hpp"

namespace Tyra {

StdpipQBuffer::StdpipQBuffer() {
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

StdpipQBuffer::~StdpipQBuffer() { deallocateDynamicData(); }

void StdpipQBuffer::setMaxVertCount(const u32& count) { maxVertCount = count; }

void StdpipQBuffer::fillByPointer(const StdpipBagPackage& pkg) {
  TYRA_ASSERT(pkg.size <= maxVertCount, "VU1 buffer supports only ",
              maxVertCount, " verts. Provided: ", pkg.size);

  deallocateDynamicData();

  vertices = pkg.vertices;
  sts = pkg.sts;
  colors = pkg.colors;
  normals = pkg.normals;
  size = pkg.size;
  bag = pkg.bag;
}

void StdpipQBuffer::fillByCopyMax(const StdpipBagPackage& pkg1,
                                  const StdpipBagPackage& pkg2,
                                  const StdpipBagPackage& pkg3) {
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

void StdpipQBuffer::fillByCopy1By2(const StdpipBagPackage& pkg1,
                                   const StdpipBagPackage& pkg2) {
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

void StdpipQBuffer::fillByCopy1By3(const StdpipBagPackage& pkg) {
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

void StdpipQBuffer::reallocateManually(const u16& t_size) {
  deallocateDynamicData();
  allocateDynamicData(t_size, bag);
  size = t_size;
}

void StdpipQBuffer::deallocateDynamicData() {
  if (_isDynamicallyAllocated) {
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
}

/** When we not receive maxVertCount vertices, we must align it by ourself.
 * Too bad - not efficient. */
void StdpipQBuffer::allocateDynamicData(u16 size, StdpipBag* bag) {
  TYRA_ASSERT(size <= maxVertCount, "Wrong size. Max buffer size in VU1 is ",
              maxVertCount, ". Provided: ", size);
  TYRA_ASSERT(!_isDynamicallyAllocated, "Buffer is already allocated");

  vertices = new (std::align_val_t(sizeof(VECTOR))) Vec4[size];

  if (bag->texture != nullptr) {
    sts = new (std::align_val_t(sizeof(VECTOR))) Vec4[size];
    _stAllocated = true;
  }

  if (bag->color->many != nullptr) {
    colors = new (std::align_val_t(sizeof(VECTOR))) Vec4[size];
    _colorAllocated = true;
  }

  if (bag->lighting != nullptr) {
    normals = new (std::align_val_t(sizeof(VECTOR))) Vec4[size];
    _normalAllocated = true;
  }

  _isDynamicallyAllocated = true;
}

bool StdpipQBuffer::any() const { return size > 0; }

void StdpipQBuffer::print() const {
  auto text = getPrint(nullptr);
  printf("%s\n", text.c_str());
}

void StdpipQBuffer::print(const char* name) const {
  auto text = getPrint(name);
  printf("%s\n", text.c_str());
}

std::string StdpipQBuffer::getPrint(const char* name) const {
  std::stringstream res;
  if (name) {
    res << name << "(";
  } else {
    res << "Path1Buffer(";
  }
  res << std::fixed << std::setprecision(2);
  res << std::endl;
  res << "Size: " << static_cast<int>(size) << std::endl;

  res << "Vectors: " << std::endl;
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
