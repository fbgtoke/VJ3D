#include "Sun.h"
#include "Game.h"

Sun::Sun() {}

Sun::~Sun() {}

void Sun::init() {
  kTarget.x = Game::instance().getResource().Float("sunTarget_x");
  kTarget.y = Game::instance().getResource().Float("sunTarget_y");
  kTarget.z = Game::instance().getResource().Float("sunTarget_z");

  kDistance = Game::instance().getResource().Float("sunDistance");

  mCurrentTime = 0;
}

void Sun::update(int deltaTime) {
  mCurrentTime += (float)deltaTime * Game::instance().getResource().Float("sunSpeed");

}

void Sun::setLevelSize(unsigned int width, unsigned int length) {
  mWidth  = width;
  mLength = length;
}

glm::vec3 Sun::getTarget() const { return kTarget; }

glm::vec3 Sun::getPosition() const { return kTarget - getDirection() * kDistance; }

glm::vec3 Sun::getDirection() const {
  glm::vec3 dir;
  dir.x = cos(mCurrentTime);
  dir.y = abs(sin(mCurrentTime)) * (-1.f);
  dir.z = 0.f;

  dir = glm::normalize(dir);

  return dir;
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
  float left  = mLength * TILE_SIZE * -0.5f;
  float right = mLength * TILE_SIZE * 0.5f;
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
