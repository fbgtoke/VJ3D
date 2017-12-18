#ifndef SCENE_LEVEL_SELECT_INCLUDE
#define SCENE_LEVEL_SELECT_INCLUDE

#include "utils.h"
#include "Scene.h"

class SceneLevelSelect : public Scene {
public:
  SceneLevelSelect();
  ~SceneLevelSelect() override;

private:
  void initScene() override;
  void updateScene(int deltaTime) override;
  void renderScene() override;

  static const int kLevelsPerRow;
  std::vector<Model*> mLevels;
  void initLevelList();
  void addLevel(const std::string& name);

  Model* mFrame;
};

#endif // SCENE_LEVEL_SELECT_INCLUDE