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
  float kRotationSpeedFast;
  float kBobSpeed;
  int kMaxRotationTime;

  int mCurrentCycle;

  enum State {
    Idle,
    RecentPicked,
    Exiting
  };
  ObstacleBonus::State mState;
};

#endif // _OBSTACLE_BONUS_INCLUDE