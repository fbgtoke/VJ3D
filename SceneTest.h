#ifndef SCENE_TEST_INCLUDE
#define SCENE_TEST_INCLUDE

#include "Scene.h"
#include "LvlReader.h"
#include "Chunk.h"
#include "Obstacle.h"
#include "Player.h"

class SceneTest : public Scene {
public:
	SceneTest();
	~SceneTest() override;

private:
	static const glm::vec3 kObsVector;

	void initScene() override;
	void updateScene(int deltaTime) override;
	void renderScene() override;

  Player mPlayer;
  void updatePlayer(int deltaTime);
  void checkPlayerChunk();
  void checkPlayerCollisions();

	std::list<Chunk*> mChunks;
};

#endif // SCENE_TEST_INCLUDE