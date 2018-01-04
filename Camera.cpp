#include "Camera.h"
#include "Game.h"

Camera::Camera()
  : mFollowing(nullptr) {}

Camera::~Camera() {}

void Camera::init() {
  kObsVector.x = Game::instance().getResource().Float("ObsVector_x");
  kObsVector.y = Game::instance().getResource().Float("ObsVector_y");
  kObsVector.z = Game::instance().getResource().Float("ObsVector_z");
  kVel = Game::instance().getResource().Float("CameraVel");

  OBS = glm::vec3(0.f);
  VRP = glm::vec3(0.f);

  mMoving = true;
}

void Camera::update(int deltaTime) {
  if (!mMoving) return;

  VRP += IN * kVel * (float)deltaTime;
  OBS += IN * kVel * (float)deltaTime;

  if (mFollowing != nullptr) {
    glm::vec3 center = mFollowing->getCenter();

    VRP.x = center.x;
    VRP.z = min(VRP.z, center.z);
    OBS = VRP + kObsVector * TILE_SIZE;
  }
}

void Camera::setMoving(bool moving) { mMoving = moving; }

void Camera::follow(const Model* model) {
  if (model != nullptr) {
    mFollowing = model;
    VRP = model->getCenter();
    OBS = model->getCenter() + kObsVector * TILE_SIZE;
  }
}

bool Camera::outOfView(const Model* model) const {
  glm::vec4 homoPosition(model->getCenter(), 1.0f);
  glm::vec4 projectedPosition =
    getProjectionMatrix() * getViewMatrix() * homoPosition;

  return projectedPosition.y/projectedPosition.w < -1.0f;
}

glm::mat4 Camera::getProjectionMatrix() const {
  return glm::ortho(
    6 * TILE_SIZE * (-1.f),
    6 * TILE_SIZE,
    2 * TILE_SIZE * (-1.f),
    (float)SCREEN_HEIGHT/(float)SCREEN_WIDTH * (12 * TILE_SIZE) - 2 * TILE_SIZE,
    0.1f,
    1000.f
  );
}

glm::mat4 Camera::getViewMatrix() const {
  return glm::lookAt(OBS, VRP, UP);
}