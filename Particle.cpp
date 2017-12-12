#include "Particle.h"

void Particle::init(int aliveTime) {
  Model::init();

  mAliveTime = aliveTime;
}

void Particle::update(int deltaTime) {
  Model::update(deltaTime);

  mAliveTime -= deltaTime;
}

bool Particle::isAlive() const { return mAliveTime > 0; }