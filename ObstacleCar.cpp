#include "ObstacleCar.h"
#include "Game.h"

ObstacleCar::ObstacleCar()
  : Obstacle(Obstacle::CAR) {}

ObstacleCar::~ObstacleCar() {}

void ObstacleCar::init(float spd) {
  Obstacle::init();
  
  setMesh(Game::instance().getResource().mesh("carriage.obj"));
  setTexture(Game::instance().getResource().texture("palette.png"));

  setVelocity(RIGHT * spd);
}

void ObstacleCar::setVelocity(const glm::vec3& velocity) {
  Obstacle::setVelocity(velocity);

  float flip = 1.f;
  if (getVelocity().x >= 0)
    flip = -1.f;

  mScale.x *= flip;
}