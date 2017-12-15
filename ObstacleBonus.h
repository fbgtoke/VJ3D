#ifndef _OBSTACLE_BONUS_INCLUDE
#define _OBSTACLE_BONUS_INCLUDE

#include "Obstacle.h"

class ObstacleBonus : public Obstacle {
public:
  ObstacleBonus();

  void init() override;
  void update(int deltaTime) override;

private:
  static const float kRotationSpeed;
  static const float kBobSpeed;

  int mCurrentCycle;
};

#endif // _OBSTACLE_BONUS_INCLUDE