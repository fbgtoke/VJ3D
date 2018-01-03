#include "SceneLevelSelect.h"
#include "Game.h"

SceneLevelSelect::SceneLevelSelect()
  : Scene(Scene::SCENE_LEVEL_SELECT) {}

SceneLevelSelect::~SceneLevelSelect() {}

void SceneLevelSelect::init() {
  Scene::init();

  mCurrentSelected = 0;
}

void SceneLevelSelect::update(int deltaTime) {
  Scene::update(deltaTime);

  if (Game::instance().getKeyPressed(27)) // Escape
    Game::instance().changeScene(Scene::SCENE_MENU);
  
  if (Game::instance().getKeyPressed('z'))
    Game::instance().changeScene(Scene::SCENE_TEST);
}

void SceneLevelSelect::initGui() {
  Scene::initGui();

  mGui = Game::instance().getResource().layout("level-select.xml");
}