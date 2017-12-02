#include "SceneTest.h"
#include "Game.h"

const glm::vec3 SceneTest::kLightDirection = glm::normalize(glm::vec3(0.0, -4.0, -1.0));
const float SceneTest::kAmbientLight = 0.4f;

SceneTest::SceneTest() {}
SceneTest::~SceneTest() {}

void SceneTest::initScene() {
	Scene::initScene();

	float FOV = (float)M_PI/3.f;
	float ar = (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT;
	float znear = 0.1f;
	float zfar  = 10000.f;
	mProjectionMatrix = glm::perspective(FOV, ar, znear, zfar);

  mPlayer.init();
  mPlayer.setMesh(Game::instance().getResource().mesh("cowboy.obj"));
  mPlayer.setTexture(Game::instance().getResource().texture("cowboy.png"));
  mPlayer.setPositionInTiles(glm::vec3(TILES_PER_CHUNK/2, 0, 0));

  VRP = mPlayer.getCenter();
  OBS = VRP + glm::vec3(5, 8, 10) * TILE_SIZE;
  up  = glm::vec3( 0, 1,   0);

  for (int i = 0; i < 100; ++i) {
    mChunks[i].init();
    mChunks[i].setDepth(i);
  }
}

void SceneTest::updateScene(int deltaTime) {
  Scene::updateScene(deltaTime);

  if (Game::instance().getKeyPressed(27)) // Escape
    Game::instance().stop();

  mPlayer.update(deltaTime);

  VRP = mPlayer.getCenter();
  OBS = VRP + glm::vec3(5, 8, 10) * TILE_SIZE;

  mViewMatrix = glm::lookAt(OBS, VRP, up);
}

void SceneTest::renderScene() {
	Scene::renderScene();

	mTexProgram->setUniform3f("lightDir", kLightDirection.x, kLightDirection.y, kLightDirection.z);
	mTexProgram->setUniform1f("ambientColor", kAmbientLight);

  mPlayer.render();
	
  for (int i = 0; i < 100; ++i)
    mChunks[i].render();
}