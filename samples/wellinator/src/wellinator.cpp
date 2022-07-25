/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Wellington Carvalho <wellcoj@gmail.com>
*/

#include "wellinator.hpp"
#include "file/file_utils.hpp"
#include "loaders/3d/md2/md2_loader.hpp"

namespace Tyra {

Wellinator::Wellinator(Engine* t_engine) { engine = t_engine; }
Wellinator::~Wellinator() {}

DynamicMesh* getWarrior(Renderer* renderer);
StaPipOptions* getRenderingOptions();
Sprite* get2DPicture(Renderer* renderer);

void Wellinator::init() {
  // Song

  engine->audio.loadSong(FileUtils::fromCwd("mafikizolo-loot.wav"));
  engine->audio.setSongLoop(true);
  engine->audio.setSongVolume(30);

  adpcmSample = engine->audio.loadADPCM(FileUtils::fromCwd("walk.adpcm"));
  engine->audio.setADPCMVolume(80, 1);

  engine->renderer.setClearScreenColor(Color(64.0F, 64.0F, 64.0F));
  warrior = getWarrior(&engine->renderer);
  renderOptions = getRenderingOptions();

  cameraPosition = Vec4(0.0F, 0.0F, 0.0F, 1.0F);
  cameraLookAt = *warrior->getPosition();

  blocksTex = engine->renderer.core.texture.repository.add(
      FileUtils::fromCwd("blocks.png"));

  mcPip.setRenderer(&engine->renderer.core);
  stapip.setRenderer(&engine->renderer.core);

  picture = get2DPicture(&engine->renderer);

  u32 rows = 8;     // 64
  u32 columns = 8;  // 64
  blocksCount = rows * columns;
  float initialCameraZ = -400.0F;

  float offset = 40.0F;
  blocks = new McpipBlock[blocksCount];
  translations = new M4x4[blocksCount];
  rotations = new M4x4[blocksCount];
  scales = new M4x4[blocksCount];
  float center = (rows / 2.0F) * offset;

  for (u32 i = 0; i < blocksCount; i++) {
    u32 column = i % columns;
    u32 row = i / rows;

    scales[i].scale(10.0F);
    translations[i].translateX(row * offset - center);
    translations[i].translateY(column * offset - center);
    translations[i].translateZ(initialCameraZ);

    u32 randRow = rand() % (rows + 1);
    u32 randColumn = rand() % (columns + 1);

    blocks[i].textureOffset =
        Vec4(mcPip.getTextureOffset() * randRow,
             mcPip.getTextureOffset() * randColumn, 0.0F, 1.0F);

    blocks[i].color = Color(128.0F, 128.0F, 128.0F, 128.0F);
  }

  // Vec4 warriorInitPosition = *warrior->getPosition();

  engine->audio.playSong();
}

void Wellinator::loop() {
  warrior->animate();

  if ((engine->pad.getPressed().DpadUp || engine->pad.getPressed().DpadDown ||
       engine->pad.getPressed().DpadLeft ||
       engine->pad.getPressed().DpadRight) &&
      adpcmTimer.getTimeDelta() > 8000) {
    adpcmTimer.prime();
    engine->audio.playADPCM(adpcmSample, 1);
  }

  engine->renderer.beginFrame(CameraInfo3D(&cameraPosition, &cameraLookAt));
  {
    for (u32 i = 0; i < blocksCount; i++) {
      rotations[i].rotateX(0.04F);
      rotations[i].rotateY(0.01F);
      rotations[i].rotateZ(0.01F);

      blocks[i].model = translations[i] * rotations[i] * scales[i];
    }

    engine->renderer.renderer2D.render(picture);

    // Vec4* nextPos = warrior->getPosition();

    // if (pad.getPressed().DpadUp) nextPos->y += 0.2;
    // if (pad.getPressed().DpadDown) nextPos->y -= 0.2;
    // if (pad.getPressed().DpadRight) nextPos->x += 0.2;
    // if (pad.getPressed().DpadLeft) nextPos->x -= 0.2;
    // if (pad.getLeftJoyPad().v > 200) nextPos->z += 0.2;
    // if (pad.getLeftJoyPad().v < 100) nextPos->z -= 0.2;

    // warrior->setPosition(*nextPos);

    engine->renderer.renderer3D.usePipeline(&stapip);
    { stapip.render(warrior, renderOptions); }

    engine->renderer.renderer3D.usePipeline(&mcPip);
    { mcPip.render(blocks, blocksCount, blocksTex, false); }
  }
  engine->renderer.endFrame();
}

DynamicMesh* getWarrior(Renderer* renderer) {
  MD2Loader loader;
  auto* data = loader.load(FileUtils::fromCwd("warrior.md2"), .08F, false);
  auto* result = new DynamicMesh(*data);
  result->translation.translateZ(-30.0F);
  delete data;
  renderer->core.texture.repository.addByMesh(result, FileUtils::getCwd(),
                                              "png");
  result->playAnimation(0, result->getFramesCount() - 1);
  result->setAnimSpeed(0.15F);
  return result;
}

Sprite* get2DPicture(Renderer* renderer) {
  auto* sprite = new Sprite;

  sprite->size.set(128.0F, 128.0F);
  sprite->position.set(500.0F, 280.0F);

  renderer->core.texture.repository.add(FileUtils::fromCwd("reward.png"))
      ->addLink(sprite->getId());

  return sprite;
}

StaPipOptions* getRenderingOptions() {
  auto* options = new StaPipOptions();

  auto* ambientColor = new Color(32.0F, 32.0F, 32.0F, 128.0F);
  auto* directionalColors = new Color[3];
  for (int i = 0; i < 3; i++) directionalColors[i].set(0.0F, 0.0F, 0.0F, 1.0F);
  auto* directionalDirections = new Vec4[3];
  for (int i = 0; i < 3; i++)
    directionalDirections[i].set(1.0F, 1.0F, 1.0F, 1.0F);

  auto* lightingOptions = new PipelineLightingOptions();  // Memory leak!
  lightingOptions->ambientColor = ambientColor;
  lightingOptions->directionalColors = directionalColors;
  lightingOptions->directionalDirections = directionalDirections;

  options->shadingType = Tyra::TyraShadingGouraud;
  options->blendingEnabled = true;
  options->antiAliasingEnabled = false;
  options->lighting = lightingOptions;

  directionalDirections[0].set(-1.0F, 0.0F, 0.0F);
  directionalColors[0].set(0.0F, 96.0F, 0.0F);

  directionalDirections[1].set(1.0F, 0.0F, 0.0F);
  directionalColors[1].set(96.0F, 0.0F, 0.0F);

  return options;
}

}  // namespace Tyra
