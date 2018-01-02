#include "Text3D.h"
#include "Game.h"

bool Text3D::mInitialized = false;
std::map<char, Mesh*> Text3D::mCharacters = std::map<char, Mesh*>();

Text3D::Text3D() : mString("") {
  if (!mInitialized)
    initCharacters();
}

Text3D::~Text3D() {}

void Text3D::setString(const std::string& str) {
  mString = str;

  for (Model* model : mModels) Game::instance().getScene()->removeModel(model);
  mModels.clear();

  for (int i = 0; i < mString.size(); ++i) {
    char c = mString[i];
    Model* model = new Model();

    /* Caps => Minus */
    if (c >= 'A' && c <= 'Z')
      c += 32;

    model->init();
    model->setMesh(mCharacters[c]);
    model->setTexture(Game::instance().getResource().texture("palette.png"));
    model->move(TILE_SIZE * (float)i * RIGHT);

    Game::instance().getScene()->addModel(model);
  }
}

void Text3D::setPosition(const glm::vec3& position) {
  mPosition = position;

  for (int i = 0; i < mModels.size(); ++i)
    mModels[i]->setPosition(position + RIGHT * (float)i);
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