#include "InputManager.h"

bool InputManager::keys[256], InputManager::specialKeys[256];
bool InputManager::mKeysCurrent[256];
bool InputManager::mKeysPrevious[256];
bool InputManager::mSpecialKeysCurrent[256];
bool InputManager::mSpecialKeysPrevious[256];
glm::ivec2 InputManager::mMousePosition;
bool InputManager::mMouseButton;
bool InputManager::mMouseButtonCurrent;
bool InputManager::mMouseButtonPrevious;
bool InputManager::mTouchControlsEnabled;

void InputManager::init() {
  mTouchControlsEnabled = false;
}

void InputManager::update() {
  for (int i = 0; i < 256; ++i) {
    mKeysPrevious[i] = mKeysCurrent[i];
    mKeysCurrent[i] = getKey(i);

    mSpecialKeysPrevious[i] = mSpecialKeysCurrent[i];
    mSpecialKeysCurrent[i] = getSpecialKey(i);
  }

  mMouseButtonPrevious = mMouseButtonCurrent;
  mMouseButtonCurrent = mMouseButton;
}

bool InputManager::getAction(InputManager::Action action) {
  switch (action) {
  case InputManager::Left:
    return getKeyPressed('a') || (mTouchControlsEnabled && getMouseButtonPressed() && mouseIsInQuarter(2));
  case InputManager::Right:
    return getKeyPressed('d') || (mTouchControlsEnabled && getMouseButtonPressed() && mouseIsInQuarter(0));
  case InputManager::Up:
    return getKeyPressed('w') || (mTouchControlsEnabled && getMouseButtonPressed() && mouseIsInQuarter(1));
  case InputManager::Down:
    return getKeyPressed('s') || (mTouchControlsEnabled && getMouseButtonPressed() && mouseIsInQuarter(3));
  case InputManager::Accept:
    return getKeyPressed('z');
  case InputManager::Cancel:
    return getKeyPressed('x');
  case InputManager::Close:
    return getKeyPressed(27);
  }
  return false;
}

void InputManager::enableTouchControls(bool enable) { mTouchControlsEnabled = enable; }

void InputManager::keyPressed(int key) { keys[key] = true; }

void InputManager::keyReleased(int key) { keys[key] = false; }

void InputManager::specialKeyPressed(int key) { specialKeys[key] = true; }

void InputManager::specialKeyReleased(int key) { specialKeys[key] = false; }

void InputManager::mouseMove(int x, int y) { mMousePosition = glm::ivec2(x, y); }

void InputManager::mousePress(int button) {
  if (button == GLUT_LEFT_BUTTON)
    mMouseButton = true;
}

void InputManager::mouseRelease(int button) {
  if (button == GLUT_LEFT_BUTTON)
    mMouseButton = false;
}

bool InputManager::getKey(int key) { return keys[key]; }

bool InputManager::getSpecialKey(int key) { return specialKeys[key]; }

bool InputManager::getKeyPressed(int key) { return !mKeysPrevious[key] && mKeysCurrent[key]; }

bool InputManager::getKeyReleased(int key) { return mKeysPrevious[key] && !mKeysCurrent[key]; }

bool InputManager::getSpecialKeyPressed(int key) {
  return !mSpecialKeysPrevious[key] && mSpecialKeysCurrent[key];
}

bool InputManager::getSpecialKeyReleased(int key) {
  return mSpecialKeysPrevious[key] && !mSpecialKeysCurrent[key];
}

glm::ivec2 InputManager::getMousePosition() { return mMousePosition; }

bool InputManager::mouseIsInQuarter(unsigned int quarter) {
  glm::vec2 position(mMousePosition.x - SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f - mMousePosition.y); 
  float angle = atan2(position.y, position.x);
  if (angle < 0.f) angle += M_PI * 2.f;

  if (quarter == 1 && angle > M_PI * 0.25f && angle < M_PI * 0.75f) return true;
  else if (quarter == 2 && angle > M_PI * 0.75f && angle < M_PI * 1.25f) return true;
  else if (quarter == 3 && angle > M_PI * 1.25f && angle < M_PI * 1.75f) return true;
  else if (quarter == 0 && (angle < M_PI * 0.25f || angle > M_PI * 1.75f)) return true;
  return false;
}

bool InputManager::getMouseButton() { return mMouseButton; }

bool InputManager::getMouseButtonPressed() { return !mMouseButtonPrevious && mMouseButtonCurrent; }

bool InputManager::getMouseButtonReleased() { return mMouseButtonPrevious && !mMouseButtonCurrent; }