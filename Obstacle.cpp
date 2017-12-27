#include "Obstacle.h"
#include "ObstacleSpawner.h"
#include "Game.h"

Obstacle::Obstacle(Obstacle::Type type)
  : mType(type), mShadow(nullptr) {}

Obstacle::~Obstacle() {
  if (mShadow != nullptr)
    mShadow->unbind();
}

Obstacle::Obstacle::Type Obstacle::getType() const { return mType; }

void Obstacle::init() {
  ModelAnimated::init();

  switch (mType) {
  case Obstacle::Cactus:
    mAnimation.setNumberOfAnimations(1);
    mAnimation.addFrame(0, Game::instance().getResource().mesh("cactus.obj"));
    mAnimation.setTimePerFrame(0);
    mAnimation.changeAnimation(0);
    break;
  case Obstacle::Stump:
    mAnimation.setNumberOfAnimations(1);
    mAnimation.addFrame(0, Game::instance().getResource().mesh("stump.obj"));
    mAnimation.setTimePerFrame(0);
    mAnimation.changeAnimation(0);
    break;
  case Obstacle::Stone:
    mAnimation.setNumberOfAnimations(1);
    mAnimation.addFrame(0, Game::instance().getResource().mesh("stone.obj"));
    mAnimation.setTimePerFrame(0);
    mAnimation.changeAnimation(0);
    break;
  case Obstacle::Carriage:
    mAnimation.setNumberOfAnimations(1);
    mAnimation.addFrame(0, Game::instance().getResource().mesh("carriage1.obj"));
    mAnimation.addFrame(0, Game::instance().getResource().mesh("animationcarriage1.obj"));
    mAnimation.addFrame(0, Game::instance().getResource().mesh("carriage2.obj"));
    mAnimation.addFrame(0, Game::instance().getResource().mesh("animationcarriage2.obj"));
    mAnimation.setTimePerFrame(100);
    mAnimation.changeAnimation(0);
    break;
  case Obstacle::Horse:
    mAnimation.setNumberOfAnimations(1);
    mAnimation.addFrame(0, Game::instance().getResource().mesh("horse.obj"));
    mAnimation.addFrame(0, Game::instance().getResource().mesh("animationhorse1.obj"));
    mAnimation.addFrame(0, Game::instance().getResource().mesh("horse.obj"));
    mAnimation.addFrame(0, Game::instance().getResource().mesh("animationhorse2.obj"));
    mAnimation.setTimePerFrame(100);
    mAnimation.changeAnimation(0);
    setRotation(glm::vec3(0.f, (float)M_PI/2.f, 0.f));
    break;
  case Obstacle::Boat:
    mAnimation.setNumberOfAnimations(1);
    mAnimation.addFrame(0, Game::instance().getResource().mesh("boat.obj"));
    mAnimation.setTimePerFrame(0);
    mAnimation.changeAnimation(0);
    break;
  default:
    mAnimation.setNumberOfAnimations(1);
    mAnimation.addFrame(0, Game::instance().getResource().mesh("cube.obj"));
    mAnimation.setTimePerFrame(0);
    mAnimation.changeAnimation(0);
    break;
  }

  setMesh(mAnimation.getCurrentFrame());
  setTexture(Game::instance().getResource().texture("palette.png"));

  if (mType != Obstacle::Boat && mType != Obstacle::Stone && mType != Obstacle::Spawner) {
    mShadow = new Shadow(this);
    mShadow->init();
    Game::instance().getScene()->addModel(mShadow);
  }
}

void Obstacle::update(int deltaTime) {
  ModelAnimated::update(deltaTime);

  if (mVelocity.x >= 0.f)
    mScale.x = -1.f;
}

void Obstacle::getBoundingBox(glm::vec3& mincoords, glm::vec3& maxcoords) const {
  //if (mType != Obstacle::Boat) {
    ModelAnimated::getBoundingBox(mincoords, maxcoords);
  /*} else {
    mincoords.x = getPosition().x - getSize().x * 0.75f;
    mincoords.y = getPosition().y - getSize().y * 0.75f;
    mincoords.z = getPosition().z - getSize().z * 0.75f;

    maxcoords.x = getPosition().x + getSize().x * 0.75f;
    maxcoords.y = getPosition().y;
    maxcoords.z = getPosition().z + getSize().z * 0.75f;
  }*/
}