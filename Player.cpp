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
  mMovingTowardsTarget = false;
}

void Player::update(int deltaTime) {
  Model::update(deltaTime);

  if (mMovingTowardsTarget)
    updateMoving(deltaTime);
  else
    updateIdle(deltaTime);

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
}

void Player::render() {
  Model::render();

  for (Particle* particle : mParticles)
    if (particle->isAlive())
      particle->render();
}

void Player::moveTowards(const glm::vec3& direction) {
  mTargetPosition = mPosition + direction * TILE_SIZE;
  mStartPosition = mPosition;
  mMovingTowardsTarget = true;
  setMesh(mFrames[1]);
}

void Player::updateMoving(int deltaTime) {
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
    mVelocity = glm::vec3(0.f);
    mMovingTowardsTarget = false;
    setMesh(mFrames[0]);
  } else {
    mVelocity = direction * kHorSpeed * distance * kDiminish;
    mPosition.y = (maxDistance - curDistance) * kJumpHeight + mStartPosition.y;
  }
}

void Player::updateIdle(int deltaTime) {
  if (Game::instance().getKeyPressed('a') && getPositionInTiles().x > 0)
    moveTowards(LEFT);
  else if (Game::instance().getKeyPressed('d') && getPositionInTiles().x < TILES_PER_CHUNK - 1)
    moveTowards(RIGHT);
  else if (Game::instance().getKeyPressed('w'))
    moveTowards(IN);
  else if (Game::instance().getKeyPressed('s') && getPositionInTiles().z < 0)
    moveTowards(OUT);
  else if (Game::instance().getKeyPressed('e'))
    explode();
}

void Player::explode() {
  for (int i = 0; i < 250; ++i) {
    Particle* particle = new Particle();
    particle->init(750 + rand()%200);
    particle->setMesh(Game::instance().getResource().mesh("cube.obj"));

    int texture = rand()%3;
    if (texture == 0)
      particle->setTexture(Game::instance().getResource().texture("chunk_blank.png"));
    else if (texture == 1)
      particle->setTexture(Game::instance().getResource().texture("chunk_grass.png"));
    else
      particle->setTexture(Game::instance().getResource().texture("chunk_water.png"));

    particle->setPosition(getCenter() + DOWN * TILE_SIZE);
    particle->setScale(glm::vec3(0.125f));

    glm::vec3 direction;
    direction.x = randomFloat(-1.f, 1.f);
    direction.y = randomFloat( 0.f, 1.f);
    direction.z = randomFloat(-1.f, 1.f);
    particle->setVelocity(glm::normalize(direction) * 0.075f);

    mParticles.push_back(particle);
  }
}