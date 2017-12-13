#ifndef _SCENE_DEAD_INCLUDE
#define _SCENE_DEAD_INCLUDE

#include "Scene.h"
#include "Text3D.h"

class SceneDead : public Scene {
public:
  SceneDead();
  ~SceneDead() override;

private:
  void initScene() override;
  void updateScene(int deltaTime) override;
  void renderScene() override;

  Text3D mText;
};

#endif // _SCENE_DEAD_INCLUDE