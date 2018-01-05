#ifndef _INPUT_MANAGER_INCLUDE
#define _INPUT_MANAGER_INCLUDE

#include "utils.h"

class InputManager {
public:
  static void init();
  static void update();

  enum Action {
    Left,
    Right,
    Up,
    Down,
    Accept,
    Cancel,
    Close
  };
  static bool getAction(InputManager::Action action);
  static void enableTouchControls(bool enable);

  // Input callback methods
  static void keyPressed(int key);
  static void keyReleased(int key);
  static void specialKeyPressed(int key);
  static void specialKeyReleased(int key);
  static void mouseMove(int x, int y);
  static void mousePress(int button);
  static void mouseRelease(int button);

  static bool getKey(int key);
  static bool getSpecialKey(int key);

  static void scanKeys();
  static bool getKeyPressed(int key);
  static bool getKeyReleased(int key);
  static bool getSpecialKeyPressed(int key);
  static bool getSpecialKeyReleased(int key);

  static glm::ivec2 getMousePosition();
  static bool mouseIsInQuarter(unsigned int quarter);

  static bool getMouseButton();
  static bool getMouseButtonPressed();
  static bool getMouseButtonReleased();

private:
  static bool keys[256], specialKeys[256];
  static bool mKeysCurrent[256];
  static bool mKeysPrevious[256];

  static bool mSpecialKeysCurrent[256];
  static bool mSpecialKeysPrevious[256];

  static glm::ivec2 mMousePosition;
  static bool mMouseButton;
  static bool mMouseButtonCurrent;
  static bool mMouseButtonPrevious;

  static bool mTouchControlsEnabled;
};

#endif // _INPUT_MANAGER_INCLUDE