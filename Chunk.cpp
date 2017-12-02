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
  createFloor();
  setType(type);
  
	Model* cactus = new Model();
  cactus->init();
  cactus->setMesh(Game::instance().getResource().mesh("cactus.obj"));
  cactus->setTexture(Game::instance().getResource().texture("cactus.png"));
  cactus->setPositionInTiles(glm::vec3(0, 0, 0));
  mModels.push_back(cactus);

  Model* boat = new Model();
  boat->init();
  boat->setMesh(Game::instance().getResource().mesh("boat.obj"));
  boat->setTexture(Game::instance().getResource().texture("boat.png"));
  boat->setPositionInTiles(glm::vec3(1, 0, 0));
  mModels.push_back(boat);

  Model* thumbleweed = new Model();
  thumbleweed->init();
  thumbleweed->setMesh(Game::instance().getResource().mesh("thumbleweed.obj"));
  thumbleweed->setTexture(Game::instance().getResource().texture("thumbleweed.png"));
  thumbleweed->setPositionInTiles(glm::vec3(3, 0, 0));
  mModels.push_back(thumbleweed);
  setDepth(depth);
}

void Chunk::update(int deltaTime) {}

void Chunk::render() {
	for (Model* model : mModels)
		model->render();
}

void Chunk::setDepth(unsigned int depth) {
  mDepth = depth;

  glm::vec3 position;
  for (Model* model : mModels) {
    position = model->getPositionInTiles() + IN * (float)mDepth;
    model->setPositionInTiles(position);
  }
}

void Chunk::setType(Chunk::ChunkType type) {
  mType = type;
  initFloor();
}

Chunk::ChunkType Chunk::getType() const { return mType; }

void Chunk::createFloor() {
  for (int i = 0; i < TILES_PER_CHUNK; ++i) {
    Model* cube = new Model();
    cube->init();
    cube->setPositionInTiles(glm::vec3(i, -1, 0));
    mFloor.push_back(cube);
    mModels.push_back(cube);
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