#include "ObstacleBonus.h"
#include "Game.h"

ObstacleBonus::ObstacleBonus()
  : Obstacle(Obstacle::BONUS) {}

ObstacleBonus::~ObstacleBonus() {}

void ObstacleBonus::init(unsigned int position) {
  Obstacle::init();

  setMesh(Game::instance().getResource().mesh("bonus1.obj"));
  setTexture(Game::instance().getResource().texture("palette.png"));

  setPositionInTiles(glm::vec3(position, 0, 0));
}