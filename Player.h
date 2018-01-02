#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include "ModelAnimated.h"
#include "Obstacle.h"
#include "Tile.h"
#include "Particle.h"

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
    towardsBoat,
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
  void moveTowardsBoat(Obstacle* boat);

  void onCollision(Model* model) override;
  bool checkCollisions() const override;

  void getAdjacentTiles(glm::vec3 v[3][3]) const;

private:
  float kTol;
  float kJumpSpeed;
  float kHorSpeed;

  void beforeRender() override;

  Player::State mState;

  glm::vec3 mTargetPosition;
  Obstacle* mTargetBoat;

  int kMaxDrowningTime;
  int kMaxExplodingTime;
  bool mTimerActivated;
  int mTimer;
  void setTimer(int time);
  bool timerExpired() const;

  void updateMoving(int deltaTime);

  void updateParticles(int deltaTime);
  void renderParticles();

  void initExplosion();
};

#endif // _PLAYER_INCLUDE