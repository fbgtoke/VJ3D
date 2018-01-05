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

  for (unsigned int i = 0; i < mLayers.size(); ++i) {
    if (isLayerVisible(i)) {
      for (Sprite* sprite : mLayers[i]) {
        ShaderProgram* shader = sprite->getShader();
        shader->use();
        shader->setUniformMatrix4f("PM", PM);
        shader->setUniformMatrix4f("VM", VM);
        shader->setUniform1i("tex", 0);

        sprite->render();
      }
    }
  }
}

void Gui::clear() {
  for (unsigned int i = 0; i < mLayers.size(); ++i) {
    for (Sprite* sprite : mLayers[i])
      delete sprite;

    mLayers[i].clear();
  }
  mLayers.clear();
}

void Gui::setNumberOfLayers(unsigned int n) {
  clear();

  mLayers = std::vector<std::vector<Sprite*>>(n, std::vector<Sprite*>(0));
  mVisible = std::vector<bool>(n, true);
}

void Gui::hideLayer(unsigned int n) { mVisible[n] = false; }
void Gui::showLayer(unsigned int n) { mVisible[n] = true; }
bool Gui::isLayerVisible(unsigned int n) const { return mVisible[n]; }

void Gui::addSprite(Sprite* sprite, unsigned int layer) {
  if (sprite != nullptr && layer < mLayers.size())
    mLayers[layer].push_back(sprite);
}

Sprite* Gui::getSprite(const std::string& name) {
  for (unsigned int i = 0; i < mLayers.size(); ++i)
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
    0.f,
    20.f
  );
}

glm::mat4 Gui::getViewMatrix() {
  return glm::lookAt(
    glm::vec3(0.f, 0.f, 10.f),
    glm::vec3(0.f, 0.f, -10.f),
    UP
  );
}