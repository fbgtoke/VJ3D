#ifndef _SCENE_HELP_INCLUDE
#define _SCENE_HELP_INCLUDE

#include "utils.h"
#include "Scene.h"
#include "Model.h"

class SceneHelp : public Scene {
public:
  SceneHelp();
  ~SceneHelp() override;

  void init() override;
  void update(int deltaTime) override;
  void render() override;

private:
  void initGui() override;

  unsigned int mCurrentLayer;
  void nextLayer();
};

#endif // _SCENE_HELP_INCLUDE