#include "Particle.h"

const float Particle::kGravity = -0.025f;
const float Particle::kFriction = 0.8f;

void Particle::init(int aliveTime) {
  Model::init();

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