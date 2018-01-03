#ifndef _OBSTACLE_BONUS_INCLUDE
#define _OBSTACLE_BONUS_INCLUDE

#include "Obstacle.h"

class ObstacleBonus : public Obstacle {
public:
  ObstacleBonus();

  void init() override;
  void update(int deltaTime) override;

  void pick();
  bool picked() const;

private:
  float kRotationSpeed;
  float kBobSpeed;

  bool mPicked;

  int mCurrentCycle;
};

#endif // _OBSTACLE_BONUS_INCLUDE