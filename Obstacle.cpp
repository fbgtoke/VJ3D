#include "Obstacle.h"
#include "ObstacleSpawner.h"
#include "Game.h"

Obstacle::Obstacle(Obstacle::Type type)
  : mType(type), mShadow(nullptr) {}

Obstacle::~Obstacle() {}

Obstacle::Obstacle::Type Obstacle::getType() const { return mType; }

void Obstacle::init() {
  ModelAnimated::init();
  
  AnimationFrame frame;

  switch (mType) {
  case Obstacle::Cactus:
    mAnimation.setNumberOfAnimations(1);
    frame.mesh = Game::instance().getResource().mesh("cactus.obj");
    frame.transform = glm::mat4(1.f);
    mAnimation.addFrame(0, frame);
    mAnimation.setTimePerFrame(0);
    mAnimation.changeAnimation(0);
    break;
  case Obstacle::Stump:
    mAnimation.setNumberOfAnimations(1);
    frame.mesh = Game::instance().getResource().mesh("stump.obj");
    frame.transform = glm::mat4(1.f);
    mAnimation.addFrame(0, frame);
    mAnimation.setTimePerFrame(0);
    mAnimation.changeAnimation(0);
    break;
  case Obstacle::Stone:
    mAnimation.setNumberOfAnimations(1);
    frame.mesh = Game::instance().getResource().mesh("stone.obj");
    frame.transform = glm::mat4(1.f);
    mAnimation.addFrame(0, frame);
    mAnimation.setTimePerFrame(0);
    mAnimation.changeAnimation(0);
    break;
  case Obstacle::Carriage:
    mAnimation.setNumberOfAnimations(1);
    frame.transform = glm::mat4(1.f);
    frame.mesh = Game::instance().getResource().mesh("carriage1.obj");
    mAnimation.addFrame(0, frame);
    frame.mesh = Game::instance().getResource().mesh("animationcarriage1.obj");
    mAnimation.addFrame(0, frame);
    frame.mesh = Game::instance().getResource().mesh("carriage2.obj");
    mAnimation.addFrame(0, frame);
    frame.mesh = Game::instance().getResource().mesh("animationcarriage2.obj");
    mAnimation.addFrame(0, frame);
    mAnimation.setTimePerFrame(100);
    mAnimation.changeAnimation(0);
    break;
  case Obstacle::Horse:
    mAnimation.setNumberOfAnimations(1);
    frame.transform = glm::mat4(1.f);
    frame.mesh = Game::instance().getResource().mesh("horse.obj");
    mAnimation.addFrame(0, frame);
    frame.mesh = Game::instance().getResource().mesh("animationhorse1.obj");
    mAnimation.addFrame(0, frame);
    frame.mesh = Game::instance().getResource().mesh("horse.obj");
    mAnimation.addFrame(0, frame);
    frame.mesh = Game::instance().getResource().mesh("animationhorse2.obj");
    mAnimation.addFrame(0, frame);
    mAnimation.setTimePerFrame(100);
    mAnimation.changeAnimation(0);
    setRotation(glm::vec3(0.f, (float)M_PI/2.f, 0.f));
    break;
  case Obstacle::Boat:
    mAnimation.setNumberOfAnimations(1);
    frame.mesh = Game::instance().getResource().mesh("boat.obj");
    frame.transform = glm::mat4(1.f);
    mAnimation.addFrame(0, frame);
    mAnimation.setTimePerFrame(0);
    mAnimation.changeAnimation(0);
    break;
  default:
    mAnimation.setNumberOfAnimations(1);
    frame.mesh = Game::instance().getResource().mesh("cube.obj");
    frame.transform = glm::mat4(1.f);
    mAnimation.addFrame(0, frame);
    mAnimation.setTimePerFrame(0);
    mAnimation.changeAnimation(0);
    break;
  }

  setMesh(mAnimation.getCurrentFrame()->mesh);
  setTexture(Game::instance().getResource().texture("palette.png"));

  if (mType != Obstacle::Boat && mType != Obstacle::Stone && mType != Obstacle::Spawner) {
    mShadow = new Shadow(this);
    mShadow->init();
  }
}

void Obstacle::update(int deltaTime) {
  ModelAnimated::update(deltaTime);

  if (mVelocity.x >= 0.f)
    mScale.x = -1.f;

  if (mShadow != nullptr)
    mShadow->update(deltaTime);
}

void Obstacle::render() {
  ModelAnimated::render();

  if (mShadow != nullptr)
    mShadow->render();
}

void Obstacle::onDestroy() {
  if (mShadow != nullptr) {
    mShadow->destroy();
    delete mShadow;
    mShadow = nullptr;
  }
}