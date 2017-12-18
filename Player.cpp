#include "Player.h"
#include "Game.h"

const float Player::kTol = 1.1f;
const float Player::kJumpSpeed = 0.2f;
const float Player::kHorSpeed = 0.015f;
const float Player::kGravity = 0.004f;

const int Player::kMaxDrowningTime = 1000;
const int Player::kMaxExplodingTime = 2000;

Player::Player() :
  mShadow(nullptr) {}

Player::~Player() {
  if (mShadow != nullptr)
    mShadow->unbind();
}

void Player::init() {
  ModelAnimated::init();

  mAnimation.setNumberOfAnimations(4);
  mAnimation.addFrame(0, Game::instance().getResource().mesh("cowboy.obj"));
  mAnimation.addFrame(1, Game::instance().getResource().mesh("animationcowboy.obj"));
  mAnimation.addFrame(2, Game::instance().getResource().mesh("drowningcowboy1.obj"));
  mAnimation.addFrame(2, Game::instance().getResource().mesh("drowningcowboy2.obj"));
  mAnimation.addFrame(3, nullptr);
  mAnimation.setTimePerFrame(200);
  mAnimation.changeAnimation(0);
  setMesh(mAnimation.getCurrentFrame());
  
  setTexture(Game::instance().getResource().texture("palette.png"));

  mTargetPosition = glm::vec3(0.f);

  mState = Player::Idle;

  mTimer = 0;
  mTimerActivated = false;

  mShadow = new Shadow(this);
  mShadow->init();
  Game::instance().getScene()->addModel(mShadow);
}

void Player::update(int deltaTime) {
  ModelAnimated::update(deltaTime);
  mTimer -= deltaTime;

  if (mState == Player::Moving)
    updateMoving(deltaTime);

  if (mState == Player::Drowning) {
    mRotation.y = 0.f;
    mPosition.y = TILE_SIZE * 0.5f;
    
    if (timerExpired())
      explode();
  }

  if (mState == Player::Exploding && timerExpired())
      changeState(Player::Dead);
}

void Player::moveTowards(const glm::vec3& position) {
  mTargetPosition = position;
  mTargetPosition.y = 0.f;

  mVelocity.y = kJumpSpeed;

  changeState(Player::Moving);
}

void Player::updateMoving(int deltaTime) {
  glm::vec3 direction;
  direction.x = mTargetPosition.x - mPosition.x;
  direction.y = 0.f;
  direction.z = mTargetPosition.z - mPosition.z;
  glm::normalize(direction);

  mVelocity.x = direction.x * kHorSpeed;
  mVelocity.y = mVelocity.y - kGravity * deltaTime;
  mVelocity.z = direction.z * kHorSpeed;
  
  float angle = atan2(-direction.z, direction.x) + (float)M_PI/2.f;
  setRotation(UP * angle);

  glm::vec3 current(mPosition.x, 0.f, mPosition.z);
  float distance = glm::distance(current, mTargetPosition);
  if (distance < kTol) {
    mPosition = mTargetPosition;
    mVelocity = glm::vec3(0.f);
    changeState(Player::Idle);    
  }
}

void Player::initExplosion() {
  for (int i = 0; i < 50; ++i) {
    Particle* particle = new Particle();
    particle->init(15000 + rand()%200);
    particle->setMesh(Game::instance().getResource().mesh("cube.obj"));

    int texture = rand()%3;
    if (texture == 0)
      particle->setTexture(Game::instance().getResource().texture("palette.png"));
    else if (texture == 1)
      particle->setTexture(Game::instance().getResource().texture("palette.png"));
    else
      particle->setTexture(Game::instance().getResource().texture("palette.png"));

    particle->setPosition(getCenter() + DOWN * TILE_SIZE);
    particle->setScale(glm::vec3(0.07f));

    glm::vec3 direction;
    direction.x = randomFloat(-.2f, .2f);
    direction.y = randomFloat( 0.f, 1.5f);
    direction.z = randomFloat(-.2f, .2f);
    particle->setVelocity(glm::normalize(direction) * 0.05f);

    Game::instance().getScene()->addModel(particle);
  }
}

void Player::changeState(Player::State state) {
  if (state == mState) return;
  mState = state;

  switch (state) {
  case Player::Idle:
    setVelocity(glm::vec3(0.f));
    mAnimation.changeAnimation(0);
    break;
  case Player::Moving:
    mAnimation.changeAnimation(1);
    Game::instance().getScene()->playSoundEffect("piu.ogg");
    break;
  case Player::onBoat:
    mVelocity.y = 0.f;
    mVelocity.z = 0.f;
    mAnimation.changeAnimation(0);
    break;
  case Player::Drowning:
    setTimer(kMaxDrowningTime);
    mAnimation.changeAnimation(2);
    mShadow->unbind();
    break;
  case Player::Exploding:
    setTimer(kMaxExplodingTime);
    setVelocity(glm::vec3(0.f));
    mAnimation.changeAnimation(3);
    Game::instance().getScene()->playSoundEffect("death.ogg");
    mShadow->unbind();
    initExplosion();
    break;
  case Player::Dead:
    mAnimation.changeAnimation(3);
    break;
  }
}

Player::State Player::getState() const { return mState; }

void Player::explode() { changeState(Player::Exploding); }
bool Player::isIdle() const { return mState == Player::Idle; }
bool Player::isExploding() const { return mState == Player::Exploding; }
bool Player::isDead() const { return mState == Player::Dead; }
bool Player::isAlive() const {
  return mState == Player::Idle || mState == Player::Moving;
}

void Player::onCollision(Model* model) {
  Obstacle* obstacle = dynamic_cast<Obstacle*> (model);
  if (obstacle != nullptr) {
    glm::vec3 min, max;

    switch (obstacle->getType()) {
    case Obstacle::Cactus:
    case Obstacle::Stump:
    case Obstacle::Carriage:
    case Obstacle::Horse:
      if (isAlive())
        explode();
      break;
    case Obstacle::Stone:
      obstacle->getMesh()->getMinMaxVertices(min, max);
      mPosition.y += max.y;
      changeState(Player::Idle);
      break;
    case Obstacle::Boat:
      if (isAlive()) {
        obstacle->getMesh()->getMinMaxVertices(min, max);
        mPosition.x = obstacle->getCenter().x;
        mPosition.y += max.y;
        mPosition.z = obstacle->getCenter().z;
        mVelocity.x = obstacle->getVelocity().x;
        changeState(Player::onBoat);
      } else if (mState == Player::Drowning) {
        explode();
      }
      break;
    case Obstacle::Bonus:
      std::cout << "GOT" << std::endl;
      obstacle->destroy();
      break;
    default:
      break;
    }
  }

  Tile* tile = dynamic_cast<Tile*> (model);
  if (tile != nullptr) {
    glm::vec3 position = tile->getPositionInTiles();
    setPositionInTiles(position + UP);

    switch (tile->getType()) {
    case Tile::Water:
      if (mState == Player::Idle)
        changeState(Player::Drowning);
      break;
    case Tile::Goal:
      Game::instance().changeScene(Scene::SCENE_WIN);
      break;
    default:
      if (mState == Player::Moving)
        changeState(Player::Idle);
      break;
    }
  }
}

bool Player::checkCollisions() const { return true; }

void Player::setTimer(int time) {
  mTimerActivated = true;
  mTimer = time;
}

bool Player::timerExpired() const {
  return mTimerActivated && mTimer < 0;
}