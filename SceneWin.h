#ifndef _SCENE_WIN_INCLUDE
#define _SCENE_WIN_INCLUDE

#include "Scene.h"
#include "Text3D.h"

class SceneWin : public Scene {
public:
  SceneWin();
  ~SceneWin() override;

  void update(int deltaTime) override;

private:
  void initGui() override;
};

#endif // _SCENE_WIN_INCLUDE