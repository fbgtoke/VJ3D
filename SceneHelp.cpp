#include "SceneHelp.h"
#include "Game.h"

SceneHelp::SceneHelp()
  : Scene(Scene::SCENE_HELP) {}

SceneHelp::~SceneHelp() {}

void SceneHelp::update(int deltaTime) {
  Scene::update(deltaTime);

  if (InputManager::getAction(InputManager::Close)) // Escape
    Game::instance().changeScene(Scene::SCENE_MENU);
}

void SceneHelp::initGui() {
  mGui = Game::instance().getResource().layout("help.xml");
}