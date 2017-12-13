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

  updatePlayer(deltaTime);

  for (Chunk* chunk : mChunks)
    chunk->update(deltaTime);

  const Obstacle* collided;
  for (Chunk* chunk : mChunks) {
    collided = chunk->checkCollisions(mPlayer);
    if (collided != nullptr) {
      std::cout << "Collision at " << mCurrentTime << std::endl;

      if (collided->getType() != Obstacle::LILLYPAD) {
        mPlayer.explode();
        Game::instance().changeScene(Scene::SCENE_DEAD);
      }
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

void SceneTest::updatePlayer(int deltaTime) {
  mPlayer.update(deltaTime);
  int playerDepth = mPlayer.getPositionInTiles().z * (-1);
  int playerOffset = mPlayer.getPositionInTiles().x;
  
  Chunk::ChunkType chunkType;
  int chunkDepth;

  for (Chunk* chunk : mChunks) {
    chunkType = chunk->getType();
    chunkDepth = chunk->getDepth();

    if (chunkType == Chunk::GOAL && chunkDepth == playerDepth) {
      Game::instance().changeScene(Scene::SCENE_WIN);
    }

    if (chunkType == Chunk::WATER && chunkDepth == playerDepth) {
      if (!chunk->hasObstacleAtPosition(Obstacle::LILLYPAD, playerOffset))
        Game::instance().changeScene(Scene::SCENE_DEAD);
    }
  }
}