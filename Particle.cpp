#include "Particle.h"
#include "Game.h"

void Particle::init() {
  Model::init();

  kGravity = Game::instance().getResource().Float("ParticleGravity");
  kFriction = Game::instance().getResource().Float("ParticleFriction");

  int aliveTime = Game::instance().getResource().Int("minAliveTime");
  int randomAddedTime = Game::instance().getResource().Int("randomAddedTime");
  mAliveTime = aliveTime + randomInt(0, randomAddedTime);

  setMesh(Game::instance().getResource().mesh("cube.obj"));

  float size = Game::instance().getResource().Float("particleSize");
  setScale(glm::vec3(size));

  glm::vec3 direction;
  direction.x = randomFloat(-.2f, .2f);
  direction.y = randomFloat( 0.f, 1.5f);
  direction.z = randomFloat(-.2f, .2f);
  setVelocity(glm::normalize(direction) * 0.25f);
}

void Particle::update(int deltaTime) {
  Model::update(deltaTime);

  mAliveTime -= deltaTime;
  if (mAliveTime < 0)
    destroy();

  mVelocity.y += kGravity;

  if (mPosition.y < getSize().y * 2.f - TILE_SIZE) {
    mVelocity.y *= kFriction * (-1.f);
    mVelocity.x *= kFriction;
    mVelocity.z *= kFriction;
  }
}

void Particle::setAliveTime(int aliveTime) { mAliveTime = aliveTime; }
void Particle::setColor(const glm::vec3& color) { mColor = color; }

void Particle::generateExplosion(const glm::vec3& origin) {
  int nParticles = Game::instance().getResource().Int("nParticles");
  for (int i = 0; i < nParticles; ++i) {
    Particle* particle = new Particle();
    particle->init();
    particle->setPosition(origin);

    int texture = randomInt(0, 100);
    if (texture < 30)
      particle->setTexture(Game::instance().getResource().texture("red.png"));
    else if (texture < 60)
      particle->setTexture(Game::instance().getResource().texture("blue.png"));
    else
      particle->setTexture(Game::instance().getResource().texture("brown.png"));

    Game::instance().getScene()->addParticle(particle);
  }
}