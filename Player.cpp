#include "Player.h"
#include "Game.h"

Player::Player() :
  mShadow(nullptr) {}

Player::~Player() {}

void Player::init() {
  ModelAnimated::init();

  kTol = Game::instance().getResource().Float("distanceTolerance");
  kJumpSpeed = Game::instance().getResource().Float("jumpSpeed");
  kHorSpeed = Game::instance().getResource().Float("horizontalSpeed");
  kMaxDrowningTime = Game::instance().getResource().Int("drowningTime");
  kMaxExplodingTime = Game::instance().getResource().Int("explodingTime");

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
  
  frame.mesh = nullptr;
  mAnimation.addFrame(3, frame);
  
  mAnimation.setTimePerFrame(100);
  mAnimation.changeAnimation(0);
  setMesh(mAnimation.getCurrentFrame()->mesh);
  
  setTexture(Game::instance().getResource().texture("palette.png"));

  mTargetPosition = glm::vec3(0.f);

  mState = Player::Idle;

  mTimer = 0;
  mTimerActivated = false;

  mShadow = new Shadow(this);
  mShadow->init();
}

void Player::update(int deltaTime) {
  ModelAnimated::update(deltaTime);
  mTimer -= deltaTime;

  if (mState == Player::Moving)
    updateMoving(deltaTime);

  if (mState == Player::towardsBoat) {
    mTargetPosition = mTargetBoat->getTopCenter();
    mTargetPosition.y += getSize().y * 0.5f;
    updateMoving(deltaTime);
  }

  if (mState == Player::Drowning) {
    mRotation.y = 0.f;
    mPosition.y = TILE_SIZE * 0.5f;
    
    if (timerExpired())
      explode();
  }

  if (mState == Player::Exploding && timerExpired())
    changeState(Player::Dead);

  if (isAlive() && mShadow != nullptr)
    mShadow->update(deltaTime);
}

void Player::render() {
  ModelAnimated::render();

  if (isAlive() && mShadow != nullptr)
    mShadow->render();
}

void Player::moveTowards(const glm::vec3& position) {
  mTargetPosition = position;
  
  changeState(Player::Moving);
}

void Player::moveTowardsBoat(Obstacle* boat) {
  mTargetBoat = boat;
  changeState(Player::towardsBoat);
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
  case Player::towardsBoat:
    mAnimation.changeAnimation(1);
    //Game::instance().getScene()->playSoundEffect("piu.ogg");
    break;
  case Player::onBoat:
    mPosition = mTargetBoat->getTopCenter();
    mVelocity.x = mTargetBoat->getVelocity().x;
    mVelocity.y = 0.f;
    mVelocity.z = 0.f;
    mRotation.y = (float)M_PI;
    mAnimation.changeAnimation(0);
    break;
  case Player::Drowning:
    setTimer(kMaxDrowningTime);
    mAnimation.changeAnimation(2);
    break;
  case Player::Exploding:
    setTimer(kMaxExplodingTime);
    setVelocity(glm::vec3(0.f));
    mAnimation.changeAnimation(3);
    //Game::instance().getScene()->playSoundEffect("death.ogg");
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

bool Player::checkCollisions() const { return true; }

void Player::setTimer(int time) {
  mTimerActivated = true;
  mTimer = time;
}

bool Player::timerExpired() const {
  return mTimerActivated && mTimer < 0;
}

void Player::onDestroy() {
  if (mShadow != nullptr) {
    mShadow->destroy();
    delete mShadow;
    mShadow = nullptr;
  }
}