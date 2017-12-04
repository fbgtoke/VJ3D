#ifndef _OBSTACLE_INCLUDE
#define _OBSTACLE_INCLUDE

#include "Model.h"

class Obstacle : public Model {
public:
  enum ObstacleType {
    TREE = 0,
    CAR,
    LILLYPAD,
    NUM_TYPES
  };

  Obstacle(ObstacleType type);
  virtual ~Obstacle();

  static Obstacle* create(ObstacleType type);
  static Obstacle* createFromStream(std::istringstream& sstream);

  ObstacleType getType() const;

private:
  static Obstacle* readTree(std::istringstream& sstream);
  static Obstacle* readCar(std::istringstream& sstream);

  const ObstacleType mType;
};

#endif // _OBSTACLE_INCLUDE