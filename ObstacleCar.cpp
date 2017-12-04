#include "ObstacleCar.h"
#include "Game.h"

ObstacleCar::ObstacleCar()
  : Obstacle(Obstacle::CAR) {}

ObstacleCar::~ObstacleCar() {}

void ObstacleCar::init(float spd) {
  Obstacle::init();
  
  setMesh(Game::instance().getResource().mesh("carriage.obj"));
  setTexture(Game::instance().getResource().texture("carriage.png"));
  setRotation(glm::vec3(0.f, (float)M_PI/2.f * -1.f, 0.f));

  setVelocity(RIGHT * spd);
}