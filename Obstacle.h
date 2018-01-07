#ifndef _OBSTACLE_INCLUDE
#define _OBSTACLE_INCLUDE

#include "ModelAnimated.h"

class Obstacle : public ModelAnimated {
public:
  enum Type {
    Cactus = 0,
    Stump,
    Stone,
    Bonus,
    Bottle,
    Carriage,
    Horse,
    Boat,
    Spawner
  };

  Obstacle(Obstacle::Type type);
  ~Obstacle() override;

  void init() override;
  void update(int deltaTime) override;

  virtual Obstacle::Type getType() const;

private:  
  const Obstacle::Type mType;
};

#endif // _OBSTACLE_INCLUDE