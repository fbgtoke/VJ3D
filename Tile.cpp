#include "Tile.h"
#include "Game.h"

Tile::Tile()
  : mType(Tile::None) {}

Tile::Tile(Tile::Type type)
  : mType(type) {}

Tile::~Tile() {}

void Tile::init() {
  Model::init();

  setMesh(Game::instance().getResource().mesh("tile.obj"));
  setTexture(Game::instance().getResource().texture("tileset.png"));
  setShader(Game::instance().getResource().shader("tile"));
}

void Tile::render() {
  glm::vec2 texcoord = type2texturecoord(mType);
  mShaderProgram->setUniform2f("texoffset", texcoord.x, texcoord.y);

  Model::render();
}

void Tile::setType(Tile::Type type) { mType = type; }
Tile::Type Tile::getType() const { return mType; }

glm::vec2 Tile::type2texturecoord(Tile::Type type) {
  glm::vec2 texcoord(0.f);
  texcoord.x = (float)(type%4) * 16.f / 64.f;
  texcoord.y = (float)(type/4) * 16.f / 64.f;

  return texcoord;
}