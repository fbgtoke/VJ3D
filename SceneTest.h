#ifndef SCENE_TEST_INCLUDE
#define SCENE_TEST_INCLUDE

#include "Scene.h"
#include "Level.h"
#include "Player.h"
#include "FrameBuffer.h"
#include "DepthBuffer.h"
#include "Sun.h"
#include "Camera.h"

class SceneTest : public Scene {
public:
	SceneTest();
	~SceneTest() override;

  void init() override;
  void update(int deltaTime) override;
  void render() override;

  void receiveString(const std::string& tag, const std::string str) override;

private:
  std::string mLevelName;
  Level mLevel;
  
  Camera mCamera;
  Sun mSun;
  
  void renderShadowmap();
  void renderFramebuffer();
  void renderScene();

  void initPlayer();
  void checkPlayerInput();
  void checkPlayerOutOfBounds();
  void checkPlayerOutOfCamera();
  void checkPlayerStandingTile();

  Obstacle* getBoatAdjacentToPlayer(const glm::vec3& direction);
  Obstacle* getStoneAdjacentToPlayer(const glm::vec3& direction);

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

  enum State {
    Ready,
    Go,
    Playing,
    Dead,
    Win
  };
  SceneTest::State mState;
  void changeState(SceneTest::State state);
};

#endif // SCENE_TEST_INCLUDE
