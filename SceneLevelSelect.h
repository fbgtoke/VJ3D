#ifndef SCENE_LEVEL_SELECT_INCLUDE
#define SCENE_LEVEL_SELECT_INCLUDE

#include "utils.h"
#include "Scene.h"

class SceneLevelSelect : public Scene {
public:
  SceneLevelSelect();
  ~SceneLevelSelect() override;

  void init() override;
  void update(int deltaTime) override;

private:
  unsigned int mCurrentSelected;
  bool mOptionSelected;
  void prevOption();
  void nextOption();

  void initGui() override;
  void updateGui() override;
};

#endif // SCENE_LEVEL_SELECT_INCLUDE