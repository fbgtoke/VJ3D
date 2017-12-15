#include "Player.h"
#include "Game.h"

const float Player::kTol = 0.95f;
const float Player::kDiminish = 1.f/2.f;
const float Player::kJumpHeight = 0.25f * TILE_SIZE;
const float Player::kHorSpeed = 0.025f;

Player::Player() {}

Player::~Player() {
  for (auto it = mParticles.begin(); it != mParticles.end(); ++it)
    delete (*it);
  mParticles.clear();
}

void Player::init() {
  Model::init();

  mFrames[0] = Game::instance().getResource().mesh("cowboy.obj");
  mFrames[1] = Game::instance().getResource().mesh("animationcowboy.obj");
  setMesh(mFrames[0]);
  
  setTexture(Game::instance().getResource().texture("palette.png"));

  mTargetPosition = glm::vec3(0.f);
  mStartPosition = glm::vec3(0.f);

  mState = Player::Idle;
}

void Player::update(int deltaTime) {
  switch (mState) {
  case Player::Idle: Model::update(deltaTime); break;
  case Player::OnLog: updateOnLog(deltaTime); break;
  case Player::Moving: updateMoving(deltaTime); break;
  case Player::Exploding: updateExploding(deltaTime); break;
  case Player::Dead: break;
  }
}

void Player::render() {
  switch (mState) {
  case Player::Idle: Model::render(); break;
  case Player::OnLog: Model::render(); break;
  case Player::Moving: Model::render(); break;
  case Player::Exploding: renderExploding(); break;
  case Player::Dead: break;
  }
}

void Player::moveTowards(const glm::vec3& direction) {
  mTargetPosition = mPosition + direction * TILE_SIZE;
  mStartPosition = mPosition;
  
  changeState(Player::Moving);
}

void Player::updateOnLog(int deltaTime) {
  Model::update(deltaTime);

  if (Game::instance().getKeyPressed('w'))
    moveTowards(IN);
  else if (Game::instance().getKeyPressed('s') && getPositionInTiles().z < 0)
    moveTowards(OUT);
}

void Player::updateMoving(int deltaTime) {
  Model::update(deltaTime);

  glm::vec3 current = glm::vec3(mPosition.x, 0.f, mPosition.z);
  glm::vec3 target  = glm::vec3(mTargetPosition.x, 0.f, mTargetPosition.z);
  glm::vec3 middle  = (mTargetPosition + mStartPosition) * 0.5f;
  glm::vec3 direction = glm::normalize(target - current);

  float distance = glm::distance(current, target);
  float maxDistance = glm::distance(middle, target);
  float curDistance = glm::distance(middle, current);

  float angle = atan2(-direction.z, direction.x) + (float)M_PI/2.f;
  setRotation(UP * angle);

  if (distance < kTol) {
    mPosition = mTargetPosition;
    changeState(Player::Idle);    
  } else {
    mVelocity = direction * kHorSpeed * distance * kDiminish;
    mPosition.y = (maxDistance - curDistance) * kJumpHeight + mStartPosition.y;
  }
}

void Player::updateExploding(int deltaTime) {
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

void Player::renderExploding() {
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
      particle->setTexture(Game::instance().getResource().texture("chunk_blank.png"));
    else if (texture == 1)
      particle->setTexture(Game::instance().getResource().texture("chunk_grass.png"));
    else
      particle->setTexture(Game::instance().getResource().texture("chunk_water.png"));

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
    setMesh(mFrames[0]);
    break;
  case Player::Moving:
    setMesh(mFrames[1]);
    Game::instance().getScene()->playSoundEffect("piu.ogg");
    break;
  case Player::Exploding:
    setVelocity(glm::vec3(0.f));
    Game::instance().getScene()->playSoundEffect("death.ogg");
    initExplosion();
    break;
  case Player::Dead:
    break;
  }
}

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