#ifndef _OBSTACLE_LOG_INCLUDE
#define _OBSTACLE_LOG_INCLUDE

#include "Obstacle.h"

class ObstacleLog : public Obstacle {
public:
  ObstacleLog();
  ~ObstacleLog() override;

  void init(float spd);

  void setVelocity(const glm::vec3& velocity) override;
};

#endif // _OBSTACLE_LOG_INCLUDE