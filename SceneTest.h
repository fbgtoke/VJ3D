#ifndef SCENE_TEST_INCLUDE
#define SCENE_TEST_INCLUDE

#include "Scene.h"
#include "Tilemap.h"
#include "Level.h"
#include "LevelGenerator.h"

class SceneTest : public Scene {
public:
	SceneTest();
	~SceneTest() override;

  void receiveString(const std::string& tag, const std::string str) override;
  void removeModel(Model* model) override;

private:
	glm::vec3 kObsVector;
  float kCameraVel;

  float mCameraVel;
  void updateCamera(int deltaTime);

	void initScene() override;
	void updateScene(int deltaTime) override;
	void renderScene() override;

  void checkPlayerOutOfCamera();
  void checkPlayerDead();

  Level* mLevel;
  std::string mLevelName;
};

#endif // SCENE_TEST_INCLUDE