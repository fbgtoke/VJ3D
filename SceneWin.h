#ifndef _SCENE_WIN_INCLUDE
#define _SCENE_WIN_INCLUDE

#include "Scene.h"
#include "Text3D.h"

class SceneWin : public Scene {
public:
  SceneWin();
  ~SceneWin() override;

private:
  void initScene() override;
  void updateScene(int deltaTime) override;
  void renderScene() override;

  Text3D mText;
};

#endif // _SCENE_WIN_INCLUDE