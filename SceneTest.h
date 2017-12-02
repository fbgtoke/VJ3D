#ifndef SCENE_TEST_INCLUDE
#define SCENE_TEST_INCLUDE

#include "Scene.h"
#include "Chunk.h"
#include "Player.h"

class SceneTest : public Scene {
public:
	SceneTest();
	~SceneTest() override;

private:
	static const glm::vec3 kLightDirection;
	static const float kAmbientLight;
	static const glm::vec3 kObsVector;

	glm::vec3 OBS, VRP, up;

	void initScene() override;
	void updateScene(int deltaTime) override;
	void renderScene() override;

	Player mPlayer;
	Chunk mChunks[100];
};

#endif // SCENE_TEST_INCLUDE