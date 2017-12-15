#include "Player.h"
#include "Game.h"

const float Player::kTol = 1.1f;
const float Player::kDiminish = 1.f/2.f;
const float Player::kJumpHeight = 0.5f * TILE_SIZE;
const float Player::kHorSpeed = 0.075f;

const int Player::kMaxDrowningTime = 1000;

Player::Player() {}

Player::~Player() {
  for (auto it = mParticles.begin(); it != mParticles.end(); ++it)
    delete (*it);
  mParticles.clear();
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
  mStartPosition = glm::vec3(0.f);

  mState = Player::Idle;

  mDrowningTime = 0;

  mParabola = glm::vec3(0.f);
}

void Player::update(int deltaTime) {
  ModelAnimated::update(deltaTime);

  if (mState == Player::Moving)
    updateMoving(deltaTime);

  if (mState == Player::Drowning) {
    mRotation.y = 0.f;
    mPosition.y = TILE_SIZE * 0.5f;
    mDrowningTime += deltaTime;

    if (mDrowningTime > kMaxDrowningTime)
      explode();
  }
    
  if (mState == Player::Exploding)
    updateParticles(deltaTime);
}

void Player::render() {
  ModelAnimated::render();

  if (mState == Player::Exploding)
    renderParticles();
}

void Player::moveTowards(const glm::vec3& position) {
  mTargetPosition = position;
  mStartPosition = mPosition;

  glm::vec2 start(mStartPosition.x, mStartPosition.z);
  glm::vec2 target(mTargetPosition.x, mTargetPosition.z);
  float distance = glm::distance(start, target);

  glm::mat3 A;
  A[0][0] = 0.f;
  A[1][0] = 0.f;
  A[2][0] = 1.f;
  
  A[0][1] = distance * distance;
  A[1][1] = distance;
  A[2][1] = 1.f;
  
  A[0][2] = (distance * 0.5f) * (distance * 0.5f);
  A[1][2] = distance * 0.5f;
  A[2][2] = 1.f;
  
  glm::vec3 B;
  B[0] = mStartPosition.y;
  B[1] = mTargetPosition.y;
  B[2] = mStartPosition.y + kJumpHeight;

  mParabola = glm::inverse(A) * B;

  changeState(Player::Moving);
}

void Player::updateMoving(int deltaTime) {
  glm::vec2 direction;
  direction.x = mTargetPosition.x - mStartPosition.x;
  direction.y = mTargetPosition.z - mStartPosition.z;
  glm::normalize(direction);

  glm::vec2 start(mPosition.x, mPosition.z);
  glm::vec2 target(mTargetPosition.x, mTargetPosition.z);
  float distance = glm::distance(start, target);

  mVelocity = glm::normalize(glm::vec3(direction.x, 0.f, direction.y)) * kHorSpeed;
  mPosition.y =
    mParabola[0] * (distance * distance) +
    mParabola[1] * (distance) +
    mParabola[2];

  float angle = atan2(-direction.y, direction.x) + (float)M_PI/2.f;
  setRotation(UP * angle);

  if (distance < kTol) {
    mPosition = mTargetPosition;
    changeState(Player::Idle);    
  }
}

void Player::updateParticles(int deltaTime) {
  auto it = mParticles.begin();
  while (it != mParticles.end()) {
    Particle* particle = (*it);
    particle->update(deltaTime);

    if (!particle->isAlive()) {
      delete particle;
      mParticles.erase(it++);
    } else {
      it++;
    }
  }

  if (mParticles.empty())
    changeState(Player::Dead);
}

void Player::renderParticles() {
  for (Particle* particle : mParticles)
    if (particle->isAlive())
      particle->render();
}

void Player::initExplosion() {
  for (int i = 0; i < 500; ++i) {
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

    mParticles.push_back(particle);
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
  case Player::Drowning:
    mAnimation.changeAnimation(2);
    break;
  case Player::Exploding:
    setVelocity(glm::vec3(0.f));
    mAnimation.changeAnimation(3);
    Game::instance().getScene()->playSoundEffect("death.ogg");
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

void Player::checkCollision(const Obstacle* obstacle) {
  if (obstacle == nullptr) return;

  switch(obstacle->getType()) {
  case Obstacle::Cactus:
  case Obstacle::Stump:
  case Obstacle::Carriage:
  case Obstacle::Horse:
    explode();
    break;
  default:
    break;
  }
}

void Player::checkTile(Tile::Type type) {
  if (mState != Player::Idle) return;

  switch (type) {
  case Tile::Water:
      changeState(Player::Drowning);
    break;
  case Tile::Goal:
    Game::instance().changeScene(Scene::SCENE_WIN);
    break;
  default:
    break;
  }
}