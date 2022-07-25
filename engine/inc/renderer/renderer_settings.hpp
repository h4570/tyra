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

#include <sstream>
#include <string>

namespace Tyra {

class RendererSettings {
 public:
  RendererSettings()
      : width(640.0F),
        height(448.0F),
        near(0.1F),
        far(4096.0F),
        projectionScale(4096.0F),
        aspectRatio(width / height) {}
  ~RendererSettings();

  const float& getWidth() const { return width; }
  const float& getHeight() const { return height; }
  const float& getNear() const { return near; }
  const float& getFar() const { return far; }
  const float& getProjectionScale() const { return projectionScale; }
  const float& getAspectRatio() const { return aspectRatio; }

  float getInterlacedHeightF() const { return getHeight() / 2; }
  unsigned int getInterlacedHeightUI() const {
    return static_cast<unsigned int>(getInterlacedHeightF());
  }

  static void copy(RendererSettings* out, const RendererSettings* in);
  void set(const RendererSettings& v);

  void print() const;
  std::string getPrint() const;

 private:
  float width, height, near, far, projectionScale, aspectRatio;
};

}  // namespace Tyra
