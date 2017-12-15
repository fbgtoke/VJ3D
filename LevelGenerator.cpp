#include "LevelGenerator.h"

const int LevelGenerator::kPeriodSlow = 4000;
const int LevelGenerator::kPeriodFast = 2500;

const float LevelGenerator::kVelSlow = 0.03f;
const float LevelGenerator::kVelFast = 0.07f;

Level* LevelGenerator::generate(const std::string& name) {
  Level* level = new Level();

  LevelGenerator::generateTilemap(name, level);
  LevelGenerator::generatePlayer(name, level);
  LevelGenerator::generateObstacles(name, level);

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
        Obstacle* obstacle = LevelGenerator::generateObstacle(level, position, value);
        if (obstacle != nullptr) level->addObstacle(obstacle);
      }
    }
  }
}

Obstacle* LevelGenerator::generateObstacle(Level* level, const glm::vec3& position, int value) {
  Obstacle* obstacle;

  switch (value) {
  case 0:
    obstacle = new Obstacle(Obstacle::Cactus);
    obstacle->init();
    break;
  case 1:
    obstacle = new Obstacle(Obstacle::Stump);
    obstacle->init();
    break;
  case 2:
    obstacle = new Obstacle(Obstacle::Stone);
    obstacle->init();
    break;
  case 3:
    obstacle = new Obstacle(Obstacle::Bonus);
    obstacle->init();
    break;
  case 4:
  case 5:
  case 6:
  case 7:
  case 8:
  case 9:
  case 10:
  case 11:
  case 12:
  case 13:
  case 14:
  case 15:
    obstacle = LevelGenerator::generateSpawner(level, value);
    break;
  default:
    obstacle = nullptr;
    break;
  }

  obstacle->setPositionInTiles(position);

  return obstacle;
}

Obstacle* LevelGenerator::generateSpawner(Level* level, int value) {
  ObstacleSpawner* spawner;

  switch (value) {
  case 4:
    spawner = new ObstacleSpawner();
    spawner->init();
    spawner->setSpawnType(Obstacle::Carriage);
    spawner->setSpawnPeriod(kPeriodSlow);
    spawner->setSpawnVel(-kVelSlow);
    spawner->setNumberOfTiles(level->getTilemap()->getWidth());
    break;
  case 5:
    spawner = new ObstacleSpawner();
    spawner->init();
    spawner->setSpawnType(Obstacle::Carriage);
    spawner->setSpawnPeriod(kPeriodFast);
    spawner->setSpawnVel(-kVelFast);
    spawner->setNumberOfTiles(level->getTilemap()->getWidth());
    break;
  case 6:
    spawner = new ObstacleSpawner();
    spawner->init();
    spawner->setSpawnType(Obstacle::Carriage);
    spawner->setSpawnPeriod(kPeriodSlow);
    spawner->setSpawnVel(kVelSlow);
    spawner->setNumberOfTiles(level->getTilemap()->getWidth());
    break;
  case 7:
    spawner = new ObstacleSpawner();
    spawner->init();
    spawner->setSpawnType(Obstacle::Carriage);
    spawner->setSpawnPeriod(kPeriodFast);
    spawner->setSpawnVel(kVelFast);
    spawner->setNumberOfTiles(level->getTilemap()->getWidth());
    break;
  case 8:
    spawner = new ObstacleSpawner();
    spawner->init();
    spawner->setSpawnType(Obstacle::Horse);
    spawner->setSpawnPeriod(kPeriodSlow);
    spawner->setSpawnVel(-kVelSlow);
    spawner->setNumberOfTiles(level->getTilemap()->getWidth());
    break;
  case 9:
    spawner = new ObstacleSpawner();
    spawner->init();
    spawner->setSpawnType(Obstacle::Horse);
    spawner->setSpawnPeriod(kPeriodFast);
    spawner->setSpawnVel(-kVelFast);
    spawner->setNumberOfTiles(level->getTilemap()->getWidth());
    break;
  case 10:
    spawner = new ObstacleSpawner();
    spawner->init();
    spawner->setSpawnType(Obstacle::Horse);
    spawner->setSpawnPeriod(kPeriodSlow);
    spawner->setSpawnVel(kVelSlow);
    spawner->setNumberOfTiles(level->getTilemap()->getWidth());
    break;
  case 11:
    spawner = new ObstacleSpawner();
    spawner->init();
    spawner->setSpawnType(Obstacle::Horse);
    spawner->setSpawnPeriod(kPeriodFast);
    spawner->setSpawnVel(kVelFast);
    spawner->setNumberOfTiles(level->getTilemap()->getWidth());
    break;
  case 12:
    spawner = new ObstacleSpawner();
    spawner->init();
    spawner->setSpawnType(Obstacle::Boat);
    spawner->setSpawnPeriod(kPeriodSlow);
    spawner->setSpawnVel(-kVelSlow);
    spawner->setNumberOfTiles(level->getTilemap()->getWidth());
    break;
  case 13:
    spawner = new ObstacleSpawner();
    spawner->init();
    spawner->setSpawnType(Obstacle::Boat);
    spawner->setSpawnPeriod(kPeriodFast);
    spawner->setSpawnVel(-kVelFast);
    spawner->setNumberOfTiles(level->getTilemap()->getWidth());
    break;
  case 14:
    spawner = new ObstacleSpawner();
    spawner->init();
    spawner->setSpawnType(Obstacle::Boat);
    spawner->setSpawnPeriod(kPeriodSlow);
    spawner->setSpawnVel(kVelSlow);
    spawner->setNumberOfTiles(level->getTilemap()->getWidth());
    break;
  case 15:
    spawner = new ObstacleSpawner();
    spawner->init();
    spawner->setSpawnType(Obstacle::Boat);
    spawner->setSpawnPeriod(kPeriodFast);
    spawner->setSpawnVel(kVelFast);
    spawner->setNumberOfTiles(level->getTilemap()->getWidth());
    break;
  default:
    spawner = nullptr;
    break;
  }

  return spawner;
}