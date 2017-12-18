#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include "ModelAnimated.h"
#include "Obstacle.h"
#include "Tile.h"
#include "Particle.h"
#include "Shadow.h"

class Player : public ModelAnimated {
public:
	Player();
	~Player() override;

	void init() override;
	void update(int deltaTime) override;

  enum State {
    Idle,
    onBoat,
    Moving,
    Exploding,
    Drowning,
    Dead
  };
  void changeState(Player::State state);
  Player::State getState() const;

  void explode();
  bool isIdle() const;
  bool isExploding() const;
  bool isDead() const;
  bool isAlive() const;

  void moveTowards(const glm::vec3& position);

  void onCollision(Model* model) override;
  bool checkCollisions() const override;

private:
  static const float kTol;
  static const float kJumpSpeed;
  static const float kHorSpeed;
  static const float kGravity;

  Player::State mState;

  glm::vec3 mTargetPosition;

  static const int kMaxDrowningTime;
  static const int kMaxExplodingTime;
  bool mTimerActivated;
  int mTimer;
  void setTimer(int time);
  bool timerExpired() const;

  void updateMoving(int deltaTime);

  void updateParticles(int deltaTime);
  void renderParticles();

  void initExplosion();

  Shadow* mShadow;
};

#endif // _PLAYER_INCLUDE