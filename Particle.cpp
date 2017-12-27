#include "Particle.h"
#include "Game.h"

void Particle::init(int aliveTime) {
  Model::init();

  kGravity = Game::instance().getResource().Float("ParticleGravity");
  kFriction = Game::instance().getResource().Float("ParticleFriction");

  mAliveTime = aliveTime;
}

void Particle::update(int deltaTime) {
  Model::update(deltaTime);

  mAliveTime -= deltaTime;
  if (mAliveTime < 0)
    destroy();

  mVelocity.y += kGravity;
}

void Particle::onCollision(Model* model) {
  Model::onCollision(model);

  Tile* tile = dynamic_cast<Tile*> (model);
  if (tile != nullptr) {
    mVelocity.y *= kFriction * (-1.f);
    mVelocity.x *= kFriction;
    mVelocity.z *= kFriction;
  }
}

bool Particle::checkCollisions() const { return true; }