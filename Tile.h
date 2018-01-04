#ifndef _TILE_INCLUDE
#define _TILE_INCLUDE

#include "utils.h"

class Tile {
public:
  enum Type {
    None = 0,
    Grass,
    Road,
    Train,
    Water,
    Goal,
    Padding1,
    Padding2,
    NoneDark,
    GrassDark,
    RoadDark,
    TrainDark,
    WaterDark,
    GoalDark 
  };

  static Tile::Type toDark(Tile::Type type);
  static glm::vec2 type2texturecoord(Tile::Type type);
};

#endif // _TILE_INCLUDE