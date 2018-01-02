#include "SceneDead.h"
#include "Game.h"

SceneDead::SceneDead()
  : Scene(Scene::SCENE_DEAD), mLevelName("") {}

SceneDead::~SceneDead() {}

void SceneDead::receiveString(const std::string& tag, const std::string str) {
  if (tag == "level-name")
    mLevelName = str;
}

void SceneDead::init() {
  Scene::init();

  float left = 0.f;
  float right = SCREEN_WIDTH * 0.125f;
  float bottom = 0.f;
  float top = SCREEN_HEIGHT * 0.125f;
  float znear = 0.1f;
  float zfar  = 10000.f;
  mProjectionMatrix = glm::ortho(left, right, bottom, top, znear, zfar);

  VRP = glm::vec3(0.f, 0.f, -1);
  OBS = glm::vec3(0.f, 0.f, 5);
  mViewMatrix = glm::lookAt(OBS, VRP, UP);

  mText.setString("YOU DIED");
  mText.setPosition(glm::vec3(0.f));
}

void SceneDead::update(int deltaTime) {
  Scene::update(deltaTime);

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

void SceneDead::render() {
  Scene::render();

  mText.render();
}