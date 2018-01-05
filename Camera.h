#ifndef _CAMERA_INCLUDE
#define _CAMERA_INCLUDE

#include "utils.h"
#include "Model.h"

class Camera {
public:
  Camera();
  ~Camera();

  void init();
  void update(int deltaTime);

  void setMoving(bool moving);
  void follow(const Model* model);

  bool outOfView(const Model* model) const;

  glm::mat4 getProjectionMatrix() const;
  glm::mat4 getViewMatrix() const;

private:
  glm::vec3 kObsVector;
  float kVel;
  float kVelFast;
  float kMargin;
  float mVel;

  bool mMoving;
  glm::vec3 OBS;
  glm::vec3 VRP;

  const Model* mFollowing;
};

#endif // _CAMERA_INCLUDE