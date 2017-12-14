#include "Tilemap.h"

Tilemap::Tilemap() {}

Tilemap::~Tilemap() {}

void Tilemap::render() {
  for (int i = 0; i < mTiles.size(); ++i) {
    for (int j = 0; j < mTiles[0].size(); ++j) {
      mTiles[i][j].render();
    }
  }
}

void Tilemap::resize(const glm::ivec2& size) {
  mTiles.clear();
  mTiles = TileArray(size.y, TileRow(size.x, Tile::None));

  for (int i = 0; i < size.y; ++i) {
    for (int j = 0; j < size.x; ++j) {
      mTiles[i][j].init();
      mTiles[i][j].setPositionInTiles(glm::vec3(j, -1, i * (-1)));
    }
  }
}

void Tilemap::setTile(const glm::ivec2& position, Tile::Type type) {
  if (!outOfBounds(position))
    mTiles[position.y][position.x] = type;
}

Tile::Type Tilemap::getTile(const glm::ivec2& position) const {
  if (!outOfBounds(position))
    return mTiles[position.y][position.x].getType();
  return Tile::None;
}

bool Tilemap::outOfBounds(const glm::ivec2& position) const {
  return
    position.y < 0 || position.y >= mTiles.size() ||
    position.x < 0 || position.x >= mTiles[0].size();
}

void Tilemap::loadFromFile(const std::string& filename) {
  CsvReader reader;
  reader.loadFromFile(filename);

  glm::ivec2 size(reader.getNumberOfColumns(), reader.getNumberOfRows());
  resize(size);

  for (int i = 0; i < size.y; ++i) {
    for (int j = 0; j < size.x; ++j) {
      int value = reader.getCell(i, j);
      Tile::Type type = static_cast<Tile::Type>(value);
      mTiles[size.y - 1 - i][j].setType(type); 
    }
  }
}