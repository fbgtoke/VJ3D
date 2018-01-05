#include "SceneMenu.h"
#include "Game.h"

SceneMenu::SceneMenu()
  : Scene(Scene::SCENE_MENU) {}

SceneMenu::~SceneMenu() {
  delete mOptions[0];
  delete mOptions[1];
  delete mOptions[2];
  delete mOptions[3];
}

void SceneMenu::init() {
	Scene::init();

	float FOV = (float)M_PI/3.f;
	float ar = (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT;
	float znear = 0.1f;
	float zfar  = 10000.f;
	mProjectionMatrix = glm::perspective(FOV, ar, znear, zfar);

  VRP = glm::vec3(3, -1, -1) * TILE_SIZE;
  OBS = glm::vec3(3, -1, 5) * TILE_SIZE;
  mViewMatrix = glm::lookAt(OBS, VRP, UP);

  kLightDirection.x = Game::instance().getResource().Float("menulightDir_x");
  kLightDirection.y = Game::instance().getResource().Float("menulightDir_y");
  kLightDirection.z = Game::instance().getResource().Float("menulightDir_z");
  kLightDirection = glm::normalize(kLightDirection);

  mCurrentOption = NEW_GAME;
  
  mOptions.push_back(Model::create("newgame.obj", "palette.png"));
  mOptions[0]->setPosition(glm::vec3(0, 0, 0));

  mOptions.push_back(Model::create("help.obj", "palette.png"));
  mOptions[1]->setPosition(glm::vec3(0, -16.f, 0));

  mOptions.push_back(Model::create("credits.obj", "palette.png"));
  mOptions[2]->setPosition(glm::vec3(0, -32.f, 0));

  mOptions.push_back(Model::create("exit.obj", "palette.png"));
  mOptions[3]->setPosition(glm::vec3(0, -48.f, 0));

  Game::instance().setBackgroundMusic("menu.ogg", 100.f);

  mOptionSelected = false;
}

void SceneMenu::update(int deltaTime) {
  Scene::update(deltaTime);

  if (InputManager::getAction(InputManager::Close)) // Escape
    Game::instance().stop();

  if (!mOptionSelected) {
    if (InputManager::getAction(InputManager::Up))
      prevOption();
    else if (InputManager::getAction(InputManager::Down))
      nextOption();
    else if (InputManager::getAction(InputManager::Accept)) {
      Game::instance().getScene()->playSoundEffect("cursorSelect.ogg");
      mOptionSelected = true;
    }

    for (int i = 0; i < NUM_OPTIONS; ++i) {
      if (i == mCurrentOption) {
        mOptions[i]->setScale(glm::vec3(1.2f));

        glm::vec3 rotation(0.f);
        rotation.y = (float)M_PI/16 * sin(0.0048f * mCurrentTime);
        rotation.z = (float)M_PI/16 * sin(0.0023f * mCurrentTime);
        mOptions[i]->setRotation(rotation);
      } else {
        mOptions[i]->setScale(glm::vec3(1.f));
        mOptions[i]->setRotation(glm::vec3(0.f, 0.f, 0.f));
        mOptions[i]->setRotationSpeed(glm::vec3(0.f, 0.f, 0.f));
      }

      mOptions[i]->update(deltaTime);
    }
  }

  if (mOptionSelected && mSoundEffects.empty()) {
    switch (mCurrentOption) {
    case NEW_GAME:
      Game::instance().changeScene(Scene::SCENE_LEVEL_SELECT);
      break;
    case HELP:
      Game::instance().changeScene(Scene::SCENE_HELP);
      break;
    case CREDITS:
      Game::instance().changeScene(Scene::SCENE_CREDITS);
      break;
    case EXIT:
      Game::instance().stop();
      break;
    default:
      break;
    }
  }
}

void SceneMenu::render() {
  Scene::render();
  
  for (int i = 0; i < NUM_OPTIONS; ++i)
    mOptions[i]->render();
}

void SceneMenu::prevOption() {
  if (mCurrentOption == NEW_GAME)
    mCurrentOption = EXIT;
  else
    mCurrentOption = static_cast<MenuOption>(mCurrentOption - 1);

  Game::instance().getScene()->playSoundEffect("cursorMove.ogg");
}

void SceneMenu::nextOption() {
  if (mCurrentOption == EXIT)
    mCurrentOption = NEW_GAME;
  else
    mCurrentOption = static_cast<MenuOption>(mCurrentOption + 1);

  Game::instance().getScene()->playSoundEffect("cursorMove.ogg");
}