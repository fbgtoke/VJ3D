#include "Chunk.h"
#include "Game.h"

Chunk::Chunk() {}

Chunk::~Chunk() {
	for (Model* model : mFloor)
		if (model != nullptr)
			delete model;

	mFloor.clear();

  for (Obstacle* obstacle : mObstacles)
    if (obstacle != nullptr)
      delete obstacle;

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

const Model* Chunk::checkCollisions(const Player& player) const {
  for (Obstacle* obstacle : mObstacles)
    if (obstacle->collides(player))
      return obstacle;

  return nullptr;
}

void Chunk::createFloor() {
  glm::vec3 position;
  position.y = -1.f;
  position.z = ((float)mDepth) * (-1.f);

  std::shared_ptr<Mesh> mesh = Game::instance().getResource().mesh("cube.obj");

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
  std::shared_ptr<Texture> texture;

  switch(mType) {
  case GRASS:
    texture = Game::instance().getResource().texture("chunk_grass.png");
    break;
  case ROAD:
    texture = Game::instance().getResource().texture("chunk_road.png");
    break;
  case TRAIN:
    texture = Game::instance().getResource().texture("chunk_rail.png");
    break;
  case GOAL:
    texture = Game::instance().getResource().texture("chunk_goal.png");
    break;
  default:
    texture = Game::instance().getResource().texture("chunk_blank.png");
    break;
  }

  for (Model* model : mFloor)
    model->setTexture(texture);
}