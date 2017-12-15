#include "SceneTest.h"
#include "Game.h"

const glm::vec3 SceneTest::kObsVector = glm::vec3(1, 5, 4);
const float SceneTest::kCameraVel = -0.025f;

SceneTest::SceneTest() {}

SceneTest::~SceneTest() {
  if (mLevel != nullptr)
    delete mLevel;
}

void SceneTest::initScene() {
	Scene::initScene();

	float left = 6 * TILE_SIZE * (-1.f);
	float right = 6 * TILE_SIZE;
  float bottom = 2 * TILE_SIZE * (-1.f);
  float top = (float)SCREEN_HEIGHT/(float)SCREEN_WIDTH * (right - left) + bottom;
	float znear = 0.1f;
	float zfar  = 10000.f;
  mProjectionMatrix = glm::ortho(left, right, bottom, top, znear, zfar);

  mLevel = LevelGenerator::generate("levels/level1");

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
  if (player->isDead())
    Game::instance().changeScene(Scene::SCENE_DEAD);
}

void SceneTest::checkPlayerOutOfCamera() {
  glm::vec4 homoPosition(mLevel->getPlayer()->getCenter(), 1.0f);
  glm::vec4 projectedPosition =
    glm::vec4(mProjectionMatrix * mViewMatrix * homoPosition);

  if (projectedPosition.y/projectedPosition.w < -1.0f)
    mLevel->getPlayer()->explode();
}

/*
void SceneTest::updatePlayer(int deltaTime) {
  mPlayer.update(deltaTime);
  
  if (mPlayer.isIdle())
    checkPlayerChunk();

  if (mPlayer.isAlive()) {
    checkPlayerCollisions();
    checkPlayerOutOfCamera();
  }

  if (mPlayer.isDead())
    Game::instance().changeScene(Scene::SCENE_DEAD);
}

void SceneTest::checkPlayerChunk() {
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
        killPlayer();
    }
  }
}

void SceneTest::checkPlayerCollisions() {
  const Obstacle* collided;
  for (Chunk* chunk : mChunks) {
    collided = chunk->checkCollisions(mPlayer);
    if (collided != nullptr) {
      switch (collided->getType()) {
      case Obstacle::LILLYPAD:
        std::cout << "Standing on lillypad" << std::endl;
        break;
      case Obstacle::TREE:
      case Obstacle::CAR:
        killPlayer();
        break;
      case Obstacle::BONUS:
        std::cout << "Bonus get" << std::endl;
        chunk->removeObstacle(collided);
        break;
      default:
        break;
      }
    }
  }
}
*/