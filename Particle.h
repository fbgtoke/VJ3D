#ifndef _PARTICLE_INCLUDE
#define _PARTICLE_INCLUDE

#include "Model.h"

class Particle : public Model {
public:
  void init(int aliveTime);
  void update(int deltaTime) override;
  
  bool isAlive() const;

private:
  int mAliveTime;
};

#endif // _PARTICLE_INCLUDE