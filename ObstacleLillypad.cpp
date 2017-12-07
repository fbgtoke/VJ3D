#include "ObstacleLillypad.h"
#include "Game.h"

ObstacleLillypad::ObstacleLillypad()
  : Obstacle(Obstacle::LILLYPAD) {}

ObstacleLillypad::~ObstacleLillypad() {}

void ObstacleLillypad::init(unsigned int position) {
  Obstacle::init();

  setMesh(Game::instance().getResource().mesh("stone.obj"));
  setTexture(Game::instance().getResource().texture("palette.png"));

  setPositionInTiles(glm::vec3(position, 0, 0));
}