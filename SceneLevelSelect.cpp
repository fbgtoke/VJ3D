#include "SceneLevelSelect.h"
#include "Game.h"

SceneLevelSelect::SceneLevelSelect()
  : Scene(Scene::SCENE_LEVEL_SELECT) {}

SceneLevelSelect::~SceneLevelSelect() {}

void SceneLevelSelect::init() {
  Scene::init();

  mCurrentSelected = 0;
  mOptionSelected = false;

  Game::instance().setBackgroundMusic("menu.ogg", 100.f);
}

void SceneLevelSelect::update(int deltaTime) {
  Scene::update(deltaTime);

  if (InputManager::getAction(InputManager::Close))
    Game::instance().changeScene(Scene::SCENE_MENU);
  
  if (InputManager::getAction(InputManager::Accept)) {
    Game::instance().getScene()->playSoundEffect("cursorSelect.ogg");
    mOptionSelected = true;
  }

  if (mOptionSelected && mSoundEffects.empty()) {
    std::string levelName = "level" + std::to_string(mCurrentSelected + 1);
    Game::instance().changeScene(Scene::SCENE_TEST);
    Game::instance().getBufferedScene()->receiveString("level-name", levelName);
  }

  if (!mOptionSelected) {
    if (InputManager::getAction(InputManager::Left))
      prevOption();
    if (InputManager::getAction(InputManager::Right))
      nextOption();
  }
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
  unsigned int numLevels = Game::instance().getResource().Int("numLevels") - 1;

  if (mCurrentSelected == 0)
    mCurrentSelected = numLevels;
  else
    mCurrentSelected -= 1;

  Game::instance().getScene()->playSoundEffect("cursorMove.ogg");
}

void SceneLevelSelect::nextOption() {
  unsigned int numLevels = Game::instance().getResource().Int("numLevels") - 1;
  if (mCurrentSelected == numLevels)
    mCurrentSelected = 0;
  else
    mCurrentSelected += 1;

  Game::instance().getScene()->playSoundEffect("cursorMove.ogg");
}