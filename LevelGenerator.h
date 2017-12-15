#ifndef _LEVEL_GENERATOR_INCLUDE
#define _LEVEL_GENERATOR_INCLUDE

#include "Level.h"
#include "Obstacle.h"
#include "ObstacleSpawner.h"

class LevelGenerator {
public:
  static Level* generate(const std::string& name);

private:
  static const int kPeriodSlow;
  static const int kPeriodFast;

  static const float kVelSlow;
  static const float kVelFast;

  static void generateTilemap(const std::string& name, Level* level);
  static void generatePlayer(const std::string& name, Level* level);
  static void generateObstacles(const std::string& name, Level* level);

  static Obstacle* generateObstacle(Level* level, const glm::vec3& position, int value);
  static Obstacle* generateSpawner(Level* level, int value);
};

#endif // _LEVEL_GENERATOR_INCLUDE