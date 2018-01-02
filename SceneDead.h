#ifndef _SCENE_DEAD_INCLUDE
#define _SCENE_DEAD_INCLUDE

#include "Scene.h"
#include "Text3D.h"

class SceneDead : public Scene {
public:
  SceneDead();
  ~SceneDead() override;

  void init() override;
  void update(int deltaTime) override;
  void render() override;

  void receiveString(const std::string& tag, const std::string str) override;

private:
  Text3D mText;

  std::string mLevelName;
};

#endif // _SCENE_DEAD_INCLUDE