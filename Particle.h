#ifndef _PARTICLE_INCLUDE
#define _PARTICLE_INCLUDE

#include "Model.h"
#include "Tile.h"

class Particle : public Model {
public:
  void init(int aliveTime);
  void update(int deltaTime) override;

  void onCollision(Model* model) override;
  bool checkCollisions() const override;

private:
  static const float kGravity;
  static const float kFriction;
  int mAliveTime;
};

#endif // _PARTICLE_INCLUDE