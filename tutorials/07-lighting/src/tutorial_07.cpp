/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include <tyra>
#include "tutorial_07.hpp"

namespace Tyra {

Tutorial07::Tutorial07(Engine* t_engine) : engine(t_engine) {
  counter = 101;
  switcher = 0;
}

Tutorial07::~Tutorial07() {}

void Tutorial07::init() {
  engine->renderer.setClearScreenColor(Color(32.0F, 32.0F, 32.0F));

  stapip.setRenderer(&engine->renderer.core);

  cameraPosition = Vec4(0.0F, 0.0F, -10.0F);
  cameraLookAt.unit();

  /**
   * Directional lights are dynamic lights with infinite range.
   *
   * Lightmap is a fast, pregenerated lightmap for a mesh.
   * It is very useful for meshes like terrain, buildings, etc..
   *
   * Lightmap in PS2 world is done by old school color-per-vertex.
   */

  loadMeshWithLightmap();
  setDirectionalLightsOptions();
}

void Tutorial07::loop() {
  switchBetweenLightmapAndDirectional();

  /** Rotate the mesh to see the effect static vs dynamic lights */
  mesh->rotation.rotateY(0.05F);

  engine->renderer.beginFrame(CameraInfo3D(&cameraPosition, &cameraLookAt));
  {
    engine->renderer.renderer3D.usePipeline(stapip);

    /** Switch between dynamic directional lights / static lightmap */
    if (switcher == 0) {
      stapip.render(mesh.get(), renderOptions);
    } else {
      stapip.render(mesh.get());
    }
  }
  engine->renderer.endFrame();
}

void Tutorial07::loadMeshWithLightmap() {
  ObjLoaderOptions options;
  options.scale = 2.0F;

  /**
   * Material colors are automatically fetched from .mtl file
   * (Kd field)
   *
   * If you want to change it manually, just try to change
   * mesh.materials[].ambient
   */

  auto data = ObjLoader::load(FileUtils::fromCwd("cup.obj"), options);

  /**
   * This model has no texture (it was exported without UV data),
   * but you can use lighting with textures as well.
   */

  /** Lets add lightmap manually */
  setLightmap(data.get());

  mesh = std::make_unique<StaticMesh>(data.get());
  mesh->translation.translateY(-1.0F);
  mesh->rotation.rotateX(1.0F);
}

void Tutorial07::setDirectionalLightsOptions() {
  // Set ambient color
  directionalAmbientColor.set(16.0F, 32.0F, 16.0F);

  // Reset colors and directions
  for (int i = 0; i < 3; i++) {
    directionalColors[i].set(0.0F, 0.0F, 0.0F, 1.0F);
    directionalDirections[i].set(1.0F, 1.0F, 1.0F, 1.0F);
  }

  // Set light and direction 1
  directionalDirections[0].set(-1.0F, 0.3F, 0.3F);
  directionalColors[0].set(16.0F, 16.0F, 96.0F);

  // Set light and direction 2
  directionalDirections[1].set(0.0F, -1.0F, 1.0F);
  directionalColors[1].set(96.0F, 16.0F, 16.0F);

  // Set render options
  renderLightingOptions.ambientColor = &directionalAmbientColor;
  renderLightingOptions.directionalColors = directionalColors.begin();
  renderLightingOptions.directionalDirections = directionalDirections.begin();
  renderOptions.shadingType = TyraShadingGouraud;
  renderOptions.blendingEnabled = true;
  renderOptions.lighting = &renderLightingOptions;
}

void Tutorial07::setLightmap(MeshBuilderData* data) {
  /** Information for mesh, that data has lightmap */
  data->loadLightmap = true;

  /** This object file has only 1 material */
  auto* material = data->materials[0];

  /** Static mesh so 1 frame */
  auto* frame = material->frames[0];

  /**
   * You can load your pregenerated lightmap here.
   * In tutorial we will generate some random lightmap.
   *
   * Dont worry about dynamic allocation,
   * it will be handled by Mesh class.
   */
  frame->colors = new Color[frame->count];
  for (u32 i = 0; i < frame->count; i++) {
    auto randomLight = 0.1F + ((2.0F + frame->vertices[i].x) / 4.0F);
    frame->colors[i] = material->ambient * randomLight;
    frame->colors[i].a = 128.0F;
  }
}

void Tutorial07::switchBetweenLightmapAndDirectional() {
  if (counter++ > 100) {
    counter = 0;
    switcher = !switcher;

    auto* material = mesh->materials[0];

    if (switcher == 0) {
      material->lightmapFlag = false;
      TYRA_LOG("Directional lights!");
    } else {
      material->lightmapFlag = true;
      TYRA_LOG("Lightmap!");
    }
  }
}

}  // namespace Tyra
