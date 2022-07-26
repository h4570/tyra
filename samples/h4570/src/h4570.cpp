/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Wellington Carvalho <wellcoj@gmail.com>
*/

#include "h4570.hpp"
#include "file/file_utils.hpp"
#include "loaders/3d/md2/md2_loader.hpp"
#include "loaders/3d/tyrobj/tyrobj_loader.hpp"
#include "thread/threading.hpp"

namespace Tyra {

float getRandomFloat(float a, float b) {
  float random = ((float)rand()) / (float)RAND_MAX;
  float diff = b - a;
  float r = random * diff;
  return a + r;
}

int getRandomInt(int a, int b) { return (rand() % (b - a + 1)) + a; }

H4570::H4570(Engine* t_engine) { engine = t_engine; }
H4570::~H4570() {}

StaticMesh* getStaticMesh(Renderer* renderer);
DynamicMesh* getWarrior(Renderer* renderer);
DynamicMesh* getCube(Renderer* renderer);
StaPipOptions* getStaPipOptions();
DynPipOptions* getDynPipOptions();
void setPipelineOptions(PipelineOptions* options);
Sprite* get2DPicture(Renderer* renderer);

void H4570::init() {
  // Song
  engine->audio.loadSong(FileUtils::fromCwd("mafikizolo-loot.wav"));
  engine->audio.setSongLoop(true);
  engine->audio.setSongVolume(30);

  adpcmSample = engine->audio.loadADPCM(FileUtils::fromCwd("walk.adpcm"));
  engine->audio.setADPCMVolume(80, 1);

  engine->renderer.setClearScreenColor(Color(64.0F, 64.0F, 64.0F));

  staticMesh = getStaticMesh(&engine->renderer);

  cube = getCube(&engine->renderer);

  warrior = getWarrior(&engine->renderer);
  warriorTex = engine->renderer.core.texture.repository.getBySpriteOrMesh(
      warrior->getMaterial(0)->getId());

  warriorsCount = 22;
  warriors = new DynamicMesh*[warriorsCount];
  for (u8 i = 0; i < warriorsCount; i++) {
    warriors[i] = new DynamicMesh(*warrior);
    warriors[i]->translation.translateX(-40.0F + static_cast<float>(i) * 4);
    warriors[i]->translation.translateY(30.0F);
    warriors[i]->translation.translateZ(-10.0F);
    warriors[i]->translation.rotateX(-1.5F);
    warriorTex->addLink(warriors[i]->getMaterial(0)->getId());
    warriors[i]->playAnimation(0, warriors[i]->getFramesCount() - 1);
    warriors[i]->setCurrentAnimationFrame(
        getRandomInt(0, warriors[i]->getFramesCount() - 1));
    warriors[i]->setAnimSpeed(getRandomFloat(0.1F, 0.9F));
  }

  staOptions = getStaPipOptions();
  dynOptions = getDynPipOptions();

  cameraPosition = Vec4(0.0F, 0.0F, 20.0F);
  cameraLookAt = *warrior->getPosition();

  blocksTex = engine->renderer.core.texture.repository.add(
      FileUtils::fromCwd("blocks.png"));

  mcPip.setRenderer(&engine->renderer.core);
  dynpip.setRenderer(&engine->renderer.core);
  stapip.setRenderer(&engine->renderer.core);

  picture = get2DPicture(&engine->renderer);

  u32 rows = 16;     // 64
  u32 columns = 16;  // 64
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

  // engine->audio.playSong();
  engine->renderer.setFrameLimit(false);
}

u32 counter = 0;

void H4570::loop() {
  if (counter++ > 100) {
    TYRA_LOG("Free RAM: ", engine->info.getAvailableRAM(), " MB");
    TYRA_LOG("FPS: ", engine->info.getFps());
    counter = 0;
  }

  for (u8 i = 0; i < warriorsCount; i++) warriors[i]->animate();

  engine->renderer.beginFrame(CameraInfo3D(&cameraPosition, &cameraLookAt));
  {
    for (u32 i = 0; i < blocksCount; i++) {
      rotations[i].rotateX(0.04F);
      rotations[i].rotateY(0.01F);
      rotations[i].rotateZ(0.01F);

      blocks[i].model = translations[i] * rotations[i] * scales[i];
    }

    engine->renderer.renderer2D.render(picture);

    engine->renderer.renderer3D.usePipeline(&stapip);
    { stapip.render(staticMesh, staOptions); }

    engine->renderer.renderer3D.usePipeline(&dynpip);
    {
      // dynpip.render(cube);
      Threading::switchThread();
      for (u8 i = 0; i < warriorsCount; i++) {
        dynpip.render(warriors[i], dynOptions);
        if (i == 5) Threading::switchThread();
        if (i == 10) Threading::switchThread();
        if (i == 15) Threading::switchThread();
      }
    }

    engine->renderer.renderer3D.usePipeline(&mcPip);
    { mcPip.render(blocks, blocksCount, blocksTex); }
  }
  engine->renderer.endFrame();
}

StaticMesh* getStaticMesh(Renderer* renderer) {
  MD2Loader loader;
  auto* data = loader.load(FileUtils::fromCwd("warrior.md2"), .08F, false);
  auto* result = new StaticMesh(*data);
  // result->translation.translateZ(-30.0F);
  delete data;

  renderer->core.texture.repository.addByMesh(result, FileUtils::getCwd(),
                                              "png");

  return result;
}

DynamicMesh* getWarrior(Renderer* renderer) {
  MD2Loader loader;
  auto* data = loader.load(FileUtils::fromCwd("warrior.md2"), .08F, false);
  auto* result = new DynamicMesh(*data);
  // result->translation.translateZ(-30.0F);
  delete data;

  renderer->core.texture.repository.addByMesh(result, FileUtils::getCwd(),
                                              "png");

  result->playAnimation(0, result->getFramesCount() - 1);
  result->setAnimSpeed(0.15F);

  return result;
}

DynamicMesh* getCube(Renderer* renderer) {
  TyrobjLoader loader;
  auto* data =
      loader.load(FileUtils::fromCwd("untitled.tyrobj"), 2, .08F, false);

  TYRA_LOG("Frames count: ", data->framesCount);
  TYRA_LOG("Materials count: ", data->materialsCount);

  for (u32 i = 0; i < data->framesCount; i++) {
    TYRA_LOG("Vertex count ", i, ": ", data->frames[i]->verticesCount);
    for (u32 j = 0; j < data->frames[i]->verticesCount; j++) {
      TYRA_LOG("Vertex ", j, ": ", data->frames[i]->vertices[j].getPrint());
    }

    TYRA_LOG("Normals count ", i, ": ", data->frames[i]->normalsCount);
    for (u32 j = 0; j < data->frames[i]->normalsCount; j++) {
      TYRA_LOG("Normal ", j, ": ", data->frames[i]->normals[j].getPrint());
    }
  }

  for (u32 i = 0; i < data->materialsCount; i++) {
    TYRA_LOG("Material ", i, ": ", data->materials[i]->name);
    TYRA_LOG("Faces count: ", data->materials[i]->count);
    for (u32 j = 0; j < data->materials[i]->count; j++) {
      TYRA_LOG("Vertex face ", j, ": ", data->materials[i]->vertexFaces);
      TYRA_LOG("Normal face ", j, ": ", data->materials[i]->normalFaces);
    }
  }

  TYRA_BREAKPOINT();
  auto* result = new DynamicMesh(*data);
  // result->translation.translateZ(-30.0F);
  delete data;

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

StaPipOptions* getStaPipOptions() {
  auto* options = new StaPipOptions();
  setPipelineOptions(options);
  return options;
}

DynPipOptions* getDynPipOptions() {
  auto* options = new DynPipOptions();
  setPipelineOptions(options);
  return options;
}

void setPipelineOptions(PipelineOptions* options) {
  auto* ambientColor = new Color(32.0F, 32.0F, 32.0F, 32.0F);
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
}

}  // namespace Tyra
