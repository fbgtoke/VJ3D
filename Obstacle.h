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

  void getBoundingBox(glm::vec3& mincoords, glm::vec3& maxcoords) const override;

private:
  const Obstacle::Type mType;
  Shadow* mShadow;
};

#endif // _OBSTACLE_INCLUDE