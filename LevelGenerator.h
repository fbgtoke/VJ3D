#ifndef _LEVEL_GENERATOR_INCLUDE
#define _LEVEL_GENERATOR_INCLUDE

#include "Level.h"
#include "Obstacle.h"
#include "ObstacleBonus.h"
#include "ObstacleSpawner.h"

class LevelGenerator {
public:
  static void generate(Level* level, const std::string& name);

private:
  static void generateTilemap(const std::string& name, Level* level);
  static void generatePlayer(const std::string& name, Level* level);
  static void generateObstacles(const std::string& name, Level* level);

  static Obstacle* generateObstacle(Level* level, const glm::vec3& position, int value);
  static Obstacle* generateSpawner(Level* level, int value);
};

#endif // _LEVEL_GENERATOR_INCLUDE