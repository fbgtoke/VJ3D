#include "Text3D.h"
#include "Game.h"

bool Text3D::mInitialized = false;
std::map<char, Mesh*> Text3D::mCharacters = std::map<char, Mesh*>();

Text3D::Text3D() : mString("") {
  if (!mInitialized)
    initCharacters();
}

Text3D::~Text3D() {}

void Text3D::render() {
  glm::vec3 position = mPosition;

  setMesh(mCharacters['0']);
  setTexture(Game::instance().getResource().texture("palette.png"));

  for (char c : mString) {
    /* Caps => Minus */
    if (c >= 'A' && c <= 'Z')
      c += 32;

    if (mCharacters.count(c) != 0) {
      setMesh(mCharacters[c]);
      Model::render();
    }
    move(RIGHT * getSize().x);
  }

  setPosition(position);
}

void Text3D::setString(const std::string& str) { mString = str; }
void Text3D::setPosition(const glm::vec3& position) { mPosition = position; }

void Text3D::initCharacters() {
  std::string file = "0.obj";

  /* Numbers */
  for (char c = '0'; c <= '9'; ++c) {
    file[0] = c;
    mCharacters[c] = Game::instance().getResource().mesh(file);
  }

  /* Letters */
  for (char c = 'a'; c <= 'z'; ++c) {
    file[0] = c;
    mCharacters[c] = Game::instance().getResource().mesh(file);
  }

  mInitialized = true;
}