#ifndef _OBSTACLE_INCLUDE
#define _OBSTACLE_INCLUDE

#include "Model.h"

class Obstacle : public Model {
public:
  enum Type {
    Cactus = 0,
    Stump,
    Stone,
    Bonus,
    Carriage,
    Horse,
    Boat,
    Spawner
  };

  Obstacle(Obstacle::Type type);
  void init() override;
  void update(int deltaTime) override;

  virtual Obstacle::Type getType() const;

private:
  const Obstacle::Type mType;
};

#endif // _OBSTACLE_INCLUDE