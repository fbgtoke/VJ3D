#include "Chunk.h"
#include "Game.h"

Chunk::Chunk() {}

Chunk::~Chunk() {
	for (Model* model : mModels)
		if (model != nullptr)
			delete model;

	mModels.clear();
}

void Chunk::init(ChunkType type, unsigned int depth) {
  setDepth(depth);

  createFloor();
  setType(type);
}

void Chunk::update(int deltaTime) {}

void Chunk::render() {
	for (Model* model : mModels)
		model->render();
}

void Chunk::setDepth(unsigned int depth) { mDepth = depth; }

unsigned int Chunk::getDepth() const { return mDepth; }

void Chunk::setType(Chunk::ChunkType type) {
  mType = type;
  initFloor();
}

Chunk::ChunkType Chunk::getType() const { return mType; }

void Chunk::addModel(Model* model) {
  glm::vec3 position = model->getPositionInTiles();
  position.z = (float)mDepth * (-1.f);

  model->setPositionInTiles(position);
  mModels.push_back(model);
}

void Chunk::createFloor() {
  for (int i = 0; i < TILES_PER_CHUNK; ++i) {
    Model* cube = new Model();
    cube->init();
    cube->setPositionInTiles(glm::vec3(i, -1, 0));

    addModel(cube);
    mFloor.push_back(cube);
  }
}

void Chunk::initFloor() {
  std::shared_ptr<Mesh> mesh;
  std::shared_ptr<Texture> texture;

  switch(mType) {
  case GRASS:
    mesh = Game::instance().getResource().mesh("cube.obj");
    texture = Game::instance().getResource().texture("cactus.png");
    break;
  case ROAD:
    mesh = Game::instance().getResource().mesh("goal.obj");
    texture = Game::instance().getResource().texture("road.png");
    break;
  case TRAIN:
    mesh = Game::instance().getResource().mesh("goal.obj");
    texture = Game::instance().getResource().texture("rail.png");
    break;
  case GOAL:
    mesh = Game::instance().getResource().mesh("goal.obj");
    texture = Game::instance().getResource().texture("goal.png");
    break;
  default:
    mesh = Game::instance().getResource().mesh("cube.obj");
    texture = Game::instance().getResource().texture("cactus.png");
    break;
  }

  for (Model* model : mFloor) {
    model->setMesh(mesh);
    model->setTexture(texture);
  }
}