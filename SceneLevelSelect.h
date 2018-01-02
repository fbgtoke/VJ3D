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
  void render() override;

private:
  void initLevelList();
  void addLevel(const std::string& name);

  Model* mFrame;
  unsigned int mCurrentSelected;

  int kLevelsPerRow;
  float kOptionSize;
  std::vector<LevelInfo> mLevels;
  std::vector<Model*> mThumbnails;

  Text3D* mHighscoreTitle;
  std::vector<Text3D*> mHighscoreTable;
  void initHighscoreText();
  void updateHighscoreTable();

  void nextIndex();
  void prevIndex();
  glm::vec3 index2position(unsigned int index) const;
};

#endif // SCENE_LEVEL_SELECT_INCLUDE