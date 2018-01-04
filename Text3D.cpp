#include "Text3D.h"
#include "Game.h"

bool Text3D::mInitialized = false;
std::map<char, Mesh*> Text3D::mCharacters = std::map<char, Mesh*>();

Text3D::Text3D() : mString("") {
  if (!mInitialized)
    initCharacters();
}

Text3D::~Text3D() {
  for (Model* model : mModels) delete model;
}

void Text3D::render() {
  for (Model* model : mModels)
    model->render();
}

void Text3D::setString(const std::string& str) {
  mString = str;

  for (Model* model : mModels) delete model;
  mModels.clear();

  for (unsigned int i = 0; i < mString.size(); ++i) {
    char c = mString[i];
    /* Caps => Minus */
    if (c >= 'A' && c <= 'Z')
      c += 32;

    Model* model = new Model();
    model->init();
    model->setMesh(mCharacters[c]);
    model->setTexture(Game::instance().getResource().texture("palette.png"));
    model->setPosition(mPosition + getCharacterWidth() * (float)i * RIGHT);
    mModels.push_back(model);
  }
}

void Text3D::setPosition(const glm::vec3& position) {
  mPosition = position;

  for (unsigned int i = 0; i < mModels.size(); ++i)
    mModels[i]->setPosition(position + getCharacterWidth() * RIGHT * (float)i);
}

void Text3D::setVisible(bool visible) {
  for (unsigned int i = 0; i < mModels.size(); ++i) {
    mModels[i]->setVisible(visible);
  }
}

float Text3D::getCharacterWidth() const {
  float width;

  Model* m = new Model();
  m->init();
  m->setMesh(mCharacters['m']);
  width = m->getSize().x;
  delete m;

  return width;
}

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