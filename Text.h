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
  void setFontSize(float size);
  void setFontColor(const glm::vec3& color);

  float getFontSize() const;

private:
  static glm::vec2 char2uvs(char c);
  glm::vec3 mFontColor;

  std::string mString;
};

#endif // _TEXT_INCLUDE