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
#include "tutorial_05.hpp"

namespace Tyra {

Tutorial05::Tutorial05(Engine* t_engine) : engine(t_engine) {}

Tutorial05::~Tutorial05() {
  engine->renderer.getTextureRepository().freeByMesh(zombieMesh.get());
  engine->renderer.getTextureRepository().freeByMesh(warriorMesh.get());
}

void Tutorial05::init() {
  dynpip.setRenderer(&engine->renderer.core);

  cameraPosition = Vec4(0.0F, 0.0F, -10.0F);
  cameraLookAt.unit();

  loadZombieMesh();
  loadWarriorMesh();
}

void Tutorial05::loop() {
  zombieMesh->update();  // update animation state
  warriorMesh->update();

  engine->renderer.beginFrame(CameraInfo3D(&cameraPosition, &cameraLookAt));
  {
    engine->renderer.renderer3D.usePipeline(dynpip);

    dynpip.render(zombieMesh.get());
    dynpip.render(warriorMesh.get(), renderOptions);
  }
  engine->renderer.endFrame();
}

void Tutorial05::loadZombieMesh() {
  ObjLoaderOptions options;
  options.scale = 20.0F;
  options.flipUVs = true;

  /** If count is > 1, loader will automatically add _00000X suffix */
  options.animation.count = 6;

  /** Starting file suffix index. It means that 1 will start from _000001.obj */
  options.animation.startingIndex = 1;

  /** If you want to know how to cook this file, please watch tutorial video */
  auto data = ObjLoader::load(FileUtils::fromCwd("zombie/zombie.obj"), options);

  zombieMesh = std::make_unique<DynamicMesh>(data.get());

  zombieMesh->translation.translate(Vec4(-3.0F, -5.0F, 0.0F));
  zombieMesh->rotation.rotateY(3.0F);

  /**
   * Tyra will play all frames by default.
   * If you want to arrange animation frames by yourself, you can use this
   * method.
   */
  zombieMesh->animation.setSequence({3, 4, 5});  // fight animation
  // zombieMesh->animation.setSequence({0, 1, 2}); // walk animation

  zombieMesh->animation.loop = true;

  engine->renderer.getTextureRepository().addByMesh(
      zombieMesh.get(), FileUtils::fromCwd("zombie/"), "png");
}

void Tutorial05::loadWarriorMesh() {
  MD2LoaderOptions options;
  options.scale = .08F;

  auto data =
      MD2Loader::load(FileUtils::fromCwd("warrior/warrior.md2"), options);

  warriorMesh = std::make_unique<DynamicMesh>(data.get());

  warriorMesh->translation.translate(Vec4(3.0F, -4.0F, 0.0F));
  warriorMesh->rotation.rotateX(-1.566F);
  warriorMesh->rotation.rotateY(1.566F);

  warriorMesh->animation.speed = 0.2F;

  /** Callback can return useful animation informations */
  warriorMesh->animation.setCallback(std::bind(
      &Tutorial05::warriorAnimationCallback, this, std::placeholders::_1));

  engine->renderer.getTextureRepository().addByMesh(
      warriorMesh.get(), FileUtils::fromCwd("warrior/"), "png");
}

void Tutorial05::warriorAnimationCallback(
    const AnimationSequenceCallback& callback) {
  if (callback == AnimationSequenceCallback_Loop) {
    TYRA_LOG("Warrior animation sequence is starting from beginning!");
  }
}

}  // namespace Tyra
