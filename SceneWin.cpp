#include "SceneWin.h"
#include "Game.h"

SceneWin::SceneWin()
  : Scene(Scene::SCENE_WIN) {}

SceneWin::~SceneWin() {}

void SceneWin::update(int deltaTime) {
  Scene::update(deltaTime);

  if (Game::instance().getKeyPressed(27)) // Escape
    Game::instance().stop();

  if (Game::instance().getKeyPressed('z'))
    Game::instance().changeScene(Scene::SCENE_LEVEL_SELECT);
}

void SceneWin::initGui() {
  mGui = Game::instance().getResource().layout("win.xml");
}