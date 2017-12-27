#include "ObstacleBonus.h"
#include "Game.h"

ObstacleBonus::ObstacleBonus()
  : Obstacle(Obstacle::Bonus) {}

void ObstacleBonus::init() {
  Obstacle::init();

  kRotationSpeed = Game::instance().getResource().Float("RotationSpeed");
  kBobSpeed = Game::instance().getResource().Float("BobSpeed");

  mAnimation.setNumberOfAnimations(1);
  mAnimation.addFrame(0, Game::instance().getResource().mesh("bonus1.obj"));
  mAnimation.setTimePerFrame(0);
  mAnimation.changeAnimation(0);
  setRotationSpeed(glm::vec3(0.f, kRotationSpeed, 0.f));

  mCurrentCycle = 0;
}

void ObstacleBonus::update(int deltaTime) {
  Obstacle::update(deltaTime);

  mCurrentCycle += deltaTime;

  mPosition.y += 0.5f * sin(kBobSpeed * (float)mCurrentCycle);
}