#ifndef SCENE_TEST_INCLUDE
#define SCENE_TEST_INCLUDE

#include "Scene.h"
#include "Tilemap.h"
#include "Level.h"
#include "LevelGenerator.h"
#include "FrameBuffer.h"

class SceneTest : public Scene {
public:
	SceneTest();
	~SceneTest() override;

  void receiveString(const std::string& tag, const std::string str) override;
  void removeModel(Model* model) override;

  glm::vec3 getLightDirection() const override;
  float getAmbientLight() const override;

private:
  float mLightAngle;
	glm::vec3 kObsVector;
  float kCameraVel;

  float mCameraVel;
  void updateCamera(int deltaTime);

	void initScene() override;
	void updateScene(int deltaTime) override;
  
  void beforeRender() override;
  void afterRender() override;

  void checkPlayerOutOfCamera();
  void checkPlayerDead();

  Level* mLevel;
  std::string mLevelName;
  FrameBuffer mFramebuffer;
};

#endif // SCENE_TEST_INCLUDE