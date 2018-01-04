#include "Text.h"
#include "Game.h"

Text::Text() {}
Text::~Text() {}

void Text::init() {
  Sprite::init();

  setShader(Game::instance().getResource().shader("text"));

  setString("");
  setFontSize(64.f);
  setFontColor(glm::vec3(1.f));
  
  mModel.setMesh(Game::instance().getResource().mesh("letter.obj"));
  mModel.setTexture(Game::instance().getResource().texture("font.png"));
}

void Text::render() {
  mShader->setUniform3f("fontcolor", mFontColor);
  
  glm::vec3 position = mModel.getPosition();

  for (unsigned int i = 0; i < mString.size(); ++i) {
    char c = mString[i];

    move(glm::vec2(getFontSize(), 0.f));

    glm::vec2 texOffset = char2uvs(c);
    mShader->setUniform2f("texOffset", texOffset);

    Sprite::render();
  }

  mModel.setPosition(position);
}

void Text::setString(const std::string& str) { mString  = str; }
void Text::setFontSize(float size) { resize(glm::vec2(size)); }
void Text::setFontColor(const glm::vec3& color) { mFontColor = color; }

float Text::getFontSize() const { return mModel.getScale().x; }

glm::vec2 Text::char2uvs(char c) {
  return glm::vec2((float)c/128.f, 0.f);
}