#ifndef _SCENE_HELP_INCLUDE
#define _SCENE_HELP_INCLUDE

#include "utils.h"
#include "Scene.h"

class SceneHelp : public Scene {
public:
  SceneHelp();
  ~SceneHelp() override;

  void update(int deltaTime) override;

private:
  void initGui() override;
};

#endif // _SCENE_HELP_INCLUDE