#include "SceneDead.h"
#include "Game.h"

SceneDead::SceneDead()
  : Scene(Scene::SCENE_DEAD), mLevelName("") {}

SceneDead::~SceneDead() {}

void SceneDead::receiveString(const std::string& tag, const std::string str) {
  if (tag == "level-name")
    mLevelName = str;
}

void SceneDead::initScene() {
  Scene::initScene();

  float FOV = (float)M_PI/3.f;
  float ar = (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT;
  float znear = 0.1f;
  float zfar  = 10000.f;
  mProjectionMatrix = glm::perspective(FOV, ar, znear, zfar);

  VRP = glm::vec3(3, -1, -1) * TILE_SIZE;
  OBS = glm::vec3(3, -1, 5) * TILE_SIZE;
  mViewMatrix = glm::lookAt(OBS, VRP, UP);

  mText.setString("YOU DIED");
  mText.setPosition(glm::vec3(0.f));
}

void SceneDead::updateScene(int deltaTime) {
  Scene::updateScene(deltaTime);

  if (Game::instance().getKeyPressed(27)) // Escape
    Game::instance().changeScene(Scene::SCENE_MENU);

  if (Game::instance().getKeyPressed('z')) {
    if (mLevelName == "") {
      Game::instance().changeScene(Scene::SCENE_MENU);
    } else {
      Game::instance().changeScene(Scene::SCENE_PLAY);
      Game::instance().getBufferedScene()->receiveString("level-name", mLevelName);
    }
  }
}