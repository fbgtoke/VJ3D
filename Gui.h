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

  void addSprite(Sprite* sprite);

private:
  static glm::mat4 getProjectionMatrix();
  static glm::mat4 getViewMatrix();

  std::list<Sprite*> mSprites;
};

#endif // _GUI_INCLUDE