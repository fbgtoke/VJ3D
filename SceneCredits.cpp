#include "SceneCredits.h"
#include "Game.h"

SceneCredits::SceneCredits()
  : Scene(Scene::SCENE_CREDITS) {}

SceneCredits::~SceneCredits() {}

void SceneCredits::update(int deltaTime) {
  Scene::update(deltaTime);

  if (InputManager::getAction(InputManager::Close)) // Escape
    Game::instance().changeScene(Scene::SCENE_MENU);

  if (InputManager::getAction(InputManager::Accept)) // Escape
    Game::instance().changeScene(Scene::SCENE_MENU);
}

void SceneCredits::initGui() {
  mGui = Game::instance().getResource().layout("credits.xml");
}