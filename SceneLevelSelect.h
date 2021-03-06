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

  void receiveString(const std::string& tag, const std::string str) override;

private:
  std::string mLevelName;

  unsigned int mCurrentSelected;
  bool mOptionSelected;
  void prevOption();
  void nextOption();

  void initGui() override;
  void updateGui(int deltaTime) override;
};

#endif // SCENE_LEVEL_SELECT_INCLUDE