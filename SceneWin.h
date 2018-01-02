#ifndef _SCENE_WIN_INCLUDE
#define _SCENE_WIN_INCLUDE

#include "Scene.h"
#include "Text3D.h"

class SceneWin : public Scene {
public:
  SceneWin();
  ~SceneWin() override;

  void init() override;
  void update(int deltaTime) override;
  void render() override;

private:
  Text3D mText;
};

#endif // _SCENE_WIN_INCLUDE