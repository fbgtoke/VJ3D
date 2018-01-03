#ifndef _SCENE_CREDITS_INCLUDE
#define _SCENE_CREDITS_INCLUDE

#include "utils.h"
#include "Scene.h"

class SceneCredits : public Scene {
public:
  SceneCredits();
  ~SceneCredits() override;

  void update(int deltaTime) override;

private:
  void initGui() override;
};

#endif // _SCENE_CREDITS_INCLUDE