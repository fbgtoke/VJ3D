#include "ObstacleTree.h"
#include "Game.h"

ObstacleTree::ObstacleTree()
  : Obstacle(Obstacle::TREE) {}

ObstacleTree::~ObstacleTree() {}

void ObstacleTree::init(unsigned int position) {
  Obstacle::init();

  if (rand()%2)
    setMesh(Game::instance().getResource().mesh("cactus.obj"));
  else
    setMesh(Game::instance().getResource().mesh("stump.obj"));

  setTexture(Game::instance().getResource().texture("palette.png"));

  setPositionInTiles(glm::vec3(position, 0, 0));
}