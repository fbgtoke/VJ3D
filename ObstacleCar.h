#ifndef _OBSTACLE_CAR_INCLUDE
#define _OBSTACLE_CAR_INCLUDE

#include "Obstacle.h"

class ObstacleCar : public Obstacle {
public:
  ObstacleCar();
  ~ObstacleCar() override;

  void init(float spd);

  void setVelocity(const glm::vec3& velocity) override;
};

#endif // _OBSTACLE_CAR_INCLUDE