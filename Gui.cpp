#include "Gui.h"
#include "Game.h"

Gui::Gui() {}

Gui::~Gui() {
  clear();
}

void Gui::init() {
  setNumberOfLayers(0);
}

void Gui::render() {
  glm::mat4 PM = getProjectionMatrix();
  glm::mat4 VM = getViewMatrix();

  for (int i = 0; i < mLayers.size(); ++i) {
    for (Sprite* sprite : mLayers[i]) {
      ShaderProgram* shader = sprite->getShader();
      if (shader != nullptr) {
        shader->use();
        shader->setUniformMatrix4f("PM", PM);
        shader->setUniformMatrix4f("VM", VM);

        sprite->render();
      }
    }
  }
}

void Gui::clear() {
  for (int i = 0; i < mLayers.size(); ++i) {
    for (Sprite* sprite : mLayers[i])
      delete sprite;

    mLayers[i].clear();
  }
  mLayers.clear();
}

void Gui::setNumberOfLayers(unsigned int n) {
  clear();

  mLayers = std::vector<std::vector<Sprite*>>(n, std::vector<Sprite*>(0));
}

void Gui::addSprite(Sprite* sprite, unsigned int layer) {
  if (sprite != nullptr && layer < mLayers.size())
    mLayers[layer].push_back(sprite);
}

Sprite* Gui::getSprite(const std::string& name) {
  for (int i = 0; i < mLayers.size(); ++i)
    for (Sprite* sprite : mLayers[i])
      if (sprite->getName() == name)
        return sprite;

  return nullptr;
}

Text* Gui::getText(const std::string& name) {
  return dynamic_cast<Text*>(getSprite(name));
}

glm::mat4 Gui::getProjectionMatrix() {
  return glm::ortho(
    0.f,
    (float)SCREEN_WIDTH,
    (float)SCREEN_HEIGHT,
    0.f,
    0.001f,
    100.f
  );
}

glm::mat4 Gui::getViewMatrix() {
  return glm::lookAt(
    glm::vec3(0.f, 0.f, 10.f),
    glm::vec3(0.f, 0.f, -10.f),
    UP
  );
}