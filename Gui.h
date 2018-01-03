#ifndef _GUI_INCLUDE
#define _GUI_INCLUDE

#include "utils.h"
#include "Sprite.h"

class Gui {
public:
  Gui();
  ~Gui();

  void init();
  void render();
  void clear();

  void setNumberOfLayers(unsigned int n);
  void addSprite(Sprite* sprite, unsigned int layer = 0);
  Sprite* getSprite(const std::string& name);

private:
  static glm::mat4 getProjectionMatrix();
  static glm::mat4 getViewMatrix();

  std::vector<std::vector<Sprite*>> mLayers;
};

#endif // _GUI_INCLUDE