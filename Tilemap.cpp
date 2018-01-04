#include "Tilemap.h"
#include "Game.h"

const std::vector<glm::vec3> Tilemap::kQuad = {
  glm::vec3(0.f, 0.f, 1.f),
  glm::vec3(0.f, 0.f, 0.f),
  glm::vec3(1.f, 0.f, 0.f),
  glm::vec3(0.f, 0.f, 1.f),
  glm::vec3(1.f, 0.f, 0.f),
  glm::vec3(1.f, 0.f, 1.f)
};

Tilemap::Tilemap() : mShader(nullptr) {}

Tilemap::~Tilemap() {
  freeVAO();
}

void Tilemap::init() {
  mWidth  = 0;
  mHeight = 0;

  mShader = Game::instance().getResource().shader("level");
  initVAO();

  kBorderSize = Game::instance().getResource().Int("BorderSize");
}

void Tilemap::render() {
  if (mShader == nullptr) return;

  mShader = Game::instance().getScene()->getShader();

  freeVAO();
  initVAO();

  glm::mat4 TG = glm::mat4(1.f);
  TG = glm::translate(TG, glm::vec3(-kBorderSize * TILE_SIZE, 0.f, kBorderSize * TILE_SIZE));
  mShader->setUniformMatrix4f("TG", TG);

  mShader->setUniform2f("texoffset", 0.f, 0.f);

  Game::instance().getResource().texture("tileset.png")->use();

  glBindVertexArray(mVAO);
  glDrawArrays(GL_TRIANGLES, 0, mVertices.size() / 3);
  glBindVertexArray(0);
}

void Tilemap::clear() {
  mWidth  = 0;
  mHeight = 0;

  mVertices.clear();
  mNormals.clear();
  mTexcoords.clear();
  updateVAO();

  mTiles.clear();
}

void Tilemap::resize(const glm::ivec2& size) {
  mWidth  = size.x;
  mHeight = size.y;

  initVertices();
  initNormals();
  initTexcoords();
  updateVAO();

  mTiles = std::vector<std::vector<Tile::Type>> (
    mHeight,
    std::vector<Tile::Type>(mWidth, Tile::None)
  );
}

void Tilemap::initVertices() {
  mVertices.clear();

  glm::vec3 stride = glm::vec3(0.f);
  glm::vec3 vertex;
  for (int i = 0; i < mHeight + kBorderSize * 2; ++i) {
    for (int j = 0; j < mWidth + kBorderSize * 2; ++j) {
      stride.x =  j;
      stride.z = -i;

      for (int k = 0; k < kQuad.size(); ++k) {
        vertex = (stride + kQuad[k]) * TILE_SIZE;

        mVertices.push_back(vertex.x);
        mVertices.push_back(vertex.y);
        mVertices.push_back(vertex.z);
      }
    }
  }
}

void Tilemap::initNormals() {
  mNormals.clear();

  for (int i = 0; i < mHeight + kBorderSize * 2; ++i) {
    for (int j = 0; j < mWidth + kBorderSize * 2; ++j) {
      for (int k = 0; k < kQuad.size(); ++k) {
        mNormals.push_back(0.f);
        mNormals.push_back(1.f);
        mNormals.push_back(0.f);
      }
    }
  }
}

void Tilemap::initTexcoords() {
  mTexcoords.clear();

  for (int i = 0; i < mHeight + kBorderSize * 2; ++i) {
    for (int j = 0; j < mWidth + kBorderSize * 2; ++j) {
      for (int k = 0; k < kQuad.size(); ++k) {
        mTexcoords.push_back(0.f);
        mTexcoords.push_back(0.f);
      }
    }
  }
}

void Tilemap::initVAO() {
  glGenVertexArrays(1, &mVAO);
  glBindVertexArray(mVAO);

  glGenBuffers(1, &mVBO_vertices);
  glBindBuffer(GL_ARRAY_BUFFER, mVBO_vertices);
  glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(float), &mVertices[0], GL_STATIC_DRAW);
  mLoc_vertices = mShader->bindVertexAttribute("vertex", 3, 0, 0);
  glEnableVertexAttribArray(mLoc_vertices);

  glGenBuffers(1, &mVBO_normals);
  glBindBuffer(GL_ARRAY_BUFFER, mVBO_normals);
  glBufferData(GL_ARRAY_BUFFER, mNormals.size() * sizeof(float), &mNormals[0], GL_STATIC_DRAW);
  mLoc_vertices = mShader->bindVertexAttribute("normal", 3, 0, 0);
  glEnableVertexAttribArray(mLoc_normals);

  glGenBuffers(1, &mVBO_texcoords);
  glBindBuffer(GL_ARRAY_BUFFER, mVBO_texcoords);
  glBufferData(GL_ARRAY_BUFFER, mTexcoords.size() * sizeof(float), &mTexcoords[0], GL_STATIC_DRAW);
  mLoc_texcoords = mShader->bindVertexAttribute("texcoord", 2, 0, 0);
  glEnableVertexAttribArray(mLoc_texcoords);

  glBindVertexArray(0);
}

void Tilemap::freeVAO() {
  if (mShader != nullptr) {
    glDeleteBuffers(1, &mVBO_vertices);
    glDeleteBuffers(1, &mVBO_normals);
    glDeleteBuffers(1, &mVBO_texcoords);

    glDeleteVertexArrays(1, &mVAO);
  }
}

void Tilemap::updateVAO() {
  glBindVertexArray(mVAO);

  glBindBuffer(GL_ARRAY_BUFFER, mVBO_vertices);
  glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(float), &mVertices[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, mVBO_normals);
  glBufferData(GL_ARRAY_BUFFER, mNormals.size() * sizeof(float), &mNormals[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, mVBO_texcoords);
  glBufferData(GL_ARRAY_BUFFER, mTexcoords.size() * sizeof(float), &mTexcoords[0], GL_STATIC_DRAW);

  glBindVertexArray(0);
}

int Tilemap::getHeight() const { return mHeight; }
int Tilemap::getWidth() const { return mWidth; }

void Tilemap::setTile(const glm::ivec2& position, Tile::Type type) {
  if (!outOfBounds(position));
    mTiles[position.y][position.x] = type;

  int index =
    ((position.y + kBorderSize) * (mWidth + kBorderSize * 2) + (position.x + kBorderSize)) * 12;

  //std::cout << "Position " << position.x << " " << position.y << std::endl;
  //std::cout << "Index " << index << std::endl;

  float uvLeft  = (type%4) * 0.25f + 0.00f;
  float uvRight = (type%4) * 0.25f + 0.25f;

  float uvTop = (type/4) * 0.25f + 0.00f;
  float uvBot = (type/4) * 0.25f + 0.25f;

  // Bottom Left
  mTexcoords[index +  0] = uvLeft;
  mTexcoords[index +  1] = uvBot;

  // Top Left
  mTexcoords[index +  2] = uvLeft;
  mTexcoords[index +  3] = uvTop;

  // Top Right
  mTexcoords[index +  4] = uvRight;
  mTexcoords[index +  5] = uvTop;

  // Bottom Left
  mTexcoords[index +  6] = uvLeft;
  mTexcoords[index +  7] = uvBot;

  // Top Right
  mTexcoords[index +  8] = uvRight;
  mTexcoords[index +  9] = uvTop;

  // Bottom Right
  mTexcoords[index + 10] = uvRight;
  mTexcoords[index + 11] = uvBot;

  updateVAO();

  //if (position.x == 0) {
  //  for (int i = 1; i <= kBorderSize; ++i) {
  //    std::cout << "Going to " << -i << " " << position.y << std::endl;
  //    setTile(glm::ivec2(-i, position.y), type);
  //  }
  //}
}

Tile::Type Tilemap::getTile(const glm::ivec2& position) const {
  if (outOfBounds(position))
    return Tile::None;
  return mTiles[position.y][position.x];
}

bool Tilemap::outOfBounds(const glm::ivec2& position) const {
  return
    position.y < 0 || position.y >= mHeight ||
    position.x < 0 || position.x >= mWidth;
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