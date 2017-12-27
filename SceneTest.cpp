#include "SceneTest.h"
#include "Game.h"

SceneTest::SceneTest()
  : mLevelName("") {}

SceneTest::~SceneTest() {
  if (mLevel != nullptr)
    delete mLevel;
}

void SceneTest::receiveString(const std::string& tag, const std::string str) {
  if (tag == "level-name")
    mLevelName = str;
}

void SceneTest::initScene() {
	Scene::initScene();

  if (mLevelName == "") {
    std::cout << "No level selected!" << std::endl;
    Game::instance().changeScene(Scene::SCENE_MENU);
    return;
  }

  kObsVector.x = Game::instance().getResource().Float("ObsVector_x");
  kObsVector.y = Game::instance().getResource().Float("ObsVector_y");
  kObsVector.z = Game::instance().getResource().Float("ObsVector_z");
  kCameraVel = Game::instance().getResource().Float("CameraVel");

	float left = 6 * TILE_SIZE * (-1.f);
	float right = 6 * TILE_SIZE;
  float bottom = 2 * TILE_SIZE * (-1.f);
  float top = (float)SCREEN_HEIGHT/(float)SCREEN_WIDTH * (right - left) + bottom;
	float znear = 0.1f;
	float zfar  = 10000.f;
  mProjectionMatrix = glm::ortho(left, right, bottom, top, znear, zfar);

  mLevel = LevelGenerator::generate("levels/" + mLevelName + "/");

  mCameraVel = kCameraVel;
  VRP = mLevel->getPlayer()->getCenter();
  OBS = VRP + kObsVector * TILE_SIZE;
}

void SceneTest::updateScene(int deltaTime) {
  Scene::updateScene(deltaTime);

  if (Game::instance().getKeyPressed(27)) // Escape
    Game::instance().changeScene(Scene::SCENE_MENU);

  updateCamera(deltaTime);
  mLevel->update(deltaTime);

  checkPlayerOutOfCamera();
  checkPlayerDead();
}

void SceneTest::renderScene() {
	Scene::renderScene();

  mLevel->render();
}

void SceneTest::updateCamera(int deltaTime) {
  VRP.x = mLevel->getPlayer()->getCenter().x;
  VRP.y = 0.f;
  VRP.z += mCameraVel * (float)deltaTime;
  OBS = VRP + kObsVector * TILE_SIZE;

  mViewMatrix = glm::lookAt(OBS, VRP, UP);
}

void SceneTest::checkPlayerDead() {
  Player* player = mLevel->getPlayer();
  if (player == nullptr) return;

  if (player->isExploding())
    mCameraVel = 0.f;
  if (player->isDead()) {
    Game::instance().changeScene(Scene::SCENE_DEAD);
    Game::instance().getBufferedScene()->receiveString("level-name", mLevelName);
  }
}

void SceneTest::checkPlayerOutOfCamera() {
  glm::vec4 homoPosition(mLevel->getPlayer()->getCenter(), 1.0f);
  glm::vec4 projectedPosition =
    glm::vec4(mProjectionMatrix * mViewMatrix * homoPosition);

  if (projectedPosition.y/projectedPosition.w < -1.0f)
    mLevel->getPlayer()->explode();
}