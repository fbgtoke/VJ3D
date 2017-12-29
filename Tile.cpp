#include "Tile.h"
#include "Game.h"

Tile::Tile()
  : mType(Tile::None), mDark(false) {}

Tile::Tile(Tile::Type type)
  : mType(type), mDark(false) {}

Tile::~Tile() {}

void Tile::init() {
  Model::init();

  setMesh(Game::instance().getResource().mesh("tile.obj"));
  setTexture(Game::instance().getResource().texture("tileset.png"));
}

void Tile::beforeRender() {
  Model::beforeRender();

  glm::vec2 texcoord = type2texturecoord(mType);
  mShaderProgram->setUniform2f("texoffset", texcoord.x, texcoord.y);

  mShaderProgram->setUniform3f("lightDir", 0.f, 0.f, 0.f);

  float ambientLight = Game::instance().getScene()->getAmbientLight();
  if (mDark) ambientLight = Game::instance().getResource().Float("minAmbientLight");
  mShaderProgram->setUniform1f("ambientColor", ambientLight);

  float diffColor = Game::instance().getResource().Float("diffuseComponent");
  if (mDark) diffColor *= 0.1f;
  mShaderProgram->setUniform1f("diffColor", diffColor);
}

void Tile::setType(Tile::Type type) { mType = type; }
Tile::Type Tile::getType() const { return mType; }

void Tile::setDark(bool dark) { mDark = dark; }

glm::vec2 Tile::type2texturecoord(Tile::Type type) {
  glm::vec2 texcoord(0.f);
  texcoord.x = (float)(type%4) * 16.f / 64.f;
  texcoord.y = (float)(type/4) * 16.f / 64.f;

  return texcoord;
}

void Tile::getBoundingBox(glm::vec3& mincoords, glm::vec3& maxcoords) const {
  mincoords = getPosition() - getSize() * 0.125f;
  maxcoords = getPosition() + getSize() * 0.125f;
}