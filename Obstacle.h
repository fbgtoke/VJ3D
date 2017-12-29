#ifndef _OBSTACLE_INCLUDE
#define _OBSTACLE_INCLUDE

#include "ModelAnimated.h"
#include "Shadow.h"

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
  ~Obstacle() override;

  void init() override;
  void update(int deltaTime) override;

  virtual Obstacle::Type getType() const;

private:
  void beforeRender() override;

  void onDestroy() override;
  
  const Obstacle::Type mType;
  Shadow* mShadow;
};

#endif // _OBSTACLE_INCLUDE