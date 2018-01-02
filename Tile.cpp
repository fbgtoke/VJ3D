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

  ShaderProgram* shader = Game::instance().getScene()->getShader();

  glm::vec2 texcoord = type2texturecoord(mType);
  shader->setUniform2f("texoffset", texcoord);

  glm::vec3 ambientColor = Game::instance().getScene()->getAmbientColor();
  if (mDark) ambientColor *= 0.05f;
  shader->setUniform3f("ambientColor", ambientColor);
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