#include "SceneHelp.h"
#include "Game.h"

SceneHelp::SceneHelp()
  : Scene(Scene::SCENE_HELP) {}

SceneHelp::~SceneHelp() {}

void SceneHelp::init() {
  Scene::init();

  mCurrentLayer = 0;
}

void SceneHelp::update(int deltaTime) {
  Scene::update(deltaTime);

  if (InputManager::getAction(InputManager::Close)) // Escape
    Game::instance().changeScene(Scene::SCENE_MENU);

  if (InputManager::getAction(InputManager::Accept))
    nextLayer();
}

void SceneHelp::render() {
  Scene::render();
}

void SceneHelp::initGui() {
  Scene::initGui();

  mGui = Game::instance().getResource().layout("help.xml");
  mGui->showLayer(0);
  mGui->hideLayer(1);
  mGui->hideLayer(2);
}

void SceneHelp::nextLayer() {
  mGui->hideLayer(mCurrentLayer);

  mCurrentLayer = (mCurrentLayer + 1)%3;
  mGui->showLayer(mCurrentLayer);
}