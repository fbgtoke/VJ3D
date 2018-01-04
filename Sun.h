#ifndef _SUN_INCLUDE
#define _SUN_INCLUDE

#include "utils.h"

class Sun {
public:
  Sun();
  ~Sun();

  void init();
  void update(int deltaTime);

  void setCurrentTime(float time);

  void setLevelSize(unsigned int width, unsigned int length);

  glm::vec3 getPosition() const;
  glm::vec3 getTarget() const;
  glm::vec3 getDirection() const;

  glm::vec3 getColor() const;

  glm::mat4 getBiasMatrix() const;
  glm::mat4 getProjectionMatrix() const;
  glm::mat4 getViewMatrix() const;

private:
  glm::vec3 kTarget;
  float kDistance;

  float mCurrentTime;
  unsigned int mWidth  = 0;
  unsigned int mLength = 0;

  float kMorningTime;
  float kDayTime;
  float kTwilightTime;
  float kNightTime;

  glm::vec3 kMorningColor;
  glm::vec3 kDayColor;
  glm::vec3 kTwilightColor;
  glm::vec3 kNightColor;
};

#endif // _SUN_INCLUDE