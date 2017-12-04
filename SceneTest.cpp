#include "SceneTest.h"
#include "Game.h"

const glm::vec3 SceneTest::kObsVector = glm::vec3(5, 8, 10);

SceneTest::SceneTest() {}

SceneTest::~SceneTest() {
  for (Chunk* chunk : mChunks)
    if (chunk != nullptr)
      delete chunk;

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
  mPlayer.setMesh(Game::instance().getResource().mesh("cowboy.obj"));
  mPlayer.setTexture(Game::instance().getResource().texture("cowboy.png"));
  mPlayer.setPositionInTiles(glm::vec3(TILES_PER_CHUNK/2, 0, 0));

  VRP = mPlayer.getCenter();
  OBS = VRP + kObsVector * TILE_SIZE;

  LvlReader reader;
  reader.loadFromFile("levels/test.lvl", mChunks);
}

void SceneTest::updateScene(int deltaTime) {
  Scene::updateScene(deltaTime);

  if (Game::instance().getKeyPressed(27)) // Escape
    Game::instance().changeScene(Scene::SCENE_MENU);

  mPlayer.update(deltaTime);
  for (Chunk* chunk : mChunks)
    chunk->update(deltaTime);

  VRP = mPlayer.getCenter();
  OBS = VRP + glm::vec3(5, 8, 10) * TILE_SIZE;

  mViewMatrix = glm::lookAt(OBS, VRP, UP);
}

void SceneTest::renderScene() {
	Scene::renderScene();

  mPlayer.render();
	
  for (Chunk* chunk : mChunks)
    chunk->render();
}