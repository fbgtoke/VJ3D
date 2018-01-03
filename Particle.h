#ifndef _PARTICLE_INCLUDE
#define _PARTICLE_INCLUDE

#include "Model.h"
#include "Tile.h"

class Particle : public Model {
public:
  void init();
  void update(int deltaTime) override;

  void setAliveTime(int aliveTime);
  void setColor(const glm::vec3& color);

  static void generateExplosion(const glm::vec3& origin);

private:
  float kGravity;
  float kFriction;

  int mAliveTime;
  glm::vec3 mColor;
};

#endif // _PARTICLE_INCLUDE