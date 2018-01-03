#ifndef _TEXT_INCLUDE
#define _TEXT_INCLUDE

#include "utils.h"
#include "Sprite.h"

class Text : public Sprite {
public:
  Text();
  ~Text() override;

  void init() override;
  void render() override;

  void setString(const std::string& str);
  void setFontColor(const glm::vec3& color);

private:
  static glm::vec4 char2uvs(char c);
  std::string mString;
  glm::vec3 mFontColor;
};

#endif // _TEXT_INCLUDE