#ifndef _OBSTACLE_BONUS_INCLUDE
#define _OBSTACLE_BONUS_INCLUDE

#include "Obstacle.h"

class ObstacleBonus : public Obstacle {
public:
  ObstacleBonus();

  void init() override;
  void update(int deltaTime) override;

private:
  float kRotationSpeed;
  float kBobSpeed;

  int mCurrentCycle;
};

#endif // _OBSTACLE_BONUS_INCLUDE