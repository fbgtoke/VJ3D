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
  void render() override;

  enum State {
    Idle,
    OnLog,
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

  void moveTowards(const glm::vec3& direction);

  void checkCollision(const Obstacle* obstacle);
  void checkTile(Tile::Type type);

private:
  static const float kTol;
  static const float kDiminish;
  static const float kJumpHeight;
  static const float kHorSpeed;

  Player::State mState;

  glm::vec3 mTargetPosition;
  glm::vec3 mStartPosition;

  static const int kMaxDrowningTime;
  int mDrowningTime;

  glm::vec3 mParabola;

  void updateMoving(int deltaTime);

  void updateParticles(int deltaTime);
  void renderParticles();

  void initExplosion();

  std::list<Particle*> mParticles;
};

#endif // _PLAYER_INCLUDE