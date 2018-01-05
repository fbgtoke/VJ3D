#ifndef _GUI_INCLUDE
#define _GUI_INCLUDE

#include "utils.h"
#include "Sprite.h"
#include "Text.h"

class Gui {
public:
  Gui();
  ~Gui();

  void init();
  void render();
  void clear();

  void setNumberOfLayers(unsigned int n);
  void hideLayer(unsigned int n);
  void showLayer(unsigned int n);
  bool isLayerVisible(unsigned int n) const;

  void addSprite(Sprite* sprite, unsigned int layer = 0);
  Sprite* getSprite(const std::string& name);

  Text* getText(const std::string& name);

private:
  static glm::mat4 getProjectionMatrix();
  static glm::mat4 getViewMatrix();

  std::vector<std::vector<Sprite*>> mLayers;
  std::vector<bool> mVisible;
};

#endif // _GUI_INCLUDE