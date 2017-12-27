#include "Tilemap.h"
#include "Game.h"

Tilemap::Tilemap() {
  kBorderSize = Game::instance().getResource().Int("BorderSize");
}

Tilemap::~Tilemap() {}

void Tilemap::clear() {
  for (int i = 0; i < getHeight(); ++i) {
    for (int j = 0; j < getWidth(); ++j) {
      Tile* tile = mTiles[i][j];
      Game::instance().getScene()->removeModel(tile);
      delete tile;
    }
  }
  mTiles.clear();

  clearMargins();
}

void Tilemap::resize(const glm::ivec2& size) {
  clear();
  mTiles = TileArray(size.y, TileRow(size.x));

  Tile* tile;
  glm::vec3 position;

  for (int i = 0; i < size.y; ++i) {
    for (int j = 0; j < size.x; ++j) {
      tile = new Tile(Tile::None);
      position.x = j;
      position.y = -1.f;
      position.z = i * (-1.f);

      mTiles[i][j] = tile;
      mTiles[i][j]->init();
      mTiles[i][j]->setPositionInTiles(position);

      Game::instance().getScene()->addModel(tile, Scene::RenderFirst);
    }
  }

  initMargins();
}

unsigned int Tilemap::getHeight() const { return mTiles.size(); }
unsigned int Tilemap::getWidth() const { return mTiles[0].size(); }

void Tilemap::setTile(const glm::ivec2& position, Tile::Type type) {
  if (!outOfBounds(position)) {
    mTiles[position.y][position.x]->setType(type);

    for (int i = 0; i < kBorderSize; ++i) {
      mMarginLeft[position.y][i]->setType(type);
      mMarginRight[position.y][i]->setType(type);
    }
  }
}

Tile::Type Tilemap::getTile(const glm::ivec2& position) const {
  if (!outOfBounds(position))
    return mTiles[position.y][position.x]->getType();
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
      glm::ivec2 index(j, size.y - 1 - i);

      setTile(index, type);
    }
  }
}

void Tilemap::initMargins() {
  Tile* tile;
  glm::vec3 position;

  mMarginLeft  = TileArray(getHeight(), TileRow(kBorderSize));
  mMarginRight = TileArray(getHeight(), TileRow(kBorderSize));
  mMarginBottom = TileArray(kBorderSize, TileRow(getWidth() + kBorderSize * 2));

  for (int i = 0; i < getHeight(); ++i) {
    for (int j = 0; j < kBorderSize; ++j) {
      tile = new Tile(Tile::Grass);
      position.x = j - kBorderSize;
      position.y = -1.f;
      position.z = i * (-1.f);

      mMarginLeft[i][j] = tile;
      mMarginLeft[i][j]->init();
      mMarginLeft[i][j]->setPositionInTiles(position);
      mMarginLeft[i][j]->setDark(true);

      Game::instance().getScene()->addModel(tile, Scene::RenderFirst);

      tile = new Tile(Tile::Grass);
      position.x = j + getWidth();
      position.y = -1.f;
      position.z = i * (-1.f);

      mMarginRight[i][j] = tile;
      mMarginRight[i][j]->init();
      mMarginRight[i][j]->setPositionInTiles(position);
      mMarginRight[i][j]->setDark(true);

      Game::instance().getScene()->addModel(tile, Scene::RenderFirst);
    }
  }

  for (int i = 0; i < kBorderSize; ++i) {
    for (int j = 0; j < getWidth() + kBorderSize * 2; ++j) {
      tile = new Tile(Tile::Grass);
      position.x = j - kBorderSize;
      position.y = -1.f;
      position.z = i + 1;

      mMarginBottom[i][j] = tile;
      mMarginBottom[i][j]->init();
      mMarginBottom[i][j]->setPositionInTiles(position);
      mMarginBottom[i][j]->setDark(true);

      Game::instance().getScene()->addModel(tile, Scene::RenderFirst);
    }
  }
}

void Tilemap::clearMargins() {
  Tile* tile;

  /* Sides */
  for (int i = 0; i < mMarginLeft.size(); ++i) {
    for (int j = 0; j < mMarginLeft[0].size(); ++j) {
      mMarginLeft[i][j]->destroy();
      mMarginRight[i][j]->destroy();
    }
  }

  /* Bottom */
  for (int i = 0; i < mMarginBottom.size(); ++i) {
    for (int j = 0; j < mMarginBottom[0].size(); ++j) {
      mMarginBottom[i][j]->destroy();
    }
  }

  mMarginLeft.clear();
  mMarginRight.clear();
  mMarginBottom.clear();
}