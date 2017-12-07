#include "SceneMenu.h"
#include "Game.h"

SceneMenu::SceneMenu() {}

SceneMenu::~SceneMenu() {}

void SceneMenu::initScene() {
	Scene::initScene();

	float FOV = (float)M_PI/3.f;
	float ar = (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT;
	float znear = 0.1f;
	float zfar  = 10000.f;
	mProjectionMatrix = glm::perspective(FOV, ar, znear, zfar);

  VRP = glm::vec3(3, -1, -1) * TILE_SIZE;
  OBS = glm::vec3(3, -1, 5) * TILE_SIZE;
  mViewMatrix = glm::lookAt(OBS, VRP, UP);

  mCurrentOption = NEW_GAME;
  
  mOptions[0].init();
  mOptions[0].setMesh(Game::instance().getResource().mesh("newgame.obj"));
  mOptions[0].setTexture(Game::instance().getResource().texture("newgame.png"));
  mOptions[0].setPosition(glm::vec3(0, 0, 0));

  mOptions[1].init();
  mOptions[1].setMesh(Game::instance().getResource().mesh("help.obj"));
  mOptions[1].setTexture(Game::instance().getResource().texture("help.png"));
  mOptions[1].setPosition(glm::vec3(0, -16.f, 0));

  mOptions[2].init();
  mOptions[2].setMesh(Game::instance().getResource().mesh("credits.obj"));
  mOptions[2].setTexture(Game::instance().getResource().texture("credits.png"));
  mOptions[2].setPosition(glm::vec3(0, -32.f, 0));

  mOptions[3].init();
  mOptions[3].setMesh(Game::instance().getResource().mesh("exit.obj"));
  mOptions[3].setTexture(Game::instance().getResource().texture("exit.png"));
  mOptions[3].setPosition(glm::vec3(0, -48.f, 0));
}

void SceneMenu::updateScene(int deltaTime) {
  Scene::updateScene(deltaTime);

  if (Game::instance().getKeyPressed(27)) // Escape
    Game::instance().stop();

  if (Game::instance().getKeyPressed('w'))
    prevOption();
  else if (Game::instance().getKeyPressed('s'))
    nextOption();
  else if (Game::instance().getKeyPressed('z'))
    selectOption();

  for (int i = 0; i < NUM_OPTIONS; ++i) {
    if (i == mCurrentOption) {
      mOptions[i].setScale(glm::vec3(1.2f));

      glm::vec3 rotation(0.f);
      rotation.y = (float)M_PI/16 * sin(0.0048f * mCurrentTime);
      rotation.z = (float)M_PI/16 * sin(0.0023f * mCurrentTime);
      mOptions[i].setRotation(rotation);
    } else {
      mOptions[i].setScale(glm::vec3(1.f));
      mOptions[i].setRotation(glm::vec3(0.f, 0.f, 0.f));
      mOptions[i].setRotationSpeed(glm::vec3(0.f, 0.f, 0.f));
    }

    mOptions[i].update(deltaTime);
  }
}

void SceneMenu::renderScene() {
	Scene::renderScene();

  for (int i = 0; i < NUM_OPTIONS; ++i)
    mOptions[i].render();
}

void SceneMenu::prevOption() {
  if (mCurrentOption == NEW_GAME)
    mCurrentOption = EXIT;
  else
    mCurrentOption = static_cast<MenuOption>(mCurrentOption - 1);
}

void SceneMenu::nextOption() {
  if (mCurrentOption == EXIT)
    mCurrentOption = NEW_GAME;
  else
    mCurrentOption = static_cast<MenuOption>(mCurrentOption + 1);
}

void SceneMenu::selectOption() {
  switch (mCurrentOption) {
  case NEW_GAME:
    Game::instance().changeScene(Scene::SCENE_TEST);
    break;
  case EXIT:
    Game::instance().stop();
    break;
  default:
    break;
  }
}