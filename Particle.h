#ifndef _PARTICLE_INCLUDE
#define _PARTICLE_INCLUDE

#include "Model.h"

class Particle : public Model {
public:
  void init(int aliveTime);
  void update(int deltaTime) override;
  
  bool isAlive() const;

private:
  static const float kGravity;
  static const float kFriction;
  static const float kMaxVelocity;
  int mAliveTime;
};

#endif // _PARTICLE_INCLUDE