#ifndef SCENE_TEST_INCLUDE
#define SCENE_TEST_INCLUDE

#include "Scene.h"
#include "Tilemap.h"
#include "Level.h"
#include "LevelInfo.h"
#include "LevelGenerator.h"
#include "Player.h"
#include "FrameBuffer.h"
#include "DepthBuffer.h"

class SceneTest : public Scene {
public:
	SceneTest();
	~SceneTest() override;

  void init() override;
  void update(int deltaTime) override;
  void render() override;

  void receiveString(const std::string& tag, const std::string str) override;

  glm::mat4 getProjectionMatrix() const override;
  glm::mat4 getDepthProjectionMatrix() const;

  glm::mat4 getViewMatrix() const override;
  glm::mat4 getDepthViewMatrix() const;

  glm::mat4 getDepthBiasMatrix() const;

  glm::vec3 getLightDirection() const override;

  bool doUpdate(Model* model) const override;

private:
  float mLightAngle;
	glm::vec3 kObsVector;
  float kCameraVel;

  float mCameraVel;
  void initCamera();
  void updateCamera(int deltaTime);
  
  void renderShadowmap();
  void renderFramebuffer();
  void renderScene();

  void initPlayer();
  void checkPlayerInput();
  void checkPlayerOutOfBounds();
  void checkPlayerOutOfCamera();
  void checkPlayerStandingTile();
  void checkPlayerDead();

  Obstacle* getBoatAdjacentToPlayer(const glm::vec3& direction);
  Obstacle* getStoneAdjacentToPlayer(const glm::vec3& direction);

  LevelInfo mLevelInfo;
  Level* mLevel;
  std::string mLevelName;

  Player* mPlayer;

  unsigned int kScorePerTile;
  unsigned int kScorePerSideWalk;
  void addScore(unsigned int score);
  void removeScore(unsigned int score);

  FrameBuffer mFramebuffer;
  DepthBuffer mDepthbuffer;

  void initGui() override;
  void updateGui() override;
  void renderGui() override;
};

#endif // SCENE_TEST_INCLUDE