#include "LevelGenerator.h"

const float LevelGenerator::kPeriodSlow = 2500;
const float LevelGenerator::kPeriodFast = 1200;

const float LevelGenerator::kVelSlow = 0.05f;
const float LevelGenerator::kVelFast = 0.15f;

Level* LevelGenerator::generate(const std::string& name) {
  Level* level = new Level();

  generateTilemap(name, level);
  generatePlayer(name, level);
  generateObstacles(name, level);

  return level;
}

void LevelGenerator::generateTilemap(const std::string& name, Level* level) {
  Tilemap* tilemap = new Tilemap();
  tilemap->loadFromFile(name + "_tilemap.csv");
  level->setTilemap(tilemap);
}

void LevelGenerator::generatePlayer(const std::string& name, Level* level) {
  Player* player = new Player();
  player->init();
  player->setPositionInTiles(glm::vec3(0, 0, 0));
  level->setPlayer(player);
}

void LevelGenerator::generateObstacles(const std::string& name, Level* level) {
  CsvReader reader;
  reader.loadFromFile(name + "_obstacles.csv");

  for (int i = 0; i < reader.getNumberOfRows(); ++i) {
    for (int j = 0; j < reader.getNumberOfColumns(); ++j) {
      glm::vec3 position;
      position.x = j;
      position.y = 0;
      position.z = (reader.getNumberOfRows() - 1 - i) * (-1);

      int value = reader.getCell(i, j);

      if (value != -1) {
        Obstacle* obstacle = generateObstacle(position, value);
        if (obstacle != nullptr) level->addObstacle(obstacle);
      }
    }
  }
}

Obstacle* LevelGenerator::generateObstacle(const glm::vec3& position, int value) {
  Obstacle* obstacle;
  ObstacleSpawner* spawner;

  switch (value) {
  case 0:
    obstacle = new Obstacle(Obstacle::Cactus);
    break;
  case 1:
    obstacle = new Obstacle(Obstacle::Stump);
    break;
  case 2:
    obstacle = new Obstacle(Obstacle::Stone);
    break;
  case 3:
    obstacle = new Obstacle(Obstacle::Bonus);
    break;
  case 4:
    obstacle = new ObstacleSpawner();
    spawner->setSpawnType(Obstacle::Carriage);
    spawner->setSpawnPeriod(kPeriodSlow);
    spawner->setSpawnVel(-kVelSlow);
    break;
  case 5:
    obstacle = spawner = new ObstacleSpawner();
    spawner->setSpawnType(Obstacle::Carriage);
    spawner->setSpawnPeriod(kPeriodFast);
    spawner->setSpawnVel(-kVelFast);
    break;
  case 6:
    obstacle = spawner = new ObstacleSpawner();
    spawner->setSpawnType(Obstacle::Carriage);
    spawner->setSpawnPeriod(kPeriodSlow);
    spawner->setSpawnVel(kVelSlow);
    break;
  case 7:
    obstacle = spawner = new ObstacleSpawner();
    spawner->setSpawnType(Obstacle::Carriage);
    spawner->setSpawnPeriod(kPeriodFast);
    spawner->setSpawnVel(kVelFast);
    break;
  case 8:
    obstacle = spawner = new ObstacleSpawner();
    spawner->setSpawnType(Obstacle::Horse);
    spawner->setSpawnPeriod(kPeriodSlow);
    spawner->setSpawnVel(-kVelSlow);
    break;
  case 9:
    obstacle = spawner = new ObstacleSpawner();
    spawner->setSpawnType(Obstacle::Horse);
    spawner->setSpawnPeriod(kPeriodFast);
    spawner->setSpawnVel(-kVelFast);
    break;
  case 10:
    obstacle = spawner = new ObstacleSpawner();
    spawner->setSpawnType(Obstacle::Horse);
    spawner->setSpawnPeriod(kPeriodSlow);
    spawner->setSpawnVel(kVelSlow);
    break;
  case 11:
    obstacle = spawner = new ObstacleSpawner();
    spawner->setSpawnType(Obstacle::Horse);
    spawner->setSpawnPeriod(kPeriodFast);
    spawner->setSpawnVel(kVelFast);
    break;
  case 12:
    obstacle = spawner = new ObstacleSpawner();
    spawner->setSpawnType(Obstacle::Boat);
    spawner->setSpawnPeriod(kPeriodSlow);
    spawner->setSpawnVel(-kVelSlow);
    break;
  case 13:
    obstacle = spawner = new ObstacleSpawner();
    spawner->setSpawnType(Obstacle::Boat);
    spawner->setSpawnPeriod(kPeriodFast);
    spawner->setSpawnVel(-kVelFast);
    break;
  case 14:
    obstacle = spawner = new ObstacleSpawner();
    spawner->setSpawnType(Obstacle::Boat);
    spawner->setSpawnPeriod(kPeriodSlow);
    spawner->setSpawnVel(kVelSlow);
    break;
  case 15:
    obstacle = spawner = new ObstacleSpawner();
    spawner->setSpawnType(Obstacle::Boat);
    spawner->setSpawnPeriod(kPeriodFast);
    spawner->setSpawnVel(kVelFast);
    break;
  default:
    obstacle = nullptr;
  }

  obstacle->init();
  obstacle->setPositionInTiles(position);

  return obstacle;
}