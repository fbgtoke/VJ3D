#include "SceneTest.h"
#include "Game.h"

const glm::vec3 SceneTest::kObsVector = glm::vec3(5, 8, 10);

SceneTest::SceneTest() {}

SceneTest::~SceneTest() {
  for (auto it = mChunks.begin(); it != mChunks.end(); ++it)
    delete (*it);
  mChunks.clear();
}

void SceneTest::initScene() {
	Scene::initScene();

	float FOV = (float)M_PI/3.f;
	float ar = (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT;
	float znear = 0.1f;
	float zfar  = 10000.f;
	mProjectionMatrix = glm::perspective(FOV, ar, znear, zfar);

  mPlayer.init();
  mPlayer.setPositionInTiles(glm::vec3(0));

  VRP = mPlayer.getCenter();
  OBS = VRP + kObsVector * TILE_SIZE;

  LvlReader::loadFromFile("levels/test.lvl", mChunks);
}

void SceneTest::updateScene(int deltaTime) {
  Scene::updateScene(deltaTime);

  if (Game::instance().getKeyPressed(27)) // Escape
    Game::instance().changeScene(Scene::SCENE_MENU);

  mPlayer.update(deltaTime);
  for (Chunk* chunk : mChunks)
    chunk->update(deltaTime);

  const Model* collided;
  for (Chunk* chunk : mChunks) {
    collided = chunk->checkCollisions(mPlayer);
    if (collided != nullptr) {
      std::cout << "Collision at " << mCurrentTime << std::endl;
      mPlayer.explode();
    }
  }

  VRP.x = mPlayer.getCenter().x;
  VRP.z = mPlayer.getCenter().z;
  OBS = VRP + kObsVector * TILE_SIZE;

  mViewMatrix = glm::lookAt(OBS, VRP, UP);
}

void SceneTest::renderScene() {
	Scene::renderScene();

  mPlayer.render();
	
  for (Chunk* chunk : mChunks)
    chunk->render();
}