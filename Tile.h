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
  void beforeRender() override;

  void setType(Tile::Type type);
  Tile::Type getType() const;

  void setDark(bool dark);

  static glm::vec2 type2texturecoord(Tile::Type type);

  void getBoundingBox(glm::vec3& mincoords, glm::vec3& maxcoords) const override;

private:
  Tile::Type mType;
  bool mDark;
};

#endif // _TILE_INCLUDE