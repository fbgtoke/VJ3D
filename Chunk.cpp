#include "Chunk.h"
#include "Game.h"

Chunk::Chunk() {}

Chunk::~Chunk() {
	for (Model* model : mModels)
		if (model != nullptr)
			delete model;

	mModels.clear();
}

void Chunk::init() {
  mDepth = 0;

  for (int i = 0; i < TILES_PER_CHUNK; ++i) {
    Model* cube = new Model();
    cube->init();
    cube->setMesh(Game::instance().getResource().mesh("cube.obj"));
    cube->setTexture(Game::instance().getResource().texture("cactus.png"));
    cube->setPositionInTiles(glm::vec3(i, -1, 0));
    mModels.push_back(cube);
  }

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