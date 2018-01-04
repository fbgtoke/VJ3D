#include "Sun.h"
#include "Game.h"

Sun::Sun() {}

Sun::~Sun() {}

void Sun::init() {
  kTarget.x = Game::instance().getResource().Float("sunTarget_x");
  kTarget.y = Game::instance().getResource().Float("sunTarget_y");
  kTarget.z = Game::instance().getResource().Float("sunTarget_z");

  kDistance = Game::instance().getResource().Float("sunDistance");

  kMorningTime = Game::instance().getResource().Float("sunMorningTime");
  kDayTime = Game::instance().getResource().Float("sunDayTime");
  kTwilightTime = Game::instance().getResource().Float("sunTwilightTime");
  kNightTime = Game::instance().getResource().Float("sunNightTime");

  kMorningColor.x = Game::instance().getResource().Float("sunMorningColor_x");
  kMorningColor.y = Game::instance().getResource().Float("sunMorningColor_y");
  kMorningColor.z = Game::instance().getResource().Float("sunMorningColor_z");

  kDayColor.x = Game::instance().getResource().Float("sunDayColor_x");
  kDayColor.y = Game::instance().getResource().Float("sunDayColor_y");
  kDayColor.z = Game::instance().getResource().Float("sunDayColor_z");

  kTwilightColor.x = Game::instance().getResource().Float("sunTwilightColor_x");
  kTwilightColor.y = Game::instance().getResource().Float("sunTwilightColor_y");
  kTwilightColor.z = Game::instance().getResource().Float("sunTwilightColor_z");

  kNightColor.x = Game::instance().getResource().Float("sunNightColor_x");
  kNightColor.y = Game::instance().getResource().Float("sunNightColor_y");
  kNightColor.z = Game::instance().getResource().Float("sunNightColor_z");


  mCurrentTime = 0;
}

void Sun::update(int deltaTime) {
  mCurrentTime += (float)deltaTime * Game::instance().getResource().Float("sunSpeed");

  if (mCurrentTime > (float)M_PI * 2.f)
    mCurrentTime -= (float)M_PI * 2.f;
}

void Sun::setCurrentTime(float time) { mCurrentTime = time; }

void Sun::setLevelSize(unsigned int width, unsigned int length) {
  mWidth  = width;
  mLength = length;
}

glm::vec3 Sun::getTarget() const { return kTarget; }

glm::vec3 Sun::getPosition() const { return kTarget - getDirection() * kDistance; }

glm::vec3 Sun::getDirection() const {
  if (mCurrentTime > kNightTime)
    return DOWN;

  glm::vec3 dir;
  dir.x = cos(mCurrentTime);
  dir.y = abs(sin(mCurrentTime)) * (-1.f);
  dir.z = 0.f;

  dir = glm::normalize(dir);

  return dir;
}

glm::vec3 Sun::getColor() const {
  if (mCurrentTime < kDayTime)
    return kMorningColor;
  if (mCurrentTime < kTwilightTime)
    return kDayColor;
  if (mCurrentTime < kNightTime)
    return kTwilightColor;
  if (mCurrentTime - (float)M_PI * 2.f < kMorningTime)
    return kNightColor;

  return glm::vec3(0.f);
}

glm::mat4 Sun::getBiasMatrix() const {
  return glm::mat4(
    0.5, 0.0, 0.0, 0.0,
    0.0, 0.5, 0.0, 0.0,
    0.0, 0.0, 0.5, 0.0,
    0.5, 0.5, 0.5, 1.0
  );
}

glm::mat4 Sun::getProjectionMatrix() const {
  float left  = mLength * TILE_SIZE * -1.5f;
  float right = mLength * TILE_SIZE * 1.5f;
  float bottom = mWidth * TILE_SIZE * -2.f;
  float top = mWidth * TILE_SIZE * 2.f;
  float znear = 0.f;
  float zfar = mWidth * TILE_SIZE * 4;
  return glm::ortho(left, right, bottom, top, znear, zfar);
}

glm::mat4 Sun::getViewMatrix() const {
  glm::vec3 vrp = getTarget();
  glm::vec3 obs = getPosition();

  return glm::lookAt(obs, vrp, UP);
}
