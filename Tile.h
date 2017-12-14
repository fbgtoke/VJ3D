#ifndef _TILE_INCLUDE
#define _TILE_INCLUDE

#include "utils.h"
#include "Model.h"

class Tile : public Model {
public:
  enum Type {
    None = 0,
    Grass,
    Road,
    Train,
    Water,
    Goal
  };

  Tile();
  Tile(Tile::Type type);
  ~Tile() override;

  void init() override;
  void render() override;

  void setType(Tile::Type type);
  Tile::Type getType() const;

  static glm::vec2 type2texturecoord(Tile::Type type);

private:
  Tile::Type mType;
};

#endif // _TILE_INCLUDE