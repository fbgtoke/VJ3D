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
  
  if (Game::instance().getKeyPressed('z')) {
    std::string levelName = "level" + std::to_string(mCurrentSelected + 1);
    Game::instance().changeScene(Scene::SCENE_TEST);
    Game::instance().getBufferedScene()->receiveString("level-name", levelName);
  }

  if (Game::instance().getKeyPressed('a'))
    prevOption();
  if (Game::instance().getKeyPressed('d'))
    nextOption();
}

void SceneLevelSelect::initGui() {
  Scene::initGui();

  mGui = Game::instance().getResource().layout("level-select.xml");
}

void SceneLevelSelect::updateGui() {
  Scene::updateGui();

  glm::vec2 position = mGui->getSprite("level" + std::to_string(mCurrentSelected + 1))->getPosition();
  mGui->getSprite("level-selector")->setPosition(position);
}

void SceneLevelSelect::prevOption() {
  if (mCurrentSelected == 0)
    mCurrentSelected = Game::instance().getResource().Int("numLevels") - 1;
  else
    mCurrentSelected -= 1;
}

void SceneLevelSelect::nextOption() {
  if (mCurrentSelected == Game::instance().getResource().Int("numLevels") - 1)
    mCurrentSelected = 0;
  else
    mCurrentSelected += 1;
}