#include "Particle.h"

const float Particle::kGravity = -0.05f;
const float Particle::kFriction = 0.99f;
const float Particle::kMaxVelocity = .5f;

void Particle::init(int aliveTime) {
  Model::init();

  mAliveTime = aliveTime;
}

void Particle::update(int deltaTime) {
  Model::update(deltaTime);

  mAliveTime -= deltaTime;

  if (mPosition.y > mScale.y)
    mVelocity.y += kGravity;

  if (mVelocity.y > kMaxVelocity)
    mVelocity.y = kMaxVelocity;
  else if (mVelocity.y < -kMaxVelocity)
    mVelocity.y = -kMaxVelocity;

  if (mPosition.y < mScale.y) {
    mVelocity.y *= kFriction * (-1.f);
    mVelocity.x *= kFriction;
    mVelocity.z *= kFriction;
  }

  if (abs(mVelocity.y) < 0.025f && mPosition.y < mScale.y) {
    mVelocity.y = mScale.y;
    mPosition.y = 0.f;
  }
}

bool Particle::isAlive() const { return mAliveTime > 0; }