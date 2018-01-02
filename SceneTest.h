#ifndef SCENE_TEST_INCLUDE
#define SCENE_TEST_INCLUDE

#include "Scene.h"
#include "Tilemap.h"
#include "Level.h"
#include "LevelGenerator.h"
#include "Player.h"
#include "FrameBuffer.h"
#include "DepthBuffer.h"

class SceneTest : public Scene {
public:
	SceneTest();
	~SceneTest() override;

  void render() override;

  void receiveString(const std::string& tag, const std::string str) override;
  void removeModel(Model* model) override;

  glm::mat4 getProjectionMatrix() const override;
  glm::mat4 getDepthProjectionMatrix() const;

  glm::mat4 getViewMatrix() const override;
  glm::mat4 getDepthViewMatrix() const;

  glm::vec3 getLightDirection() const override;

private:
  float mLightAngle;
	glm::vec3 kObsVector;
  float kCameraVel;

  float mCameraVel;
  void initCamera();
  void updateCamera(int deltaTime);

	void initScene() override;
	void updateScene(int deltaTime) override;
  
  void renderShadowmap();
  void renderFramebuffer();
  void renderScene();

  void initPlayer();
  void checkPlayerInput();
  void checkPlayerOutOfCamera();
  void checkPlayerStandingTile();
  void checkPlayerDead();

  Level* mLevel;
  std::string mLevelName;

  Player* mPlayer;

  FrameBuffer mFramebuffer;
  DepthBuffer mDepthbuffer;
};

#endif // SCENE_TEST_INCLUDE