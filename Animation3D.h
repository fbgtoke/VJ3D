#ifndef _ANIMATION3D_INCLUDE
#define _ANIMATION3D_INCLUDE

#include "utils.h"
#include "Mesh.h"

typedef struct {
  Mesh* mesh = nullptr;
  glm::mat4 transform = glm::mat4(1.f);
} AnimationFrame;

class Animation3D {
public:
  Animation3D();
  ~Animation3D();

  void update(unsigned int deltaTime);

  void setNumberOfAnimations(unsigned int number);
  void addFrame(unsigned int animationId, const AnimationFrame& frame);

  void changeAnimation(unsigned int animationId);

  void setPlay(bool play);
  void setRepeat(bool repeat);

  bool isPlaying() const;
  bool isRepeating() const;

  void setTimePerFrame(unsigned int time);

  void nextFrame();

  const AnimationFrame* getCurrentFrame() const;

private:
  typedef std::vector<AnimationFrame> Animation;
  std::vector<Animation> mAnimations;

  unsigned int mCurrentAnimation;
  unsigned int mCurrentFrame;

  bool mPlaying;
  bool mRepeating;

  unsigned int mTimePerFrame;
  unsigned int mTimeCurFrame;
};

#endif // _ANIMATION3D_INCLUDE