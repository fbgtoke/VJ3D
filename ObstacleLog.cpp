#include "ObstacleLog.h"
#include "Game.h"

ObstacleLog::ObstacleLog()
  : Obstacle(Obstacle::LOG) {}

ObstacleLog::~ObstacleLog() {}

void ObstacleLog::init(float spd) {
  Obstacle::init();

  setMesh(Game::instance().getResource().mesh("boat.obj"));
  setTexture(Game::instance().getResource().texture("palette.png"));

  setVelocity(RIGHT * spd);
}

void ObstacleLog::setVelocity(const glm::vec3& velocity) {
  Obstacle::setVelocity(velocity);

  float flip = 1.f;
  if (getVelocity().x >= 0)
    flip = -1.f;

  mScale.x *= flip;
}