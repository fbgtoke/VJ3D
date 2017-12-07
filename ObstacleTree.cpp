#include "ObstacleTree.h"
#include "Game.h"

ObstacleTree::ObstacleTree()
  : Obstacle(Obstacle::TREE) {}

ObstacleTree::~ObstacleTree() {}

void ObstacleTree::init(unsigned int position) {
  Obstacle::init();
  setMesh(Game::instance().getResource().mesh("cactus.obj"));
  setTexture(Game::instance().getResource().texture("cactus.png"));

  setPositionInTiles(glm::vec3(position, 0, 0));
}