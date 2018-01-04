#include "Tile.h"

Tile::Type Tile::toDark(Tile::Type type) {
  unsigned int index = static_cast<unsigned int>(type);
  return static_cast<Tile::Type>(index + 8);
}

glm::vec2 Tile::type2texturecoord(Tile::Type type) {
  glm::vec2 texcoord(0.f);
  texcoord.x = (float)(type%4) * 16.f / 64.f;
  texcoord.y = (float)(type/4) * 16.f / 64.f;

  return texcoord;
}