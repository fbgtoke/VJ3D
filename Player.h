#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include "Model.h"
#include "Particle.h"

class Player : public Model {
public:
	Player();
	~Player() override;

	void init() override;
	void update(int deltaTime) override;
  void render() override;

  void explode();

  bool isIdle() const;

private:
  static const float kTol;
  static const float kDiminish;
  static const float kJumpHeight;
  static const float kHorSpeed;

  glm::vec3 mTargetPosition;
  glm::vec3 mStartPosition;
  bool mMovingTowardsTarget;
  void moveTowards(const glm::vec3& direction);

  void updateMoving(int deltaTime);
  void updateIdle(int deltaTime);

  Mesh* mFrames[2];

  std::list<Particle*> mParticles;
};

#endif // _PLAYER_INCLUDE