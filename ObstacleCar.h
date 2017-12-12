#ifndef _OBSTACLE_CAR_INCLUDE
#define _OBSTACLE_CAR_INCLUDE

#include "Obstacle.h"
#include "Animation3D.h"

class ObstacleCar : public Obstacle {
public:
  ObstacleCar();
  ~ObstacleCar() override;

  void init(float spd);
  void update(int deltaTime) override;

  void setVelocity(const glm::vec3& velocity) override;

private:
  Animation3D mAnimation;
};

#endif // _OBSTACLE_CAR_INCLUDE