#include "Animation3D.h"

Animation3D::Animation3D() {
  mCurrentAnimation = -1;
  mCurrentFrame = -1;

  mPlaying = true;
  mRepeating = true;

  mTimePerFrame = 100;
  mTimeCurFrame = 0;
}

Animation3D::~Animation3D() {
  mAnimations.clear();
}

void Animation3D::update(unsigned int deltaTime) {
  if (!mPlaying) return;

  mTimeCurFrame += deltaTime;
  if (mTimeCurFrame >= mTimePerFrame)
    nextFrame();
}

void Animation3D::setNumberOfAnimations(unsigned int number) {
  mAnimations.clear();
  mAnimations.resize(number);
}

void Animation3D::addFrame(unsigned int animationId, std::shared_ptr<Mesh> frame) {
  mAnimations[animationId].push_back(frame);
}

void Animation3D::changeAnimation(unsigned int animationId) {
  mCurrentAnimation = animationId;
  mCurrentFrame = 0;

  mTimeCurFrame = 0;
}

void Animation3D::setPlay(bool play) { mPlaying = play; }
void Animation3D::setRepeat(bool repeat) { mRepeating = repeat; }

bool Animation3D::isPlaying() const { return mPlaying; }
bool Animation3D::isRepeating() const { return mRepeating; }

void Animation3D::setTimePerFrame(unsigned int time) {
  mTimePerFrame = time;
  mTimeCurFrame = 0;
}

void Animation3D::nextFrame() {
  if (mCurrentFrame < mAnimations[mCurrentAnimation].size() - 1)
    ++mCurrentFrame;
  else if (mRepeating)
    mCurrentFrame = 0;

  mTimeCurFrame = 0;
}

std::shared_ptr<Mesh> Animation3D::getCurrentFrame() {
  return mAnimations[mCurrentAnimation][mCurrentFrame];
}