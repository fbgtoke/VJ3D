#ifndef _ANIMATION3D_INCLUDE
#define _ANIMATION3D_INCLUDE

#include "utils.h"
#include "Mesh.h"

class Animation3D {
public:
  Animation3D();
  ~Animation3D();

  void update(unsigned int deltaTime);

  void setNumberOfAnimations(unsigned int number);
  void addFrame(unsigned int animationId, Mesh* frame);

  void changeAnimation(unsigned int animationId);

  void setPlay(bool play);
  void setRepeat(bool repeat);

  bool isPlaying() const;
  bool isRepeating() const;

  void setTimePerFrame(unsigned int time);

  void nextFrame();

  Mesh* getCurrentFrame();

private:
  typedef std::vector<Mesh*> Animation;
  std::vector<Animation> mAnimations;

  int mCurrentAnimation;
  int mCurrentFrame;

  bool mPlaying;
  bool mRepeating;

  unsigned int mTimePerFrame;
  unsigned int mTimeCurFrame;
};

#endif // _ANIMATION3D_INCLUDE