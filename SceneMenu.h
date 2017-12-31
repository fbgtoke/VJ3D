#ifndef _SCENE_MENU_INCLUDE
#define _SCENE_MENU_INCLUDE

#include "Scene.h"
#include "Model.h"

class SceneMenu : public Scene {
public:
  SceneMenu();
  ~SceneMenu() override;

private:
  void initScene() override;
  void updateScene(int deltaTime) override;

  enum MenuOption {
    NEW_GAME = 0,
    HELP,
    CREDITS,
    EXIT,
    NUM_OPTIONS
  };
  std::vector<Model*> mOptions;
  MenuOption mCurrentOption;

  void prevOption();
  void nextOption();
  void selectOption();
};

#endif // _SCENE_MENU_INCLUDE