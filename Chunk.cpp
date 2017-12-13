#include "Chunk.h"
#include "Game.h"

Chunk::Chunk() {}

Chunk::~Chunk() {
  for (auto it = mFloor.begin(); it != mFloor.end(); ++it)
    delete (*it);
	mFloor.clear();

  for (auto it = mObstacles.begin(); it != mObstacles.end(); ++it)
    delete (*it);
  mObstacles.clear();
}

void Chunk::init(ChunkType type, unsigned int depth) {
  setDepth(depth);

  createFloor();
  setType(type);
}

void Chunk::update(int deltaTime) {
  for (Obstacle* obstacle : mObstacles)
    obstacle->update(deltaTime);
}

void Chunk::render() {
	for (Model* model : mFloor) model->render();
  for (Obstacle* obstacle : mObstacles) obstacle->render();
}

void Chunk::setDepth(unsigned int depth) { mDepth = depth; }

unsigned int Chunk::getDepth() const { return mDepth; }

void Chunk::setType(Chunk::ChunkType type) {
  mType = type;
  initFloor();
}

Chunk::ChunkType Chunk::getType() const { return mType; }

void Chunk::addObstacle(Obstacle* obstacle) {
  glm::vec3 position = obstacle->getPositionInTiles();
  position.z = ((float)mDepth) * (-1.f);

  obstacle->setPositionInTiles(position);
  mObstacles.push_back(obstacle);
}

const Obstacle* Chunk::checkCollisions(const Player& player) const {
  for (Obstacle* obstacle : mObstacles)
    if (obstacle->collides(player))
      return obstacle;

  return nullptr;
}

bool Chunk::hasObstacleAtPosition(Obstacle::ObstacleType type, unsigned int position) const {
  for (Obstacle* obstacle : mObstacles)
    if (obstacle->getType() == type)
      if (obstacle->getPositionInTiles().x == position)
        return true;

  return false;
}

void Chunk::removeObstacle(const Obstacle* obstacle) {
  auto it = mObstacles.begin();

  while (it != mObstacles.end()) {
    if (obstacle == (*it)) {
      delete obstacle;
      mObstacles.erase(it++);
    } else {
      it++;
    }
  }
}

void Chunk::createFloor() {
  glm::vec3 position;
  position.y = -1.f;
  position.z = ((float)mDepth) * (-1.f);

  Mesh* mesh = Game::instance().getResource().mesh("cube.obj");

  for (int i = 0; i < TILES_PER_CHUNK; ++i) {
    Model* cube = new Model();
    cube->init();
    cube->setMesh(mesh);

    position.x = (float)i;
    cube->setPositionInTiles(position);

    mFloor.push_back(cube);
  }
}

void Chunk::initFloor() {
  std::string textureName;

  switch(mType) {
  case GRASS: textureName = "chunk_grass.png"; break;
  case ROAD:  textureName = "chunk_road.png";  break;
  case TRAIN: textureName = "chunk_rail.png";  break;
  case WATER: textureName = "chunk_water.png"; break;
  case GOAL:  textureName = "chunk_goal.png";  break;
  default:    textureName = "chunk_blank.png"; break;
  }

  Texture* texture = Game::instance().getResource().texture(textureName);

  for (Model* model : mFloor)
    model->setTexture(texture);
}