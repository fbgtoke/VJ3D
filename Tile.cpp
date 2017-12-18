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
  setShader(Game::instance().getResource().shader("tile"));
}

void Tile::render() {
  glm::vec2 texcoord = type2texturecoord(mType);
  mShaderProgram->setUniform2f("texoffset", texcoord.x, texcoord.y);

  if (mDark)
    mShaderProgram->setUniform1f("ambient", -0.5f);
  else
    mShaderProgram->setUniform1f("ambient", 0.7f);


  Model::render();
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

bool Tile::collides(const Model* m) const {
  if (m == nullptr) return false;
  
  glm::vec3 mincoords1, mincoords2, maxcoords1, maxcoords2;
  mincoords1 = getPosition() - getSize() * 0.125f;
  mincoords2 = m->getPosition() - m->getSize() * 0.125f;
  maxcoords1 = mincoords1 + getSize() * 0.125f;
  maxcoords2 = mincoords2 + m->getSize() * 0.125f;

  return
    maxcoords1.x > mincoords2.x && mincoords1.x < maxcoords2.x &&
    maxcoords1.y > mincoords2.y && mincoords1.y < maxcoords2.y &&
    maxcoords1.z > mincoords2.z && mincoords1.z < maxcoords2.z;
}