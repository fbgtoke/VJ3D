#include "ObstacleCar.h"
#include "Game.h"

ObstacleCar::ObstacleCar()
  : Obstacle(Obstacle::CAR) {}

ObstacleCar::~ObstacleCar() {}

void ObstacleCar::init(float spd) {
  Obstacle::init();

  setTexture(Game::instance().getResource().texture("palette.png"));

  setVelocity(RIGHT * spd);

  mAnimation.setNumberOfAnimations(2);

  mAnimation.addFrame(0, Game::instance().getResource().mesh("carriage1.obj"));
  mAnimation.addFrame(0, Game::instance().getResource().mesh("animationcarriage1.obj"));
  mAnimation.addFrame(0, Game::instance().getResource().mesh("carriage2.obj"));
  mAnimation.addFrame(0, Game::instance().getResource().mesh("animationcarriage2.obj"));

  mAnimation.addFrame(1, Game::instance().getResource().mesh("horse.obj"));
  mAnimation.addFrame(1, Game::instance().getResource().mesh("animationhorse1.obj"));
  mAnimation.addFrame(1, Game::instance().getResource().mesh("horse.obj"));
  mAnimation.addFrame(1, Game::instance().getResource().mesh("animationhorse2.obj"));

  unsigned int frame = rand()%2;

  mAnimation.changeAnimation(frame);
  mAnimation.setTimePerFrame(200);

  setMesh(mAnimation.getCurrentFrame());
  if (frame == 1)
    setRotation(UP * (float)M_PI/2.f);
}

void ObstacleCar::update(int deltaTime) {
  Obstacle::update(deltaTime);
  mAnimation.update(deltaTime);

  setMesh(mAnimation.getCurrentFrame());
}

void ObstacleCar::setVelocity(const glm::vec3& velocity) {
  Obstacle::setVelocity(velocity);

  float flip = 1.f;
  if (getVelocity().x >= 0)
    flip = -1.f;

  mScale.x *= flip;
}