#ifndef _TILEMAP_INCLUDE
#define _TILEMAP_INCLUDE

#include "utils.h"
#include "Tile.h"
#include "CsvReader.h"

class Tilemap {
public:
  Tilemap();
  ~Tilemap();

  void clear();
  void resize(const glm::ivec2& size);
  unsigned int getHeight() const;
  unsigned int getWidth() const;

  void setTile(const glm::ivec2& position, Tile::Type type);
  Tile::Type getTile(const glm::ivec2& position) const;

  bool outOfBounds(const glm::ivec2& position) const;

  void loadFromFile(const std::string& filename);

private:
  int kBorderSize;

  typedef std::vector<Tile*> TileRow;
  typedef std::vector<TileRow> TileArray;
  TileArray mTiles;

  void initMargins();
  void clearMargins();
  TileArray mMarginLeft;
  TileArray mMarginRight;
  TileArray mMarginBottom;
};

#endif // _TILEMAP_INCLUDE