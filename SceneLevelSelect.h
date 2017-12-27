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

  void initLevelList();
  void addLevel(const std::string& name);

  Model* mFrame;
  unsigned int mCurrentSelected;

  int kLevelsPerRow;
  float kOptionSize;
  std::vector<Model*> mLevels;

  void nextIndex();
  void prevIndex();
  glm::vec3 index2position(unsigned int index) const;

};

#endif // SCENE_LEVEL_SELECT_INCLUDE