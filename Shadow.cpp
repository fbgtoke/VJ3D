#include "Shadow.h"
#include "Game.h"

Shadow::Shadow(const Model* model)
  : mModel(model) {}

Shadow::~Shadow() {}

void Shadow::init() {
  Model::init();

  setMesh(Game::instance().getResource().mesh("plane.obj"));
  setTexture(Game::instance().getResource().texture("shadow.png"));
}

void Shadow::update(int deltaTime) {
  if (mModel == nullptr) {
    destroy();
    return;
  }

  Model::update(deltaTime);

  glm::vec3 position = mModel->getPosition();
  position.y = 0.001f;
  position.x += TILE_SIZE * 0.125f;
  setPosition(position);

  const float base = mModel->getCenter().y - mModel->getSize().y * 0.5f;
  const float max_height = TILE_SIZE * 2.f;
  const float min_scale = std::max(0.f, 1.f - base / max_height);
  glm::vec3 scale;
  scale.x = min_scale * (mModel->getSizeInTiles().x + mModel->getSizeInTiles().y) * 0.75f;
  scale.y = 1.f;
  scale.z = min_scale * mModel->getSizeInTiles().z * 1.5f;
  setScale(scale);
}

void Shadow::render() {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDepthMask(false);

  Model::render();

  glDepthMask(true);
  glDisable(GL_BLEND);
}

void Shadow::unbind() { mModel = nullptr; }