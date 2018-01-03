#include "SceneCredits.h"
#include "Game.h"

SceneCredits::SceneCredits()
  : Scene(Scene::SCENE_CREDITS) {}

SceneCredits::~SceneCredits() {}

void SceneCredits::update(int deltaTime) {
  Scene::update(deltaTime);

  if (Game::instance().getKeyPressed(27)) // Escape
    Game::instance().changeScene(Scene::SCENE_MENU);
}

void SceneCredits::initGui() {
  mGui = Game::instance().getResource().layout("credits.xml");
}