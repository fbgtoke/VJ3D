#ifndef _SCENE_HELP_INCLUDE
#define _SCENE_HELP_INCLUDE

#include "utils.h"
#include "Scene.h"
#include "Model.h"
#include "Camera.h"
#include "Player.h"

class SceneHelp : public Scene {
public:
  SceneHelp();
  ~SceneHelp() override;

  void init() override;
  void update(int deltaTime) override;
  void render() override;

  glm::vec3 getLightDirection() const override;
  glm::mat4 getProjectionMatrix() const override;
  glm::mat4 getViewMatrix() const override;

private:
  Camera mCamera;

  std::vector<Model*> mModels[4];
  int mCountdown;
  void initLayer0();
  void initLayer1();
  void initLayer2();
  void initLayer3();

  void updateLayer0(int deltaTime);

  void initGui() override;

  unsigned int mCurrentLayer;
  void nextLayer();
};

#endif // _SCENE_HELP_INCLUDE