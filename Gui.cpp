#include "Gui.h"
#include "Game.h"

Gui::Gui() {}

Gui::~Gui() {
  for (Sprite* sprite : mSprites)
    delete sprite;
}

void Gui::init() {}

void Gui::render() {
  glEnable(GL_BLEND);

  glm::mat4 PM = getProjectionMatrix();
  glm::mat4 VM = getViewMatrix();

  for (Sprite* sprite : mSprites) {
    ShaderProgram* shader = sprite->getShader();
    if (shader != nullptr) {
      shader->use();
      shader->setUniformMatrix4f("PM", PM);
      shader->setUniformMatrix4f("VM", VM);

      sprite->render();
    }
  }

  glDisable(GL_BLEND);
}

void Gui::addSprite(Sprite* sprite) {
  if (sprite != nullptr)
    mSprites.push_back(sprite);
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