#include "Player.h"
#include "Game.h"

Player::Player() {}

Player::~Player() {}

void Player::init() {
  ModelAnimated::init();

  kTol = Game::instance().getResource().Float("distanceTolerance");
  kJumpSpeed = Game::instance().getResource().Float("jumpSpeed");
  kHorSpeed = Game::instance().getResource().Float("horizontalSpeed");
  kMaxDrowningTime = Game::instance().getResource().Int("drowningTime");
  kMaxExplodingTime = Game::instance().getResource().Int("explodingTime");

  kMinJumpsChangeSound = Game::instance().getResource().Int("minJumpsChangeSound");
  kMaxJumpsChangeSound = Game::instance().getResource().Int("maxJumpsChangeSound");

  AnimationFrame frame;
  frame.transform = glm::mat4(1.f);

  mAnimation.setNumberOfAnimations(4);
  frame.mesh = Game::instance().getResource().mesh("cowboy.obj");
  mAnimation.addFrame(0, frame);
  
  frame.mesh = Game::instance().getResource().mesh("animationcowboy.obj");
  frame.transform = glm::mat4(1.f);
  int kJumpFrames = Game::instance().getResource().Int("jumpFrames");
  for (int i = 0; i < kJumpFrames/2; ++i) {
    frame.transform = glm::translate(frame.transform, UP * kJumpSpeed);
    mAnimation.addFrame(1, frame);
  }
  for (int i = 0; i < kJumpFrames/2; ++i) {
    frame.transform = glm::translate(frame.transform, DOWN * kJumpSpeed);
    mAnimation.addFrame(1, frame);
  }

  frame.mesh = Game::instance().getResource().mesh("drowningcowboy1.obj");
  mAnimation.addFrame(2, frame);
  frame.mesh = Game::instance().getResource().mesh("drowningcowboy2.obj");
  mAnimation.addFrame(2, frame);
  
  mAnimation.setTimePerFrame(100);
  mAnimation.changeAnimation(0);
  setMesh(mAnimation.getCurrentFrame()->mesh);
  
  setTexture(Game::instance().getResource().texture("palette.png"));

  mTargetPosition = glm::vec3(0.f);

  mState = Player::Idle;

  mTimer = 0;
  mTimerActivated = false;

  mJumpsChangeSound = randomInt(kMinJumpsChangeSound, kMaxJumpsChangeSound);

  mDrunk = false;
  kDrunkCountdownMax = Game::instance().getResource().Int("drunkCountdownMax");
}

void Player::update(int deltaTime) {
  ModelAnimated::update(deltaTime);
  mTimer -= deltaTime;

  if (mState == Player::Moving)
    updateMoving(deltaTime);

  if (mState == Player::onBoat) {
    mPosition = mTargetBoat->getTopCenter();
    mPosition.y += getSize().y * 0.5f;
  }

  if (mState == Player::Drowning) {
    mRotation.y = 0.f;
    mPosition.y = TILE_SIZE * 0.5f;
    
    if (timerExpired())
      explode();
  }

  if (mState == Player::Exploding && timerExpired())
    changeState(Player::Dead);

  if (mDrunk) {
    mDrunkCountdown -= deltaTime;
    mDrunk = (mDrunkCountdown > 0);
  }
}

void Player::beforeRender() {
  mEnableRendering =
    mState != Player::Exploding &&
    mState != Player::Dead;
  ModelAnimated::beforeRender();
}

void Player::moveTowards(const glm::vec3& position) {
  mTargetPosition = position;
  
  changeState(Player::Moving);
}

void Player::moveTowardsBoat(Obstacle* boat) {
  mTargetBoat = boat;
  changeState(Player::onBoat);
  playJumpSound();
}

void Player::updateMoving(int deltaTime) {
  glm::vec3 direction = mTargetPosition - mPosition;
  glm::normalize(direction);

  if (mState == Player::Moving)
    mVelocity = direction * kHorSpeed;
  else if (mState == Player::towardsBoat)
    mVelocity = direction * abs(mTargetBoat->getVelocity().x);
  
  float angle = atan2(-direction.z, direction.x) + (float)M_PI/2.f;
  setRotation(UP * angle);

  float distance = glm::distance(mPosition, mTargetPosition);
  if (mState == Player::Moving && distance < kTol) {
    mPosition = mTargetPosition;
    mVelocity = glm::vec3(0.f);

    changeState(Player::Idle);
  } else if (mState == Player::towardsBoat && distance < mTargetBoat->getSize().x * 0.5f)
    changeState(Player::onBoat);
}

void Player::changeState(Player::State state) {
  switch (state) {
  case Player::Idle:
    setPositionInTiles(getPositionInTiles());
    setVelocity(glm::vec3(0.f));
    mAnimation.changeAnimation(0);
    break;
  case Player::Moving:
  case Player::towardsBoat:
    mAnimation.changeAnimation(1);
    playJumpSound();
    break;
  case Player::onBoat:
    mVelocity = glm::vec3(0.f);
    mAnimation.changeAnimation(0);
    break;
  case Player::Drowning:
    setTimer(kMaxDrowningTime);
    mAnimation.changeAnimation(2);
    Game::instance().getScene()->playSoundEffect("deathDrowning.ogg");
    break;
  case Player::Exploding:
    setTimer(kMaxExplodingTime);
    setVelocity(glm::vec3(0.f));
    Particle::generateExplosion(getCenter());

    if (mState == Player::Drowning)
      Game::instance().getScene()->playSoundEffect("drowning.ogg");
    break;
  case Player::Dead:
    break;
  }

  mState = state;
}

Player::State Player::getState() const { return mState; }

void Player::explode() { changeState(Player::Exploding); }
bool Player::isIdle() const { return mState == Player::Idle; }
bool Player::isExploding() const { return mState == Player::Exploding; }
bool Player::isDead() const { return mState == Player::Dead; }
bool Player::isAlive() const {
  return
    mState == Player::Idle ||
    mState == Player::Moving ||
    mState == Player::towardsBoat ||
    mState == Player::onBoat;
}

void Player::onCollision(Model* model) {
  Obstacle* obstacle = dynamic_cast<Obstacle*> (model);
  if (obstacle != nullptr) {
    glm::vec3 min, max;

    switch (obstacle->getType()) {
    case Obstacle::Cactus:
      if (isAlive()) {
        explode();
        Game::instance().getScene()->playSoundEffect("deathCactus.ogg");
      }
      break;
    case Obstacle::Carriage:
    case Obstacle::Horse:
      if (isAlive()) {
        explode();

        if (randomInt(0, 100) < 50)
          Game::instance().getScene()->playSoundEffect("deathOverrun0.ogg");
        else
          Game::instance().getScene()->playSoundEffect("deathOverrun1.ogg");
      }
      break;
    case Obstacle::Stone:
      break;
    case Obstacle::Boat:
      if (mState != Player::Drowning) {
        obstacle->getMesh()->getMinMaxVertices(min, max);
        mPosition.y += max.y;
      } else {
        explode();
      }
      break;
    case Obstacle::Bonus:
      if (!(dynamic_cast<ObstacleBonus*>(obstacle))->picked()) {
        (dynamic_cast<ObstacleBonus*>(obstacle))->pick();
        Game::instance().getScene()->playSoundEffect("bonus.ogg");
      }
      break;
    case Obstacle::Bottle:
      mDrunk = true;
      mDrunkCountdown = kDrunkCountdownMax;
      obstacle->destroy();
      Game::instance().getScene()->playSoundEffect("bonus.ogg");
      break;
    default:
      break;
    }
  }
}

void Player::getAdjacentTiles(glm::vec3 v[3][3]) const {
  glm::vec3 position = getPositionInTiles();
  position.y = 0.f;

  v[0][0] = position + LEFT + IN;
  v[0][1] = position + IN;
  v[0][2] = position + RIGHT + IN;

  v[1][0] = position + LEFT;
  v[1][1] = position;
  v[1][2] = position + RIGHT;
  
  v[2][0] = position + LEFT + OUT;
  v[2][1] = position + OUT;
  v[2][2] = position + RIGHT + OUT;
}

bool Player::isDrunk() const { return mDrunk; }

bool Player::checkCollisions() const { return true; }

void Player::setTimer(int time) {
  mTimerActivated = true;
  mTimer = time;
}

bool Player::timerExpired() const {
  return mTimerActivated && mTimer < 0;
}

void Player::playJumpSound() {
  --mJumpsChangeSound;

  if (mJumpsChangeSound <= 0) {
    int sound = randomInt(0, 100);

    if (sound < 30)
      Game::instance().getScene()->playSoundEffect("jump1.ogg");
    else if (sound < 60)
      Game::instance().getScene()->playSoundEffect("jump2.ogg");
    else
      Game::instance().getScene()->playSoundEffect("jump3.ogg");

    mJumpsChangeSound = randomInt(kMinJumpsChangeSound, kMaxJumpsChangeSound);
  } else {
    Game::instance().getScene()->playSoundEffect("jump0.ogg");
  }
}