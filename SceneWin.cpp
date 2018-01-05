#include "SceneWin.h"
#include "Game.h"

SceneWin::SceneWin()
  : Scene(Scene::SCENE_WIN) {}

SceneWin::~SceneWin() {}

void SceneWin::update(int deltaTime) {
  Scene::update(deltaTime);

  if (InputManager::getAction(InputManager::Close)) // Escape
    Game::instance().stop();

  if (InputManager::getAction(InputManager::Accept))
    Game::instance().changeScene(Scene::SCENE_LEVEL_SELECT);
}

void SceneWin::initGui() {
  mGui = Game::instance().getResource().layout("win.xml");
}