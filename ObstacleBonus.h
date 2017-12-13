#ifndef _OBSTACLE_BONUS_INCLUDE
#define _OBSTACLE_BONUS_INCLUDE

#include "Obstacle.h"

class ObstacleBonus : public Obstacle {
public:
  ObstacleBonus();
  ~ObstacleBonus() override;

  void init(unsigned int position);

private:
};

#endif // _OBSTACLE_BONUS_INCLUDE