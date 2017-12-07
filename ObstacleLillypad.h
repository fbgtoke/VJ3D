#ifndef _OBSTACLE_LILLYPAD_INCLUDE
#define _OBSTACLE_LILLYPAD_INCLUDE

#include "Obstacle.h"

class ObstacleLillypad : public Obstacle {
public:
  ObstacleLillypad();
  ~ObstacleLillypad() override;

  void init(unsigned int position);

private:
};

#endif // _OBSTACLE_LILLYPAD_INCLUDE