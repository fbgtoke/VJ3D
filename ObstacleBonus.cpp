#include "ObstacleBonus.h"
#include "Game.h"

ObstacleBonus::ObstacleBonus()
  : Obstacle(Obstacle::Bonus) {}

void ObstacleBonus::init() {
  Obstacle::init();

  kRotationSpeed = Game::instance().getResource().Float("RotationSpeed");
  kRotationSpeedFast = Game::instance().getResource().Float("RotationSpeedFast");
  kBobSpeed = Game::instance().getResource().Float("BobSpeed");
  kMaxRotationTime = Game::instance().getResource().Int("MaxRotationTime");

  AnimationFrame frame;
  frame.mesh = Game::instance().getResource().mesh("bonus1.obj");
  frame.transform = glm::mat4(1.f);

  mAnimation.setNumberOfAnimations(1);
  mAnimation.addFrame(0, frame);
  mAnimation.setTimePerFrame(0);
  mAnimation.changeAnimation(0);
  setRotationSpeed(glm::vec3(0.f, kRotationSpeed, 0.f));

  mCurrentCycle = 0;

  mState = ObstacleBonus::Idle;
}

void ObstacleBonus::update(int deltaTime) {
  Obstacle::update(deltaTime);
  mCurrentCycle += deltaTime;

  if (mState == ObstacleBonus::Idle) {
    mPosition.y += 0.5f * sin(kBobSpeed * (float)mCurrentCycle);
  } else if (mState == ObstacleBonus::RecentPicked) {
    if (mCurrentCycle >= kMaxRotationTime) {
      mState = ObstacleBonus::Exiting;
      mVelocity.y = 0.2f;
      mCurrentCycle = 0;
    }
  } else {
    if (mCurrentCycle >= kMaxRotationTime)
      destroy();
  }
}

void ObstacleBonus::pick() {
  mState = ObstacleBonus::RecentPicked;
  mCurrentCycle = 0;
  
  setRotationSpeed(glm::vec3(0.f, kRotationSpeedFast, 0.f));
}

bool ObstacleBonus::picked() const { return mState != ObstacleBonus::Idle; }