#ifndef SCENE_LEVEL_SELECT_INCLUDE
#define SCENE_LEVEL_SELECT_INCLUDE

#include "utils.h"
#include "Scene.h"
#include "LevelInfo.h"
#include "Text3D.h"

class SceneLevelSelect : public Scene {
public:
  SceneLevelSelect();
  ~SceneLevelSelect() override;

  void init() override;
  void update(int deltaTime) override;

private:
  unsigned int mCurrentSelected;

  void initGui() override;
};

#endif // SCENE_LEVEL_SELECT_INCLUDE