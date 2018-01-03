#include "Text.h"
#include "Game.h"

Text::Text() {}
Text::~Text() {}

void Text::init() {
  Sprite::init();

  resize(glm::vec2(64.f, 64.f));
  setShader(Game::instance().getResource().shader("text"));
  setTexture(Game::instance().getResource().texture("font.png"));
  setString("");
  setFontColor(glm::vec3(1.f));
}

void Text::render() {
  mShader->setUniform3f("fontColor", mFontColor);

  glm::vec2 position = mPosition;

  for (int i = 0; i < mString.size(); ++i) {
    char c = mString[i];

    setTextureRect(char2uvs(c));
    move(glm::vec2(mSize.x, 0.f));

    Sprite::render();
  }

  mPosition = position;
}

void Text::setString(const std::string& str) { mString  = str; }
void Text::setFontColor(const glm::vec3& color) { mFontColor  = color; }

glm::vec4 Text::char2uvs(char c) {
  return glm::vec4(
    (float)c/128.f,
    ((float)c + 1.f)/128.f,
    0.f,
    1.f
  );
}