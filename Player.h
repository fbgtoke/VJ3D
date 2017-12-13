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
  bool isDead() const;
  bool isAlive() const;

private:
  static const float kTol;
  static const float kDiminish;
  static const float kJumpHeight;
  static const float kHorSpeed;

  enum State {
    Idle,
    Moving,
    Exploding,
    Dead
  };
  Player::State mState;
  void changeState(Player::State state);

  glm::vec3 mTargetPosition;
  glm::vec3 mStartPosition;
  void moveTowards(const glm::vec3& direction);

  void updateIdle(int deltaTime);
  void updateMoving(int deltaTime);
  void updateExploding(int deltaTime);
  void updateDead(int deltaTime);

  void renderIdle();
  void renderMoving();
  void renderExploding();
  void renderDead();

  void initExplosion();

  Mesh* mFrames[2];

  std::list<Particle*> mParticles;
};

#endif // _PLAYER_INCLUDE