#ifndef _OBSTACLE_TREE_INCLUDE
#define _OBSTACLE_TREE_INCLUDE

#include "Obstacle.h"

class ObstacleTree : public Obstacle {
public:
  ObstacleTree();
  ~ObstacleTree() override;

  void init(unsigned int position);

private:
};

#endif // _OBSTACLE_TREE_INCLUDE