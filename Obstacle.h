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
    Carriage,
    Horse,
    Boat,
    Spawner
  };

  Obstacle(Obstacle::Type type);
  void init() override;
  void update(int deltaTime) override;
  void render() override;

  virtual Obstacle::Type getType() const;

private:
  const Obstacle::Type mType;

  void renderShadow();
};

#endif // _OBSTACLE_INCLUDE