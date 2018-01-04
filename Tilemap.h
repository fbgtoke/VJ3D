#ifndef _TILEMAP_INCLUDE
#define _TILEMAP_INCLUDE

#include "utils.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Tile.h"
#include "CsvReader.h"

class Tilemap {
public:
  Tilemap();
  ~Tilemap();

  void init();
  void render();

  void clear();
  void resize(const glm::ivec2& size);
  int getHeight() const;
  int getWidth() const;

  void setTile(const glm::ivec2& position, Tile::Type type);
  Tile::Type getTile(const glm::ivec2& position) const;

  bool outOfBounds(const glm::ivec2& position) const;

  void loadFromFile(const std::string& filename);

private:
  int kBorderSize;
  int mWidth;
  int mHeight;
  static const std::vector<glm::vec3> kQuad;
  std::vector<float> mVertices;
  std::vector<float> mTexcoords;
  std::vector<float> mNormals;
  std::vector<std::vector<Tile::Type>> mTiles;
  
  void initVertices();
  void initTexcoords();
  void initNormals();

  void initVAO();
  void updateVAO();
  void freeVAO();

  void initMargins();

  ShaderProgram* mShader;
  GLuint mVAO;
  GLuint mVBO_vertices, mVBO_normals, mVBO_texcoords;
  GLuint mLoc_vertices, mLoc_normals, mLoc_texcoords;
};

#endif // _TILEMAP_INCLUDE