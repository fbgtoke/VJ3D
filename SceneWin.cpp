#include "SceneWin.h"
#include "Game.h"

SceneWin::SceneWin() {}

SceneWin::~SceneWin() {}

void SceneWin::initScene() {
  Scene::initScene();

  float FOV = (float)M_PI/3.f;
  float ar = (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT;
  float znear = 0.1f;
  float zfar  = 10000.f;
  mProjectionMatrix = glm::perspective(FOV, ar, znear, zfar);

  VRP = glm::vec3(3, -1, -1) * TILE_SIZE;
  OBS = glm::vec3(3, -1, 5) * TILE_SIZE;
  mViewMatrix = glm::lookAt(OBS, VRP, UP);

  mText.setString("YOU WON");
  mText.setPosition(glm::vec3(0.f));
}

void SceneWin::updateScene(int deltaTime) {
  Scene::updateScene(deltaTime);

  if (Game::instance().getKeyPressed(27)) // Escape
    Game::instance().stop();

  if (Game::instance().getKeyPressed('z'))
    Game::instance().changeScene(Scene::SCENE_MENU);
}

void SceneWin::renderScene() {
  Scene::renderScene();

  mText.render();
}